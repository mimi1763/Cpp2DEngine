/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: math_solver.cpp
---------------------------------------------------
*/

//Math Solver API
//Consult your license regarding permissions and restrictions.
//Copyright (c) 2005-2011 by Jan Ivar Goli.

#include "math_solver.h"

//Dependencies
#include <ctime>
#include <cmath>

#include <limits>
#include <cctype>
#include <sstream>
#include <stack>
#include <utility>
#include <algorithm>
#include <iomanip>

namespace lunar {

//Private

//Symbol
//Public

math_solver::_Symbol::_Symbol():_Type(UNKNOWN),
	_Value(std::numeric_limits<real>::quiet_NaN()) {
	//Empty
}

math_solver::_Symbol::~_Symbol() {
	//Empty
}

//Range
//Protected

math_solver::_Range::_Range():_Type(_Symbol::UNKNOWN),
	_Level(0),_Parent(0) {
	//Empty
}

math_solver::_Range::~_Range() {

	for (_Ranges::const_iterator _Iter=_Myranges.begin(),
		_End=_Myranges.end();_Iter!=_End;++_Iter)
			delete (*_Iter);
}

//Conditional
//Protected

math_solver::_Conditional::_Conditional() {
	//Empty
}

math_solver::_Conditional::~_Conditional() {
	//Empty - dummy
}

//Math solver
//Public

//Result
//Private

void math_solver::result::copy(const result &_Result) {

_Answer=_Result._Answer;
_Solution=_Result._Solution;
_Generate_solution=_Result._Generate_solution;

_Valid=_Result._Valid;
}

//Result
//Public

math_solver::result::result():_Answer(0.0),
	_Generate_solution(true),_Valid(false) {
	//Empty
}

math_solver::result::result(const result &_Result) {
	copy(_Result);
}

math_solver::result::~result() {
	//Empty
}

math_solver::result &math_solver::result::operator=(
	const result &_Result) {

	//Worth doing
	if (this!=&_Result) copy(_Result);

return *this;
}

math_solver::result::operator bool() const {
	return _Valid;
}

math_solver::real math_solver::result::answer() const {
	return _Answer;
}

void math_solver::result::solution(steps &_Solution) const {
	_Solution=this->_Solution; //Make copy
}

void math_solver::result::generate_solution(bool _Generate_solution) {
	this->_Generate_solution=_Generate_solution;
}

bool math_solver::result::generate_solution() const {
	return _Generate_solution;
}

//Process
//Private

void math_solver::process::copy(const process &_Process) {
	_Result=_Process._Result;
}

//Process
//Public

math_solver::process::process(math_solver *_Owner):
	_Owner(_Owner),_Thread(0),_Working(false) {
	//Empty
}

math_solver::process::process(const process &_Process) {

_Owner=0; //Not copyable
_Thread=0; //Request thread

_Working=false; //Not copyable

copy(_Process);
}

math_solver::process::process(math_solver *_Owner,
	const process &_Process):_Owner(_Owner) {

_Thread=0; //Request thread
_Working=false; //Not copyable

copy(_Process);
}

math_solver::process::~process() {

	if (_Thread) {
	
	stop();
	delete _Thread;
	}
}

math_solver::process &math_solver::process::operator=(
	const process &_Process) {

	//Worth doing
	if (this!=&_Process) copy(_Process);

return *this;
}

bool math_solver::process::stop() {

	if (!_Thread || !_Working) return true;
	
	//Cancel the thread immediately
	if (pthread_cancel(*_Thread)==0) _Working=false;

return !_Working;
}

bool math_solver::process::working() const {
	return _Working;
}

const std::string &math_solver::process::input() const {
	return _Input;
}

math_solver::result &math_solver::process::output() {
	return _Result;
}

//Object
//Protected

math_solver::object::object(const std::string &_Name):
	_Name(_Name) {
	//Empty
}

math_solver::object::object(const object &_Object) {
	copy(_Object);
}

math_solver::object::object(const std::string &_Name,
	const object &_Object):_Name(_Name) {
	copy(_Object);
}

math_solver::object::~object() {
	//Empty
}

void math_solver::object::copy(const object &_Object) {

_Category=_Object._Category;
_Description=_Object._Description;
}

//Object
//Public

math_solver::object &math_solver::object::operator=(
	const object &_Object) {

	//Worth doing
	if (this!=&_Object) copy(_Object);

return *this;
}

const std::string &math_solver::object::name() const {
	return _Name;
}

void math_solver::object::category(const std::string &_Category) {
	this->_Category=_Category;
}

const std::string &math_solver::object::category() const {
	return _Category;
}

void math_solver::object::description(const std::string &_Description) {
	this->_Description=_Description;
}

const std::string &math_solver::object::description() const {
	return _Description;
}

//Constant
//Protected

math_solver::constant::constant(const std::string &_Name,
	real _Value):object(_Name),_Value(_Value) {
	//Empty
}

math_solver::constant::constant(const constant &_Constant):
	object(_Constant) {
	copy(_Constant);
}

math_solver::constant::constant(const std::string &_Name,
	const constant &_Constant):object(_Name,_Constant) {
	copy(_Constant);
}

math_solver::constant::~constant() {
	//Empty
}

void math_solver::constant::copy(const constant &_Constant) {
	_Value=_Constant._Value;
}

//Constant
//Public

math_solver::constant &math_solver::constant::operator=(
	const constant &_Constant) {

	//Worth doing
	if (this!=&_Constant) {
	
	object::operator=(_Constant); //Invoke base operator=
	copy(_Constant);
	}

return *this;
}

void math_solver::constant::value(real _Value) {
	this->_Value=_Value;
}

math_solver::real math_solver::constant::value() const {
	return _Value;
}

//Variable
//Protected

math_solver::variable::variable(const std::string &_Name,
	const std::string &_Value):object(_Name),_Value(_Value) {
	//Empty
}

math_solver::variable::variable(const variable &_Variable):
	object(_Variable) {
	copy(_Variable);
}

math_solver::variable::variable(const std::string &_Name,
	const variable &_Variable):object(_Name,_Variable) {
	copy(_Variable);
}

math_solver::variable::~variable() {
	//Empty
}

void math_solver::variable::copy(const variable &_Variable) {
	_Value=_Variable._Value;
}

//Variable
//Public

math_solver::variable &math_solver::variable::operator=(
	const variable &_Variable) {

	//Worth doing
	if (this!=&_Variable) {
	
	object::operator=(_Variable); //Invoke base operator=
	copy(_Variable);
	}

return *this;
}

void math_solver::variable::value(const std::string &_Value) {
	this->_Value=_Value;
}

const std::string &math_solver::variable::value() const {
	return _Value;
}

//Function
//Protected

math_solver::function::function(const std::string &_Name,
	ptr _Ptr,unsigned int _Parameters):
		object(_Name),_Ptr(_Ptr),_Parameters(_Parameters),
			_Parameters_required(_Parameters>0?_Parameters:1)  {
	//Empty
}

math_solver::function::function(const function &_Function):
	object(_Function) {
	copy(_Function);
}

math_solver::function::function(const std::string &_Name,
	const function &_Function):object(_Name,_Function) {
	copy(_Function);
}

math_solver::function::~function() {
	//Empty
}

void math_solver::function::copy(const function &_Function) {

_Ptr=_Function._Ptr;
_Parameters=_Function._Parameters;
_Parameters_required=_Function._Parameters_required;
}

//Function
//Public

math_solver::function &math_solver::function::operator=(
	const function &_Function) {

	//Worth doing
	if (this!=&_Function) {
	
	object::operator=(_Function); //Invoke base operator=
	copy(_Function);
	}

return *this;
}

math_solver::real math_solver::function::operator()(
	const arg &_Arg) {
	return (_Ptr?(*_Ptr)(_Arg):0.0);
}

void math_solver::function::pointer(ptr _Ptr) {
	this->_Ptr=_Ptr;
}

math_solver::function::ptr math_solver::function::pointer() const {
	return _Ptr;
}

void math_solver::function::parameters(unsigned int _Parameters) {

	if (_Parameters==0) _Parameters_required=1;
	else if (_Parameters_required>_Parameters)
		_Parameters_required=_Parameters;

this->_Parameters=_Parameters;
}

unsigned int math_solver::function::parameters() const {
	return _Parameters;
}

void math_solver::function::parameters_required(
	unsigned int _Parameters_required) {

	if (_Parameters==0 || _Parameters_required==0)
		this->_Parameters_required=1;
	else if (_Parameters_required>_Parameters)
		this->_Parameters_required=_Parameters;
	else this->_Parameters_required=_Parameters_required;
}

unsigned int math_solver::function::parameters_required() const {
	return _Parameters_required;
}

//Math solver
//Private

const math_solver::real math_solver::_Epsilon=std::
	numeric_limits<real>::epsilon();

const int math_solver::_Significand_precision=std::
	numeric_limits<real>::digits;

void math_solver::copy(const math_solver &_Math_solver) {

_Answer=_Math_solver._Answer;
_Error_log=_Math_solver._Error_log;
}

void math_solver::log(const std::string &_Error_message) {

//Get local time
time_t _Value;
time(&_Value);

struct tm _Time;
localtime_s(&_Time,&_Value);

std::stringstream _Time_stamp;
_Time_stamp<<"[";

//Hours
	if (_Time.tm_hour<10) _Time_stamp<<"0";

_Time_stamp<<_Time.tm_hour<<":";

//Minutes
	if (_Time.tm_min<10) _Time_stamp<<"0";

_Time_stamp<<_Time.tm_min<<":";

//Seconds
	if (_Time.tm_sec<10) _Time_stamp<<"0";

_Time_stamp<<_Time.tm_sec<<"]";

	//Enter shared section
	if (mutex_lock()) {
	
	//Write to log
	_Error_log.push_front(_Time_stamp.str()+" "+_Error_message);
	mutex_unlock(); //Exit shared section
	}
}

//Thread specific

bool math_solver::init_pthread() {

	//Init thread attribute
	if (pthread_attr_init(&_Thread_attribute)!=0) {
	
	_Thread_attribute=0;
	return false;
	}
	
	//Init thread mutex
	if (pthread_mutex_init(&_Thread_mutex,0)!=0) {
	
	_Thread_mutex=0;
	return false;
	}

//Set thread attribute as joinable
return (pthread_attr_setdetachstate(&_Thread_attribute,PTHREAD_CREATE_JOINABLE)==0);
}

bool math_solver::init_process(process *_Process) const {

	if (!_Process->_Thread) _Process->_Thread=new pthread_t();

_Process->_Working=true;
return (pthread_create(_Process->_Thread,&_Thread_attribute,
	&math_solver::start_solve,(void*)_Process)==0);
}

bool math_solver::mutex_lock() {
	return (pthread_mutex_lock(&_Thread_mutex)==0);
}

bool math_solver::mutex_unlock() {
	return (pthread_mutex_unlock(&_Thread_mutex)==0);
}

void *math_solver::start_solve(void *_Data) {

process *_Process=static_cast<process*>(_Data);
_Process->_Result._Valid=false;

_Process->_Myexpression.clear();
parse(_Process->_Input,_Process->_Myexpression);

	if (!_Process->_Owner->validate(_Process->_Myexpression)) {
	
	_Process->_Working=false;
	pthread_exit((void*)1);
	return (void*)1;
	}
	
	if (!_Process->_Owner->finalize(_Process->_Myexpression)) {
	
	_Process->_Working=false;
	pthread_exit((void*)1);
	return (void*)1;
	}
	
	if (!_Process->_Owner->calculate(_Process->_Myexpression,
		_Process->_Result)) {
	
	_Process->_Working=false;
	pthread_exit((void*)1);
	return (void*)1;
	}

_Process->_Result._Valid=true;

_Process->_Working=false;
pthread_exit((void*)0);
return (void*)0;
}

//Step 1 (parse)

void math_solver::parse(std::string _Data,_Expression &_Myexpression) {

	//Remove special characters outside symbol parentheses
	for (size_t i=0,_Level=0;i<_Data.size();++i) {
	
		if (_Data[i]=='{') ++_Level;
		else if (_Data[i]=='}') {
		
			if (_Level>0) --_Level;
		}
		else if ((unsigned char)_Data[i]<=' ') {
		
			if (_Level==0) _Data.erase(i--,1);
		}
		
		//Skip special ascii value (blank)
		else if (i+2<_Data.size() && _Data[i]=='\'' &&
			_Data[i+2]=='\'') i+=2;
	}
	
	//Nothing to do
	if (_Data.empty()) return;

bool _Locked=false;

	//Parse data to expression symbols
	for (size_t i=0;i<_Data.size();++i) {
	
	//1. Check for symbol parentheses { }
	
		if (_Data[i]=='{' && i+1<_Data.size()) {
		
			//Skip empty pair
			if (_Data[++i]=='}') continue;
		
		_Symbol _Mysymbol;
		
			//Search for the end parenthesis of this symbol
			for (unsigned int _Level=1;i<_Data.size();++i) {
			
				if (_Data[i]=='{') ++_Level;
				else if (_Data[i]=='}') {
				
					//Parentheses are balanced
					if (--_Level==0) break;
				}
			
			_Mysymbol._Data+=_Data[i];
			}
		
		_Myexpression.push_back(_Mysymbol);
		_Locked=true; //User symbol can not be modified
		continue;
		}
	
	unsigned int _Type=_Symbol::UNKNOWN;
	_Symbol *_Back=(_Myexpression.empty()?0:&_Myexpression.back());
	
	//2. Check for numbers and ascii characters
	
		if (i+2<_Data.size()) {
		
			//Check for number prefixes
			if (_Data.compare(i,2,"0x")==0 ||
				_Data.compare(i,2,"0X")==0) {
			
				if (hexadecimal_number(_Data[i+2]) ||
					decimal_point(_Data[i+2])) {
					_Type=_Symbol::NUMBER | _Symbol::NUM_HEXADECIMAL;
				}
			}
			else if (_Data.compare(i,2,"0d")==0 ||
					 _Data.compare(i,2,"0D")==0) {
			
				if ((decimal_number(_Data[i+2]) ||
					 decimal_point(_Data[i+2])) &&
						//Resolve ambiguity (hex higher priority)
						(!_Back || (_Back->_Type & _Symbol::NUMBER)==0 ||
						 (_Back->_Type & _Symbol::NUM_HEXADECIMAL)==0)) {
					_Type=_Symbol::NUMBER | _Symbol::NUM_DECIMAL;
				}
			}
			else if (_Data.compare(i,2,"0q")==0 ||
					 _Data.compare(i,2,"0Q")==0) {
			
				if (quaternary_number(_Data[i+2]) ||
					decimal_point(_Data[i+2])) {
					_Type=_Symbol::NUMBER | _Symbol::NUM_QUATERNARY;
				}
			}
			else if (_Data.compare(i,2,"0b")==0 ||
					 _Data.compare(i,2,"0B")==0) {
			
				if ((binary_number(_Data[i+2]) ||
					 decimal_point(_Data[i+2])) &&
						//Resolve ambiguity (hex higher priority)
						(!_Back || (_Back->_Type & _Symbol::NUMBER)==0 ||
						 (_Back->_Type & _Symbol::NUM_HEXADECIMAL)==0)) {
					_Type=_Symbol::NUMBER | _Symbol::NUM_BINARY;
				}
			}
			
			//Check for ascii character
			else if (_Data[i]=='\'' && _Data[i+2]=='\'') {
				_Type=_Symbol::ASCII;
			}
			
			//Add new symbol to expression
			if (_Type!=_Symbol::UNKNOWN) {
			
			_Symbol _Mysymbol;
			_Mysymbol._Type=_Type;
			_Mysymbol._Data=_Data.substr(i,3);
			_Myexpression.push_back(_Mysymbol);
			
			i+=2; //Skip
			continue;
			}
		}
		
		//Check for all supported number symbols
		if (hexadecimal_number(_Data[i]) || decimal_point(_Data[i])) {
		
			if (_Back && _Back->_Type & _Symbol::NUMBER) {
			
				//Check for decimal point
				if (decimal_point(_Data[i])) {
				
				bool _Unique=true;
				
					//Only one decimal point is allowed
					for (size_t j=0;j<_Back->_Data.size();++j) {
					
						if (decimal_point(_Back->_Data[j]) ||
							//Decimal point not allowed after exponent
							_Back->_Data[j]=='E') {
						
						_Unique=false;
						break;
						}
					}
					
					if (_Unique) _Type=_Back->_Type;
				}
				
				//Check for octal prefix
				else if (_Back->_Data=="0") {
				
					if (octal_number(_Data[i])) {
					
					_Back->_Type^=_Symbol::NUM_DECIMAL; //Remove
					_Back->_Type|=_Symbol::NUM_OCTAL; //Add
					_Type=_Back->_Type;
					}
				}
				
				//Check for compatibility with previous number type
				//Hexadecimal
				else if ((_Back->_Type & _Symbol::NUM_HEXADECIMAL &&
						  hexadecimal_number(_Data[i])) ||
				//Octal
						 (_Back->_Type & _Symbol::NUM_OCTAL &&
						  octal_number(_Data[i])) ||
				//Quaternary
						 (_Back->_Type & _Symbol::NUM_QUATERNARY &&
						  quaternary_number(_Data[i])) ||
				//Binary
						 (_Back->_Type & _Symbol::NUM_BINARY &&
						  binary_number(_Data[i])))
					_Type=_Back->_Type;
			}
			
			//Check for decimal number type (default)
			if (_Type==_Symbol::UNKNOWN && decimal_number(_Data[i])) {
			
			_Type=_Symbol::NUMBER | _Symbol::NUM_DECIMAL;
			
				if (_Back && !_Locked) {
				
					//Check if previous unknown has decimal point
					if (_Back->_Type==_Symbol::UNKNOWN && decimal_point(
						_Back->_Data[_Back->_Data.size()-1])) {
					
						//Use as decimal point
						if (_Back->_Data.size()==1) {
						
							//Only if previous is not number
							if (_Myexpression.size()<2 || ((*(++_Myexpression.
								rbegin()))._Type & _Symbol::NUMBER)==0)
									_Back->_Type=_Type; //Redefine
						}
						else {
						
						//Add intermediate symbol
						_Symbol _Mysymbol;
						_Mysymbol._Type=_Type;
						_Mysymbol._Data=_Back->_Data[_Back->_Data.size()-1];
						_Myexpression.push_back(_Mysymbol);
						
						_Back->_Data.erase(_Back->_Data.size()-1,1);
						_Back=&_Myexpression.back(); //Update back pointer
						}
					}
					
					//Check for number exponent signature (E, E+, E-)
					else if (_Myexpression.size()>1) {
					
					_Expression::reverse_iterator _Riter=
						_Myexpression.rbegin();
					
						//Check for add/subtract operator
						if (_Myexpression.size()>2 &&
							_Back->_Type & _Symbol::OPERATOR &&
							_Back->_Type & _Symbol::OP_BINARY &&
								(_Back->_Type & _Symbol::OP_ADD ||
								 _Back->_Type & _Symbol::OP_SUBTRACT))
									++_Riter; //Skip add/subtract
						
						//Check for exponent symbol
						if ((*_Riter)._Type==_Symbol::UNKNOWN &&
							(*_Riter)._Data=="E") {
						
						_Symbol &_Mysymbol=(*++_Riter);
						
							//Only if previous is decimal number type
							if (_Mysymbol._Type & _Symbol::NUMBER &&
								_Mysymbol._Type & _Symbol::NUM_DECIMAL) {
							
							bool _Unique=true;
							
								//Only one exponent symbol is allowed
								for (size_t j=0;j<_Mysymbol._Data.size();++j) {
								
									if (_Mysymbol._Data[j]=='E') {
									
									_Unique=false;
									break;
									}
								}
								
								if (_Unique) {
								
									for (_Expression::const_iterator _Iter=_Riter.base(),
										_End=_Myexpression.end();_Iter!=_End;++_Iter)
											_Mysymbol._Data+=(*_Iter)._Data;
								
								_Myexpression.erase(_Riter.base(),
									_Myexpression.end()); //Remove E, E+, E-
								_Back=&_Myexpression.back(); //Update back pointer
								}
							}
						}
					}
				}
			}
			
			if (_Type!=_Symbol::UNKNOWN) {
			
				//Concatenate with previous symbol
				if (_Back && _Back->_Type==_Type)
					_Back->_Data+=_Data[i];
				else {
				
				//Add new symbol to expression
				_Symbol _Mysymbol;
				_Mysymbol._Type=_Type;
				_Mysymbol._Data=_Data[i];
				_Myexpression.push_back(_Mysymbol);
				}
			
			continue;
			}
		}
	
	//3. Check for operators, parentheses and delimiters
	
	unsigned int _Symbol_size=1; //Default
	
		//Check for multi-sized symbols first
		//Operators
		//Binary (arithmetic)
		if (_Data.compare(i,2,"**")==0) {
		
		_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
			_Symbol::OP_POWER;
		_Symbol_size=2;
		}
		
		//Binary (relational)
		else if (_Data.compare(i,2,"==")==0) {
		
		_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
			_Symbol::OP_EQUAL;
		_Symbol_size=2;
		}
		else if (_Data.compare(i,2,"<>")==0 ||
				 _Data.compare(i,2,"!=")==0 ||
				 _Data.compare(i,2,"~=")==0) {
		
		_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
			_Symbol::OP_DIFFERENT;
		_Symbol_size=2;
		}
		else if (_Data.compare(i,2,">=")==0) {
		
		_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
			_Symbol::OP_GREATER_EQUAL;
		_Symbol_size=2;
		}
		else if (_Data.compare(i,2,"<=")==0) {
		
		_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
			_Symbol::OP_LESS_EQUAL;
		_Symbol_size=2;
		}
		
		//Binary (logical)
		else if (_Data.compare(i,2,"&&")==0) {
		
		_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
			_Symbol::OP_AND;
		_Symbol_size=2;
		}
		else if (_Data.compare(i,2,"||")==0) {
		
		_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
			_Symbol::OP_OR;
		_Symbol_size=2;
		}
		
		//Binary (bitwise)
		else if (_Data.compare(i,2,"<<")==0) {
		
		_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
			_Symbol::OP_SHIFT_LEFT;
		_Symbol_size=2;
		}
		else if (_Data.compare(i,2,">>")==0) {
		
		_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
			_Symbol::OP_SHIFT_RIGHT;
		_Symbol_size=2;
		}
		
		//Check for single-sized symbols
		//Operators
		//Unary (logical/bitwise)
		else if (_Data.compare(i,1,"!")==0) {
			_Type=_Symbol::OPERATOR | _Symbol::OP_UNARY |
				_Symbol::OP_NOT;
		}
		else if (_Data.compare(i,1,"~")==0) {
			_Type=_Symbol::OPERATOR | _Symbol::OP_UNARY |
				_Symbol::OP_COMPLEMENT;
		}
		
		//Binary (arithmetic)
		else if (_Data.compare(i,1,"+")==0) {
			_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
				_Symbol::OP_ADD;
		}
		else if (_Data.compare(i,1,"-")==0) {
			_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
				_Symbol::OP_SUBTRACT;
		}
		else if (_Data.compare(i,1,"*")==0) {
			_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
				_Symbol::OP_MULTIPLY;
		}
		else if (_Data.compare(i,1,"/")==0 ||
				 _Data.compare(i,1,"\\")==0) {
			_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
				_Symbol::OP_DIVIDE;
		}
		else if (_Data.compare(i,1,"%")==0) {
			_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
				_Symbol::OP_MODULO;
		}
		else if (_Data.compare(i,1,"^")==0) {
			_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
				_Symbol::OP_POWER;
		}
		
		//Binary (relational)
		else if (_Data.compare(i,1,"=")==0) {
			_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
				_Symbol::OP_EQUAL;
		}
		else if (_Data.compare(i,1,">")==0) {
			_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
				_Symbol::OP_GREATER;
		}
		else if (_Data.compare(i,1,"<")==0) {
			_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
				_Symbol::OP_LESS;
		}
		
		//Binary (bitwise)
		else if (_Data.compare(i,1,"&")==0) {
			_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
				_Symbol::OP_BIT_AND;
		}
		else if (_Data.compare(i,1,"|")==0) {
			_Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
				_Symbol::OP_BIT_OR;
		}
		
		//Ternary (conditional)
		else if (_Data.compare(i,1,"?")==0) {
			_Type=_Symbol::OPERATOR | _Symbol::OP_TERNARY |
				_Symbol::OP_IF;
		}
		else if (_Data.compare(i,1,":")==0) {
			_Type=_Symbol::OPERATOR | _Symbol::OP_TERNARY |
				_Symbol::OP_ELSE;
		}
		
		//Parentheses
		//Left
		else if (_Data.compare(i,1,"(")==0 ||
				 _Data.compare(i,1,"[")==0) {
			_Type=_Symbol::PARENTHESIS | _Symbol::PAR_LEFT;
		}
		
		//Right
		else if (_Data.compare(i,1,")")==0 ||
				 _Data.compare(i,1,"]")==0) {
			_Type=_Symbol::PARENTHESIS | _Symbol::PAR_RIGHT;
		}
		
		//Delimiters
		else if (_Data.compare(i,1,";")==0) {
			_Type=_Symbol::DELIMITER;
		}
		
		//Concatenate with previous symbol
		if (_Type==_Symbol::UNKNOWN && _Back &&
			_Back->_Type==_Symbol::UNKNOWN && !_Locked)
				_Back->_Data+=_Data[i];
		else {
		
			//Resolve special cases
			//Check for unary positive/negative
			if ((_Type & _Symbol::OPERATOR && _Type & _Symbol::OP_BINARY &&
				(_Type & _Symbol::OP_ADD || _Type & _Symbol::OP_SUBTRACT)) &&
					(!_Back || _Back->_Type & _Symbol::OPERATOR ||
					 (_Back->_Type & _Symbol::PARENTHESIS &&
					  _Back->_Type & _Symbol::PAR_LEFT) ||
					  _Back->_Type & _Symbol::DELIMITER)) {
			
			_Type^=_Symbol::OP_BINARY; //Remove
			_Type|=_Symbol::OP_UNARY; //Add
			
				if (_Type & _Symbol::OP_ADD) {
				
				_Type^=_Symbol::OP_ADD; //Remove
				_Type|=_Symbol::OP_POSITIVE; //Add
				}
				else {
				
				_Type^=_Symbol::OP_SUBTRACT; //Remove
				_Type|=_Symbol::OP_NEGATIVE; //Add
				}
			}
		
		//Add new symbol to expression
		_Symbol _Mysymbol;
		_Mysymbol._Type=_Type;
		_Mysymbol._Data=_Data.substr(i,_Symbol_size);
		_Myexpression.push_back(_Mysymbol);
		
			//Make sure added symbol is unlocked
			if (_Type==_Symbol::UNKNOWN) _Locked=false;
			
			//Skip if multi-symbol added
			if (_Symbol_size>1) i+=_Symbol_size-1;
		}
	}
}

bool  math_solver::hexadecimal_number(unsigned char _Value) {
	return (decimal_number(_Value) || (_Value>='a' && _Value<='f') ||
		(_Value>='A' && _Value<='F'));
}

bool  math_solver::decimal_number(unsigned char _Value) {
	return (_Value>='0' && _Value<='9');
}

bool  math_solver::octal_number(unsigned char _Value) {
	return (_Value>='0' && _Value<='7');
}

bool  math_solver::quaternary_number(unsigned char _Value) {
	return (_Value>='0' && _Value<='3');
}

bool  math_solver::binary_number(unsigned char _Value) {
	return (_Value=='0' || _Value=='1');
}

bool  math_solver::decimal_point(unsigned char _Value) {
	return (_Value=='.' || _Value==',');
}

//Step 2 (validate)

bool math_solver::validate(_Expression &_Myexpression) {

	if (_Myexpression.empty()) {
	
	log("Nothing to solve");
	return false;
	}

std::list<std::pair<std::string,_Expression::iterator>>
	_Current_variables; //For tracing circular reference

	//Find and replace unknowns with constants, variables and functions
	for (_Expression::iterator _Iter=_Myexpression.begin(),
		_End=_Myexpression.end();_Iter!=_End;) {
	
	_Symbol &_Mysymbol=(*_Iter); //Increase speed
	
		//Done tracing variable(s)
		while (!_Current_variables.empty() &&
			_Current_variables.back().second==_Iter)
				_Current_variables.pop_back();
		
		//Nothing to do
		if (_Mysymbol._Type!=_Symbol::UNKNOWN) {
		
		++_Iter;
		continue;
		}
	
	//Check for variable
	variable *_Variable=variable_pointer(_Mysymbol._Data);
	
		if (_Variable) {
		
			for (std::list<std::pair<std::string,_Expression::iterator>>::
				const_iterator _Viter=_Current_variables.begin(),
					_Vend=_Current_variables.end();_Viter!=_Vend;++_Viter) {
			
				//Check for circular reference
				if (_Mysymbol._Data==(*_Viter).first) {
				
				log("Circular reference in variable '"+
					_Mysymbol._Data+"'");
				return false;
				}
			}
		
		_Expression _Var_expression;
		parse(_Variable->_Value,_Var_expression);
		
			if (_Var_expression.empty()) {
			
			log("Variable '"+_Mysymbol._Data+"' not initialized");
			return false;
			}
		
		std::string _Variable_name=_Mysymbol._Data;
		
		_Iter=_Myexpression.erase(_Iter);
		_Myexpression.insert(_Iter,_Var_expression.begin(),
			_Var_expression.end());
		
		//Start tracing variable
		_Current_variables.push_back(std::pair<std::string,
			_Expression::iterator>(_Variable_name,_Iter));
		
		std::advance(_Iter,-(int)_Var_expression.size());
		continue;
		}
	
	//Check for constant
	constant *_Constant=constant_pointer(_Mysymbol._Data);
	
		if (_Constant) {
		
		_Mysymbol._Type=_Symbol::NUMBER | _Symbol::NUM_DECIMAL;
		_Mysymbol._Value=_Constant->_Value;
		++_Iter;
		continue;
		}
	
	//Check for function
	function *_Function=function_pointer(_Mysymbol._Data);
	
		if (_Function) {
		
		_Mysymbol._Type=_Symbol::FUNCTION;
		_Mysymbol._Value=(size_t)_Function;
		++_Iter;
		continue;
		}
		
		//Check for special keywords
		//ans is defined as the previous answer
		if (_Mysymbol._Data=="ans") {
		
		_Mysymbol._Type=_Symbol::NUMBER | _Symbol::NUM_DECIMAL;
		_Mysymbol._Value=_Answer;
		++_Iter;
		continue;
		}
	
	log("Undefine symbol '"+_Mysymbol._Data+"'");
	return false;
	}

unsigned int _Level=0; //For parentheses
std::stack<unsigned int> _Conditionals; //For conditional nesting
std::stack<std::pair<unsigned int,std::pair<function*,unsigned int>>>
	_Functions; //For function parentheses

	for (_Expression::iterator _Iter=_Myexpression.begin(),
		_End=_Myexpression.end();_Iter!=_End;++_Iter) {
	
	_Symbol &_Mysymbol=(*_Iter); //Increase speed
	
	_Symbol *_Next_symbol=(++_Iter!=_End?&*_Iter:0);
	
	//1. Add multiply operator if missing
	
		//Allowed after
		if (_Next_symbol &&
			(_Mysymbol._Type & _Symbol::NUMBER ||
			 _Mysymbol._Type & _Symbol::ASCII ||
			(_Mysymbol._Type & _Symbol::PARENTHESIS &&
				_Mysymbol._Type & _Symbol::PAR_RIGHT))) {
		
			//Allowed before
			if (_Next_symbol->_Type & _Symbol::NUMBER ||
				_Next_symbol->_Type & _Symbol::ASCII ||
				(_Next_symbol->_Type & _Symbol::OPERATOR &&
					_Next_symbol->_Type & _Symbol::OP_UNARY) ||
				(_Next_symbol->_Type & _Symbol::PARENTHESIS &&
					_Next_symbol->_Type & _Symbol::PAR_LEFT) ||
				_Next_symbol->_Type & _Symbol::FUNCTION) {
			
			//Insert multiply operator
			_Symbol _Multiply;
			_Multiply._Type=_Symbol::OPERATOR | _Symbol::OP_BINARY |
				_Symbol::OP_MULTIPLY;
			_Multiply._Data="*";
			_Myexpression.insert(_Iter,_Multiply);
			--_Iter;
			}
		}
	
	--_Iter;
	
	//2. Check operators
	
		if (_Mysymbol._Type & _Symbol::OPERATOR) {
		
		bool _Valid=true;
		
			//Binary/ternary
			//Check left operand
			if ((_Mysymbol._Type & _Symbol::OP_UNARY)==0) {
			
			_Symbol *_Prev_symbol=(_Iter!=_Myexpression.begin()?
				&*(--_Iter):0);
			
				if (_Prev_symbol) ++_Iter;
				
				if (!_Prev_symbol ||
					(_Prev_symbol->_Type & _Symbol::OPERATOR &&
						(_Prev_symbol->_Type & _Symbol::OP_UNARY)==0) ||
					(_Prev_symbol->_Type & _Symbol::PARENTHESIS &&
						_Prev_symbol->_Type & _Symbol::PAR_LEFT) ||
					 _Prev_symbol->_Type & _Symbol::DELIMITER)
						_Valid=false;
			}
			
			//Unary/binary/ternary
			//Check right-operand
			if (_Valid && (!_Next_symbol ||
				(_Next_symbol->_Type & _Symbol::OPERATOR &&
					(_Next_symbol->_Type & _Symbol::OP_UNARY)==0) ||
				(_Next_symbol->_Type & _Symbol::PARENTHESIS &&
					_Next_symbol->_Type & _Symbol::PAR_RIGHT) ||
				 _Next_symbol->_Type & _Symbol::DELIMITER))
					_Valid=false;
			
			if (!_Valid) {
			
			std::string _Arity;
			
				if (_Mysymbol._Type & _Symbol::OP_UNARY)
					_Arity="unary";
				else if (_Mysymbol._Type & _Symbol::OP_BINARY)
					_Arity="binary";
				else _Arity="ternary";
			
			log("Invalid operand for "+_Arity+" operator '"+
				_Mysymbol._Data+"'");
			return false;
			}
			
			//Ternary
			if (_Mysymbol._Type & _Symbol::OP_TERNARY) {
			
				//Add conditional to stack
				if (_Mysymbol._Type & _Symbol::OP_IF)
					_Conditionals.push(_Level); 
				else if (_Mysymbol._Type & _Symbol::OP_ELSE) {
				
					if (_Conditionals.empty() ||
						_Conditionals.top()!=_Level) {
					
					log("Missing if condition");
					return false;
					}
				
				_Conditionals.pop();
				}
			}
		}
	
	//3. Check parentheses
	
		else if (_Mysymbol._Type & _Symbol::PARENTHESIS) {
		
			//Left
			if (_Mysymbol._Type & _Symbol::PAR_LEFT) {
			
				//Empty pair?
				if (_Next_symbol &&
					_Next_symbol->_Type & _Symbol::PARENTHESIS &&
					_Next_symbol->_Type & _Symbol::PAR_RIGHT) {
				
				log("Empty parentheses");
				return false;
				}
			
			++_Level;
			}
			//Right
			else {
			
				//Check if unbalanced parenthesis
				if (_Level==0) {
				
				log("Missing left parenthesis");
				return false;
				}
				
				//Check if invalid conditional
				if (!_Conditionals.empty() &&
					_Conditionals.top()==_Level) {
				
				log("Missing else condition");
				return false;
				}
				
				//Check if function parenthesis
				if (!_Functions.empty() &&
					_Functions.top().first==_Level) {
				
					//Check parameters required
					if (_Functions.top().second.second<_Functions.
						top().second.first->_Parameters_required) {
					
					log("Too few arguments for function '"+
						_Functions.top().second.first->_Name+"'");
					return false;
					}
				
				_Functions.pop();
				}
			
			--_Level;
			}
		}
	
	//4. Check delimiters
	
		else if (_Mysymbol._Type & _Symbol::DELIMITER) {
		
			//Misplaced delimiter
			if (_Functions.empty() ||
				_Functions.top().first!=_Level) {
			
			log("Delimiters only allowed inside function parentheses");
			return false;
			}
			
			//Misplaced delimiter
			if (!_Conditionals.empty() &&
				_Conditionals.top()==_Level) {
			
			log("Delimiters only allowed outside conditionals");
			return false;
			}
			
			//Check argument count
			if (_Functions.top().second.first->_Parameters>0 &&
				_Functions.top().second.first->_Parameters==
					_Functions.top().second.second) {
			
			log("Too many arguments for function '"+
				_Functions.top().second.first->_Name+"'");
			return false;
			}
		
		_Symbol *_Prev_symbol=&*(--_Iter); //Safe
		++_Iter;
		
			//Empty argument?
			//Check previous
			if ((_Prev_symbol &&
				 _Prev_symbol->_Type & _Symbol::PARENTHESIS &&
					_Prev_symbol->_Type & _Symbol::PAR_LEFT) ||
			//Check next
				(_Next_symbol &&
				(_Next_symbol->_Type & _Symbol::DELIMITER ||
				 (_Next_symbol->_Type & _Symbol::PARENTHESIS &&
					_Next_symbol->_Type & _Symbol::PAR_RIGHT)))) {
			
			log("Empty argument in function '"+
				_Functions.top().second.first->_Name+"'");
			return false;
			}
		
		++_Functions.top().second.second; //Add argument
		}
	
	//5. Check functions
	
		else if (_Mysymbol._Type & _Symbol::FUNCTION) {
		
			//Missing parenthesis
			if (!_Next_symbol ||
				(_Next_symbol->_Type & _Symbol::PARENTHESIS)==0 ||
				(_Next_symbol->_Type & _Symbol::PAR_LEFT)==0) {
			
			log("Function '"+_Mysymbol._Data+"' is missing parenthesis");
			return false;
			}
		
		//Add function to stack
		_Functions.push(std::pair<unsigned int,std::pair<function*,
			unsigned int>>(_Level+1,std::pair<function*,unsigned int>(
				(function*)(size_t)_Mysymbol._Value,1)));
		}
	}
	
	//Unbalanced parenthesis
	if (_Level>0) {
	
	log("Missing right parenthesis");
	return false;
	}
	
	//Invalid conditional
	if (!_Conditionals.empty()) {
	
	log("Missing else condition");
	return false;
	}

return true;
}

//Step 3 (finalize)

bool math_solver::finalize(_Expression &_Myexpression) {

unsigned int _Level=0; //For parentheses
std::stack<std::pair<unsigned int,std::pair<_Expression::iterator,bool>>>
	_Parentheses; //Parentheses data

	for (_Expression::iterator _Iter=_Myexpression.begin(),
		_End=_Myexpression.end();_Iter!=_End;) {
	
	_Symbol &_Mysymbol=(*_Iter); //Increase speed
	
		//Reduce unary positive/negative operators
		if (_Mysymbol._Type & _Symbol::OPERATOR) {
		
			if (_Mysymbol._Type & _Symbol::OP_UNARY &&
					(_Mysymbol._Type & _Symbol::OP_POSITIVE ||
					 _Mysymbol._Type & _Symbol::OP_NEGATIVE)) {
			
			_Symbol *_Next_symbol=(++_Iter!=_End?&*_Iter:0);
			--_Iter;
			
			bool _Reduce=(_Mysymbol._Type & _Symbol::OP_POSITIVE)!=0;
			
				if (_Next_symbol &&
					_Next_symbol->_Type & _Symbol::OPERATOR &&
					_Next_symbol->_Type & _Symbol::OP_UNARY &&
						(_Next_symbol->_Type & _Symbol::OP_POSITIVE ||
						 _Next_symbol->_Type & _Symbol::OP_NEGATIVE)) {
				
					//Reduce to +
					if (_Mysymbol._Type==_Next_symbol->_Type) {
					
						if (_Next_symbol->_Type & _Symbol::OP_NEGATIVE) {
						
						_Next_symbol->_Type^=_Symbol::OP_NEGATIVE; //Remove
						_Next_symbol->_Type|=_Symbol::OP_POSITIVE; //Add
						_Next_symbol->_Data="+";
						}
					}
					//Reduce to -
					else {
					
						if (_Next_symbol->_Type & _Symbol::OP_POSITIVE) {
						
						_Next_symbol->_Type^=_Symbol::OP_POSITIVE; //Remove
						_Next_symbol->_Type|=_Symbol::OP_NEGATIVE; //Add
						_Next_symbol->_Data="-";
						}
					}
				
				_Reduce=true;
				}
				
				if (_Reduce) {
				
				_Iter=_Myexpression.erase(_Iter);
				continue;
				}
			}
		}
		
		//Convert data and add sign to numbers
		else if (_Mysymbol._Type & _Symbol::NUMBER ||
				 _Mysymbol._Type & _Symbol::ASCII) {
		
		_Symbol *_Prev_symbol=(_Iter!=_Myexpression.begin()?
			&*(--_Iter):0);
		
		bool _Negate=false;
		
			if (_Prev_symbol) {
			
				//Check sign
				if (_Prev_symbol->_Type & _Symbol::OPERATOR &&
					_Prev_symbol->_Type & _Symbol::OP_UNARY &&
					_Prev_symbol->_Type & _Symbol::OP_NEGATIVE) {
				
					//Negate
					if (nan(_Mysymbol._Value)) _Negate=true;
					else negate(_Mysymbol);
				
				_Iter=_Myexpression.erase(_Iter);
				}
				else ++_Iter;
			}
			
			//Convert data to value
			if (nan(_Mysymbol._Value)) {
			
				//Number
				if (_Mysymbol._Type & _Symbol::NUMBER) {
				
				std::string _Data=_Mysymbol._Data; //Copy, don't change original data
				
					//Replace , with .
					for (size_t i=0;i<_Data.size();++i) {
					
						if (_Data[i]==',') _Data[i]='.';
						if (_Data[i]=='.') break;
					}
					
					//Decimal
					if (_Mysymbol._Type & _Symbol::NUM_DECIMAL) {
					
					std::stringstream _Stream;
					
						//Check for prefix (forced decimal)
						if (_Data.size()>2 && std::tolower((unsigned char)
							_Data[1])=='d') _Stream<<_Data.substr(2);
						else _Stream<<_Data;
					
					_Stream>>_Mysymbol._Value; //Safe
					}
					else {
					
					unsigned int _Off=0; //Actual data offset (prefix size)
					unsigned int _Bits=0; //Bits representing one digit
					
						//Hexadecimal
						if (_Mysymbol._Type & _Symbol::NUM_HEXADECIMAL) {
							_Off=2; _Bits=4;
						}
						//Octal
						else if (_Mysymbol._Type & _Symbol::NUM_OCTAL) {
							_Off=1; _Bits=3;
						}
						//Quaternary
						else if (_Mysymbol._Type & _Symbol::NUM_QUATERNARY) {
							_Off=2; _Bits=2;
						}
						//Binary
						else if (_Mysymbol._Type & _Symbol::NUM_BINARY) {
							_Off=2; _Bits=1;
						}
					
					_Mysymbol._Value=to_decimal(_Data.substr(_Off),
						_Bits,_Negate);
					
						//2nd complement, add sign only
						if (_Negate) {
						
						_Mysymbol._Data="-"+_Mysymbol._Data;
						_Negate=false;
						}
					}
				}
				//Ascii character
				else if (_Mysymbol._Type & _Symbol::ASCII) {
					_Mysymbol._Value=(unsigned char)_Mysymbol._Data[1];
				}
				
				//Negate
				if (_Negate) negate(_Mysymbol);
				
				//Invalid value
				if (nan(_Mysymbol._Value) || infinity(_Mysymbol._Value)) {
				
				log("'"+_Mysymbol._Data+"' is not a valid value");
				return false;
				}
			}
		}
		
		//Remove redundant parentheses
		else if (_Mysymbol._Type & _Symbol::PARENTHESIS) {
		
			//Left
			if (_Mysymbol._Type & _Symbol::PAR_LEFT) {
			
			++_Level;
			
			_Symbol *_Prev_symbol=(_Iter!=_Myexpression.begin()?
				&*(--_Iter):0);
			
				if (_Prev_symbol) ++_Iter;
				
				//Add only non-function parentheses to stack
				if (!_Prev_symbol ||
					(_Prev_symbol->_Type & _Symbol::FUNCTION)==0) {
				
				//Initially redundant?
				bool _Redundant=(!_Prev_symbol ||
					_Prev_symbol->_Type==_Mysymbol._Type ||
					_Prev_symbol->_Type & _Symbol::DELIMITER ||
						(_Prev_symbol->_Type & _Symbol::OPERATOR &&
						 _Prev_symbol->_Type & _Symbol::OP_TERNARY));
				
				_Parentheses.push(std::pair<unsigned int,std::pair<
					_Expression::iterator,bool>>(_Level,std::pair<
						_Expression::iterator,bool>(_Iter,_Redundant)));
				}
			}
			//Right
			else {
			
			--_Level;
			
				//Correct parentheses pair
				if (!_Parentheses.empty() &&
					_Parentheses.top().first==_Level+1) {
				
				_Symbol *_Next_symbol=(++_Iter!=_End?&*_Iter:0);
				--_Iter;
				
				bool _Redundant=_Parentheses.top().second.second;
				
					//Special case, always not redundant
					if (_Redundant && _Next_symbol &&
						_Next_symbol->_Type!=_Mysymbol._Type &&
						(_Next_symbol->_Type & _Symbol::DELIMITER)==0 &&
							((_Next_symbol->_Type & _Symbol::OPERATOR)==0 ||
							 (_Next_symbol->_Type & _Symbol::OP_TERNARY)==0))
								_Redundant=false;
					
					//Special case (x)
					if (!_Redundant) {
					
					_Symbol *_Sym=&*(--(--_Iter)); //Safe
					++(++_Iter);
					
						if (_Sym->_Type & _Symbol::PARENTHESIS &&
							_Sym->_Type & _Symbol::PAR_LEFT)
								_Redundant=true;
					}
					
					//Redundant parentheses pair
					if (_Redundant) {
					
					bool _Negate=false;
					
						//Check for negative sign before parentheses pair
						if (_Parentheses.top().second.first!=
							_Myexpression.begin()) {
						
						_Symbol *_Sym=&*(--_Parentheses.top().second.first);
						++_Parentheses.top().second.first;
						
							if (_Sym->_Type & _Symbol::OPERATOR &&
								_Sym->_Type & _Symbol::OP_UNARY &&
								_Sym->_Type & _Symbol::OP_NEGATIVE)
									_Negate=true;
						}
						
						if (_Negate) {
						
						_Myexpression.erase(_Iter); //Erase right
						_Iter=_Myexpression.erase(_Parentheses.top().
							second.first); //Erase left, rewind
						}
						else {
						
						_Iter=_Myexpression.erase(_Iter); //Erase right
						_Myexpression.erase(_Parentheses.top().
							second.first); //Erase left
						}
					}
				
				_Parentheses.pop();
				
					if (_Redundant) continue;
				}
			}
		}
	
	++_Iter;
	}

return true;
}

//Step 4 (calculate)

bool math_solver::calculate(_Expression &_Myexpression,result &_Result) {

//Create range root
_Range _Range_root;
_Range_root._Begin=_Myexpression.begin();
_Range_root._End=_Myexpression.end();

std::stack<_Range*> _Current_ranges;
_Current_ranges.push(&_Range_root);

std::stack<unsigned int> _Parentheses_level;

	//Create nested ranges
	for (_Expression::iterator _Iter=_Myexpression.begin(),
		_End=_Myexpression.end();_Iter!=_End;++_Iter) {
	
	_Symbol &_Mysymbol=(*_Iter); //Increase speed
	
		//Conditionals
		if (_Mysymbol._Type & _Symbol::OPERATOR) {
		
			if (_Mysymbol._Type & _Symbol::OP_TERNARY) {
			
				//If
				if (_Mysymbol._Type & _Symbol::OP_IF) {
				
				//Create new conditional range
				_Conditional *_Myconditional=new _Conditional;
				_Myconditional->_Type=_Current_ranges.top()->_Type;
				_Myconditional->_Begin=_Current_ranges.top()->_Begin;
				_Myconditional->_End=_Iter;
				
				//Transfer parent ranges to condition range
				_Myconditional->_Myranges=_Current_ranges.top()->_Myranges;
				_Current_ranges.top()->_Myranges.clear();
				
					//Replace parentheses/delimiter parent range
					if (_Myconditional->_Type & _Symbol::PARENTHESIS ||
						_Myconditional->_Type & _Symbol::DELIMITER) {
					
						//Change childs parent to new conditional range
						for (_Ranges::const_iterator _Riter=_Myconditional->
							_Myranges.begin(),_Rend=_Myconditional->
								_Myranges.end();_Riter!=_Rend;++_Riter)
									(*_Riter)->_Parent=_Myconditional;
					
					delete _Current_ranges.top();
					_Current_ranges.pop();
					_Current_ranges.top()->_Myranges.pop_back();
					}
				
				_Myconditional->_Parent=_Current_ranges.top();
				_Myconditional->_If_range._Parent=_Myconditional->_Parent;
				_Myconditional->_Else_range._Parent=_Myconditional->_Parent;
				
				_Myconditional->_Level=_Current_ranges.top()->_Level+1;
				_Myconditional->_If_range._Level=_Myconditional->_Level;
				_Myconditional->_Else_range._Level=_Myconditional->_Level;
				
				//Add conditional to parent ranges
				_Current_ranges.top()->_Myranges.push_back(_Myconditional);
				
				//Set if range as current parent
				_Myconditional->_If_range._Type=_Mysymbol._Type;
				_Myconditional->_If_range._Begin=_Iter;
				++_Myconditional->_If_range._Begin;
				_Current_ranges.push(&_Myconditional->_If_range);
				}
				//Else
				else if (_Mysymbol._Type & _Symbol::OP_ELSE) {
				
					//Close all ranges above if
					for (_Range *_Myrange=_Current_ranges.top();
						(_Myrange->_Type & _Symbol::OPERATOR)==0 ||
						(_Myrange->_Type & _Symbol::OP_IF)==0;
							_Myrange=_Current_ranges.top()) {
					
					_Myrange->_End=_Iter;
					_Current_ranges.pop();
					}
				
				//Close if range
				_Current_ranges.top()->_End=_Iter;
				_Current_ranges.pop();
				
				//Retrieve conditional
				_Conditional *_Myconditional=static_cast<_Conditional*>(
					_Current_ranges.top()->_Myranges.back());
				
				//Set else range as current parent
				_Myconditional->_Else_range._Type=_Mysymbol._Type;
				_Myconditional->_Else_range._Begin=_Iter;
				++_Myconditional->_Else_range._Begin;
				_Current_ranges.push(&_Myconditional->_Else_range);
				}
			}
		}
		
		//Parentheses
		else if (_Mysymbol._Type & _Symbol::PARENTHESIS) {
		
			//Left
			if (_Mysymbol._Type & _Symbol::PAR_LEFT) {
			
			//Create new parentheses range
			_Range *_Myrange=new _Range;
			_Myrange->_Type=_Mysymbol._Type;
			_Myrange->_Level=_Current_ranges.top()->_Level+1;
			_Myrange->_Parent=_Current_ranges.top();
			_Myrange->_Begin=_Iter;
			++_Myrange->_Begin;
			
			//Set parentheses range as current parent
			_Current_ranges.top()->_Myranges.push_back(_Myrange);
			_Current_ranges.push(_Myrange);
			
			_Parentheses_level.push(_Myrange->_Level);
			}
			//Right
			else {
			
			_Range *_Myrange;
			
				//Close all ranges above left parenthesis
				do {
				
				_Myrange=_Current_ranges.top();
				_Myrange->_End=_Iter;
				_Current_ranges.pop();
				} while (_Myrange->_Level>_Parentheses_level.top());
			
			_Parentheses_level.pop();
			}
		}
		
		//Delimiters
		else if (_Mysymbol._Type & _Symbol::DELIMITER) {
		
		_Range *_Myrange;
		
			//Close all ranges above left parenthesis
			do {
			
			_Myrange=_Current_ranges.top();
			_Myrange->_End=_Iter;
			_Current_ranges.pop();
			} while (_Myrange->_Level>_Parentheses_level.top());
		
		//Create delimiter range
		_Myrange=new _Range;
		_Myrange->_Type=_Mysymbol._Type;
		_Myrange->_Level=_Current_ranges.top()->_Level+1;
		_Myrange->_Parent=_Current_ranges.top();
		_Myrange->_Begin=_Iter;
		++_Myrange->_Begin;
		
		_Current_ranges.top()->_Myranges.push_back(_Myrange);
		_Current_ranges.push(_Myrange);
		}
	}
	
	//Special case, close else conditions
	while (_Current_ranges.size()>1) {
	
	_Current_ranges.top()->_End=_Range_root._End;
	_Current_ranges.pop();
	}

_Current_ranges.pop();

_Ranges _Myranges;

	//Sort ranges by traversing range root
	if (!_Range_root._Myranges.empty()) {
	
	//Create and use a stack for iterative traversal
	std::stack<std::pair<_Range*,_Ranges::iterator>> _Stack;
	_Stack.push(std::pair<_Range*,_Ranges::iterator>(
		&_Range_root,_Range_root._Myranges.begin()));
	
	std::stack<std::pair<_Conditional*,bool>> _Current_conditionals;
	
	_Ranges::iterator _Iter=_Myranges.begin();
	_Ranges::iterator _End=_Myranges.end();
	
	_Ranges::iterator _Insert_begin=_Iter;
	_Ranges::iterator _Insert_end=_End;
	
		do {
		
			//Prepare if/else range of current conditional
			if (!_Current_conditionals.empty()) {
			
			_Range *_Myrange=(_Current_conditionals.top().second?
				&_Current_conditionals.top().first->_Else_range:
				&_Current_conditionals.top().first->_If_range);
			
			_Stack.push(std::pair<_Range*,_Ranges::iterator>(
				_Myrange,_Myrange->_Myranges.begin()));
			_Insert_end=_Insert_begin=++_Iter;
			}
			
			//Traverse given range
			while (!_Stack.empty()) {
			
			_Range *_Myrange=_Stack.top().first; //Increase speed
			
				//Add new range to stack
				if (_Stack.top().second!=_Myrange->_Myranges.end()) {
				
				_Myrange=(*_Stack.top().second++);
				_Stack.push(std::pair<_Range*,_Ranges::iterator>(
					_Myrange,_Myrange->_Myranges.begin()));
				continue;
				}
			
			_Ranges::iterator _Insert_iter=_Insert_begin;
			
				//Find correct iterator (sorted by level desc)
				while (_Insert_iter!=_Insert_end &&
					(*_Insert_iter)->_Level>=_Myrange->_Level)
						++_Insert_iter;
				
				//Add range to range list
				if (_Insert_iter==_Insert_begin) _Iter=_Insert_begin=
					_Myranges.insert(_Insert_iter,_Myrange);
				else _Myranges.insert(_Insert_iter,_Myrange);
			
			_Stack.pop();
			}
			
			//Check current conditional
			if (!_Current_conditionals.empty()) {
			
				//Else-range of conditional is traversed
				//Close conditional
				if (_Current_conditionals.top().second) {
				
				_Current_conditionals.pop();
				
					if (_Current_conditionals.empty())
						_Insert_end=_End;
					else {
					
					//Find if range iterator for current conditional
					_Range *_Myrange=&_Current_conditionals.top().
						first->_If_range;
					
						while ((*_Insert_end)!=_Myrange)
							++_Insert_end;
					
					++_Insert_end;
					}
				}
				//If-range of conditional is traversed
				//Traverse else-range
				else _Current_conditionals.top().second=true;
			}
			
			//Find conditionals in range [_Iter,_Insert_end)
			for (;_Iter!=_Insert_end;++_Iter) {
			
			_Conditional *_Myconditional=dynamic_cast<
				_Conditional*>(*_Iter);
			
				if (_Myconditional) {
				
				_Current_conditionals.push(std::pair<
					_Conditional*,bool>(_Myconditional,false));
				break;
				}
			}
			
			//Special case, continue with current conditional
			if (!_Current_conditionals.empty() &&
				_Iter==_Insert_end) --_Iter;
		
		} while (_Iter!=_End);
	}
	
	//Add range root to list
	if (_Myranges.empty())
		_Myranges.push_back(&_Range_root);

std::list<_Expression::iterator*> _Iterators;

	for (_Ranges::iterator _Iter=_Myranges.begin(),
		_End=_Myranges.end();_Iter!=_End;++_Iter) {
	
	_Iterators.push_back(&(*_Iter)->_Begin);
	
		if ((*_Iter)->_End!=_Myexpression.end())
			_Iterators.push_back(&(*_Iter)->_End);
	}
	
	if (_Result._Generate_solution) _Result._Solution.clear();

_Range *_Myrange;

	//Calculate expression
	for (_Ranges::iterator _Iter=_Myranges.begin(),
		_End=_Myranges.end();_Iter!=_End;++_Iter) {
	
	_Myrange=(*_Iter); //Increase speed
	
		//Calculate inside range [begin,end)
		for (unsigned int _Order=0;++_Myrange->_Begin!=
			_Myrange->_End && _Order<=10;++_Order) {
		
			for (_Expression::iterator _Eiter=--_Myrange->_Begin,
				_Eend=_Myrange->_End;_Eiter!=_Eend;) {
			
			_Symbol *_Mysymbol=&(*_Eiter); //Increase speed
			
				//Nothing to do
				if ((_Mysymbol->_Type & _Symbol::OPERATOR)==0 ||
					(_Order==0 && (_Mysymbol->_Type & _Symbol::OP_UNARY)==0)) {
				
				++_Eiter;
				continue;
				}
			
			bool _Found=false;
			
				switch (_Order) {
				
					//Unary operators (right-to-left)
					//Positive/negative/not/complement
					case 0: {
					
					_Found=(_Mysymbol->_Type & _Symbol::OP_POSITIVE ||
							_Mysymbol->_Type & _Symbol::OP_NEGATIVE ||
							_Mysymbol->_Type & _Symbol::OP_NOT ||
							_Mysymbol->_Type & _Symbol::OP_COMPLEMENT);
					
						if (_Found) {
						
							//Right-most unary first
							while ((*++_Eiter)._Type & _Symbol::OPERATOR)
								_Mysymbol=&(*_Eiter); //Update
							
							//Positive
							if (_Mysymbol->_Type & _Symbol::OP_POSITIVE) {
								(*_Eiter)._Value=(*_Eiter)._Value;
							}
							//Negative
							else if (_Mysymbol->_Type & _Symbol::OP_NEGATIVE) {
								(*_Eiter)._Value=-(*_Eiter)._Value;
							}
							//Not
							else if (_Mysymbol->_Type & _Symbol::OP_NOT) {
								(*_Eiter)._Value=!(*_Eiter)._Value;
							}
							//Complement
							else if (_Mysymbol->_Type & _Symbol::OP_COMPLEMENT) {
								(*_Eiter)._Value=real(~(int64)(*_Eiter)._Value);
							}
						}
					
					break;
					}
					
					//Binary operators (left-to-right)
					//Power
					case 1: {
					
					_Found=(_Mysymbol->_Type & _Symbol::OP_POWER)!=0;
					
						if (_Found) {
						
						_Expression::const_iterator _Operand=_Eiter;
						++_Operand; //Right-operand
						
							//Power
							if (_Mysymbol->_Type & _Symbol::OP_POWER) {
								(*_Eiter)._Value=pow((*--_Eiter)._Value,
									(*_Operand)._Value);
							}
						}
					
					break;
					}
					
					//Multiply/divide/modulo
					case 2: {
					
					_Found=(_Mysymbol->_Type & _Symbol::OP_MULTIPLY ||
							_Mysymbol->_Type & _Symbol::OP_DIVIDE ||
							_Mysymbol->_Type & _Symbol::OP_MODULO);
					
						if (_Found) {
						
						_Expression::const_iterator _Operand=_Eiter;
						++_Operand; //Right-operand
						
							//Multiply
							if (_Mysymbol->_Type & _Symbol::OP_MULTIPLY) {
								(*_Eiter)._Value=(*--_Eiter)._Value*
									(*_Operand)._Value;
							}
							//Divide
							else if (_Mysymbol->_Type & _Symbol::OP_DIVIDE) {
								(*_Eiter)._Value=(*--_Eiter)._Value/
									(*_Operand)._Value;
							}
							//Modulo
							else if (_Mysymbol->_Type & _Symbol::OP_MODULO) {
								(*_Eiter)._Value=fmod((*--_Eiter)._Value,
									(*_Operand)._Value);
							}
						}
					
					break;
					}
					
					//Add/subtract
					case 3: {
					
					_Found=(_Mysymbol->_Type & _Symbol::OP_ADD ||
							_Mysymbol->_Type & _Symbol::OP_SUBTRACT);
					
						if (_Found) {
						
						_Expression::const_iterator _Operand=_Eiter;
						++_Operand; //Right-operand
						
							//Add
							if (_Mysymbol->_Type & _Symbol::OP_ADD) {
								(*_Eiter)._Value=(*--_Eiter)._Value+
									(*_Operand)._Value;
							}
							//Subtract
							else if (_Mysymbol->_Type & _Symbol::OP_SUBTRACT) {
								(*_Eiter)._Value=(*--_Eiter)._Value-
									(*_Operand)._Value;
							}
						}
					
					break;
					}
					
					//Shift-left/shift-right
					case 4: {
					
					_Found=(_Mysymbol->_Type & _Symbol::OP_SHIFT_LEFT ||
							_Mysymbol->_Type & _Symbol::OP_SHIFT_RIGHT);
					
						if (_Found) {
						
						_Expression::const_iterator _Operand=_Eiter;
						++_Operand; //Right-operand
						
							//Shift-left
							if (_Mysymbol->_Type & _Symbol::OP_SHIFT_LEFT) {
								(*_Eiter)._Value=real((int64)(*--_Eiter)._Value<<
									(int64)(*_Operand)._Value);
							}
							//Shift-right
							else if (_Mysymbol->_Type & _Symbol::OP_SHIFT_RIGHT) {
								(*_Eiter)._Value=real((int64)(*--_Eiter)._Value>>
									(int64)(*_Operand)._Value);
							}
						}
					
					break;
					}
					
					//Greater/less/greater-equal/less-equal
					case 5: {
					
					_Found=(_Mysymbol->_Type & _Symbol::OP_GREATER ||
							_Mysymbol->_Type & _Symbol::OP_LESS ||
							_Mysymbol->_Type & _Symbol::OP_GREATER_EQUAL ||
							_Mysymbol->_Type & _Symbol::OP_LESS_EQUAL);
					
						if (_Found) {
						
						_Expression::const_iterator _Operand=_Eiter;
						++_Operand; //Right-operand
						
							//Greater
							if (_Mysymbol->_Type & _Symbol::OP_GREATER) {
								(*_Eiter)._Value=(*--_Eiter)._Value>
									(*_Operand)._Value;
							}
							//Less
							else if (_Mysymbol->_Type & _Symbol::OP_LESS) {
								(*_Eiter)._Value=(*--_Eiter)._Value<
									(*_Operand)._Value;
							}
							//Greater-equal
							else if (_Mysymbol->_Type & _Symbol::OP_GREATER_EQUAL) {
								(*_Eiter)._Value=(*--_Eiter)._Value>=
									(*_Operand)._Value;
							}
							//Less-equal
							else if (_Mysymbol->_Type & _Symbol::OP_LESS_EQUAL) {
								(*_Eiter)._Value=(*--_Eiter)._Value<=
									(*_Operand)._Value;
							}
						}
					
					break;
					}
					
					//Equal/different
					case 6: {
					
					_Found=(_Mysymbol->_Type & _Symbol::OP_EQUAL ||
							_Mysymbol->_Type & _Symbol::OP_DIFFERENT);
					
						if (_Found) {
						
						_Expression::const_iterator _Operand=_Eiter;
						++_Operand; //Right-operand
						
							//Equal
							if (_Mysymbol->_Type & _Symbol::OP_EQUAL) {
								(*_Eiter)._Value=(*--_Eiter)._Value==
									(*_Operand)._Value;
							}
							//Different
							else if (_Mysymbol->_Type & _Symbol::OP_DIFFERENT) {
								(*_Eiter)._Value=(*--_Eiter)._Value!=
									(*_Operand)._Value;
							}
						}
					
					break;
					}
					
					//Bitwise-and
					case 7: {
					
					_Found=(_Mysymbol->_Type & _Symbol::OP_BIT_AND)!=0;
					
						if (_Found) {
						
						_Expression::const_iterator _Operand=_Eiter;
						++_Operand; //Right-operand
						
							//Bitwise-and
							if (_Mysymbol->_Type & _Symbol::OP_BIT_AND) {
								(*_Eiter)._Value=real((int64)(*--_Eiter)._Value &
									(int64)(*_Operand)._Value);
							}
						}
					
					break;
					}
					
					//Bitwise-or
					case 8: {
					
					_Found=(_Mysymbol->_Type & _Symbol::OP_BIT_OR)!=0;
					
						if (_Found) {
						
						_Expression::const_iterator _Operand=_Eiter;
						++_Operand; //Right-operand
						
							//Bitwise-or
							if (_Mysymbol->_Type & _Symbol::OP_BIT_OR) {
								(*_Eiter)._Value=real((int64)(*--_Eiter)._Value |
									(int64)(*_Operand)._Value);
							}
						}
					
					break;
					}
					
					//And
					case 9: {
					
					_Found=(_Mysymbol->_Type & _Symbol::OP_AND)!=0;
					
						if (_Found) {
						
						_Expression::const_iterator _Operand=_Eiter;
						++_Operand; //Right-operand
						
							//And
							if (_Mysymbol->_Type & _Symbol::OP_AND) {
								(*_Eiter)._Value=(*--_Eiter)._Value &&
									(*_Operand)._Value;
							}
						}
					
					break;
					}
					
					//Or
					case 10: {
					
					_Found=(_Mysymbol->_Type & _Symbol::OP_OR)!=0;
					
						if (_Found) {
						
						_Expression::const_iterator _Operand=_Eiter;
						++_Operand; //Right-operand
						
							//Or
							if (_Mysymbol->_Type & _Symbol::OP_OR) {
								(*_Eiter)._Value=(*--_Eiter)._Value ||
									(*_Operand)._Value;
							}
						}
					
					break;
					}
				}
				
				if (_Found) {
				
				real _Value=(*_Eiter)._Value;
				
					//Invalid operation
					if (nan(_Value) || infinity(_Value)) {
					
					std::string _Operation;
					
						//Unary operation
						if (_Mysymbol->_Type & _Symbol::OP_UNARY) {
						
						_Operation=(*_Eiter)._Data;
						_Operation=(*--_Eiter)._Data+_Operation;
						}
						//Binary operation
						else if (_Mysymbol->_Type & _Symbol::OP_BINARY) {
						
						_Operation=(*_Eiter)._Data;
						_Operation+=(*++_Eiter)._Data;
						_Operation+=(*++_Eiter)._Data;
						}
					
					log("Invalid operation '"+_Operation+"'");
					return false;
					}
					
					//Write expression to solution
					if (_Result._Generate_solution) {
					
					_Expression::iterator _First=_Eiter;
					_Expression::iterator _Last=_Eiter;
					
						if (_Mysymbol->_Type & _Symbol::OP_UNARY)
							--_First;
						else if (_Mysymbol->_Type & _Symbol::OP_BINARY)
							++(++_Last);
					
					write_solution(_Myexpression,*_Mysymbol,_First,_Last,
						_Result._Solution);
					
					//Update expression visually
					std::stringstream _Stream;
					_Stream<<_Value;
					(*_Eiter)._Data=_Stream.str();
					}
					
					//<operator><number>
					//             ^
					if (_Mysymbol->_Type & _Symbol::OP_UNARY) {
					
					--_Eiter;
					
						//Can cause invalidation
						if (_Eiter==_Myrange->_Begin)
							erase_elements(_Myexpression,_Eiter,
								_Eiter,_Iterators);
						else {
						
						_Eiter=_Myexpression.erase(_Eiter); //Safe
						--_Eiter; //Look for another unary
						}
					}
					//<number><operator><number>
					//   ^
					else if (_Mysymbol->_Type & _Symbol::OP_BINARY) {
					
					++_Eiter;
					erase_elements(_Myexpression,_Eiter,
						++(++_Expression::iterator(_Eiter)),
							_Iterators);
					}
				}
				else {
				
				++_Eiter;
				
					if (_Mysymbol->_Type & _Symbol::OP_BINARY)
						++_Eiter; //Skip to next binary operator
				}
			}
		}
	
	//Clean up calculated range
	erase_iterator(&_Myrange->_Begin,_Iterators);
	erase_iterator(&_Myrange->_End,_Iterators);
	
		//Nothing to do
		if (_Myexpression.size()<3) continue;
	
	_Conditional *_Myconditional=dynamic_cast<_Conditional*>(_Myrange);
	--_Myrange->_Begin;
	
		//Conditional
		if (_Myconditional) {
		
		bool _Condition=!equal((*_Myrange->_Begin)._Value,0.0);
		
			//Write expression to solution
			if (_Result._Generate_solution) {
			
			write_solution(_Myexpression,_Condition?
				*_Myrange->_End:*_Myconditional->_If_range._End,
					_Myrange->_Begin,--_Expression::iterator(
						_Myconditional->_Else_range._End),
							_Result._Solution);
			}
			
			//Condition true
			if (_Condition) {
			
			//Erase condition
			erase_elements(_Myexpression,_Myrange->_Begin,
				++_Myrange->_End,_Iterators);
			
			//Erase else
			erase_elements(_Myexpression,--_Myconditional->
				_Else_range._Begin,_Myconditional->_Else_range._End,
					_Iterators);
			erase_iterator(&_Myconditional->_Else_range._Begin,_Iterators);
			erase_iterator(&_Myconditional->_Else_range._End,_Iterators);
			
			//Erase all ranges dependent by a false condition
			_Ranges::iterator _Else_begin=_Iter;
			
				while ((*++_Else_begin)!=&_Myconditional->_If_range);
			
			_Ranges::iterator _Else_end=_Else_begin;
			
				while ((*++_Else_end)!=&_Myconditional->_Else_range);
			
			_Myranges.erase(++_Else_begin,++_Else_end);
			}
			//Condition false
			else {
			
			//Erase condition and if
			erase_elements(_Myexpression,_Myrange->_Begin,
				++_Myconditional->_If_range._End,_Iterators);
			erase_iterator(&_Myconditional->_If_range._End,_Iterators);
			
				//Set range iterator to begin at else range
				while ((*++_Iter)!=&_Myconditional->_If_range);
			}
		}
		
		//Parentheses
		else if (_Myrange->_End!=_Myexpression.end() &&
			(*_Myrange->_End)._Type & _Symbol::PARENTHESIS) {
		
			//Find corresponding parenthesis
			while ((_Myrange->_Begin->_Type & _Symbol::PARENTHESIS)==0)
				--_Myrange->_Begin;
			
			if (_Myrange->_Begin!=_Myexpression.begin()) {
			
				//Function
				if ((*--_Myrange->_Begin)._Type & _Symbol::FUNCTION) {
				
				function *_Function=(function*)(size_t)
					(*_Myrange->_Begin)._Value;
				function::arg _Arg;
				
					//Populate argument list
					for (_Expression::const_iterator _Aiter=
						++_Myrange->_Begin;_Aiter!=_Myrange->_End;
							++_Aiter) _Arg.push_back(
								(*++_Aiter)._Value);
				
				//Call function and keep result
				(*++_Myrange->_Begin)._Value=(*_Function)(_Arg);
				--(--_Myrange->_Begin);
				
					//Write expression to solution
					if (_Result._Generate_solution) {
					
					write_solution(_Myexpression,*_Myrange->_Begin,
						_Myrange->_Begin,_Myrange->_End,_Result._Solution);
					
					_Expression::iterator _Viter=_Myrange->_Begin;
					++(++_Viter);
					
					//Update expression visually
					std::stringstream _Stream;
					_Stream<<(*_Viter)._Value;
					(*_Viter)._Data=_Stream.str();
					}
				
				//Erase function
				erase_elements(_Myexpression,_Myrange->_Begin,
					_Myrange->_Begin,_Iterators);
				
				//Erase parameters
				erase_elements(_Myexpression,++(++_Expression::
					iterator(_Myrange->_Begin)),_Myrange->_End,
						_Iterators);
				}
				else ++_Myrange->_Begin;
			}
		
		//Erase left parenthesis
		erase_elements(_Myexpression,_Myrange->_Begin,
			_Myrange->_Begin,_Iterators);
		
		//Erase right parenthesis
		erase_elements(_Myexpression,_Myrange->_End,
			_Myrange->_End,_Iterators);
		}
	}
	
	//Update answer for the latest expression
	if (!_Myexpression.empty()) {
	
		//Enter shared section
		if (mutex_lock()) {
		
		_Answer=_Myexpression.front()._Value;
		_Result._Answer=_Answer;
		mutex_unlock(); //Exit shared section
		}
	}

return true;
}

void math_solver::erase_elements(_Expression &_Myexpression,
	_Expression::iterator &_First,_Expression::iterator &_Last,
		std::list<_Expression::iterator*> &_Iterators) {

	//Nothing to do
	if (_First==_Last && &_First!=&_Last) return;
	
	//Remove iterators in range (_First,_Last)
	if (_First!=_Last) {
	
		for (_Expression::iterator _Iter=++_Expression::
			iterator(_First);_Iter!=_Last;++_Iter)
				erase_iterator(_Iter,_Iterators);
	}

std::list<_Expression::iterator*> _Invalid_iterators;
_Expression::iterator *_Iterator;

	for (std::list<_Expression::iterator*>::const_iterator _Iter=
		_Iterators.begin(),_End=_Iterators.end();_Iter!=_End;
			++_Iter) {
	
	_Iterator=(*_Iter); //Increase speed
	
		if (*_Iterator==_First && _Iterator!=&_First)
			_Invalid_iterators.push_back(_Iterator);
	}
	
	//Erase one element
	if (_First==_Last) _First=_Myexpression.erase(_First);
	//Erase multiple elements
	else _First=_Myexpression.erase(_First,_Last);
	
	//Update invalid iterators
	if (!_Invalid_iterators.empty()) {
	
		for (std::list<_Expression::iterator*>::const_iterator _Iter=
			_Invalid_iterators.begin(),_End=_Invalid_iterators.end();
				_Iter!=_End;++_Iter) *(*_Iter)=_First;
	}
}

void math_solver::erase_iterator(_Expression::iterator &_Iterator,
	std::list<_Expression::iterator*> &_Iterators) {

	for (std::list<_Expression::iterator*>::iterator _Iter=
		_Iterators.begin(),_End=_Iterators.end();_Iter!=_End;) {
	
		if (*(*_Iter)==_Iterator)
			_Iter=_Iterators.erase(_Iter);
		else ++_Iter;
	}
}

void math_solver::erase_iterator(_Expression::iterator *_Iterator,
	std::list<_Expression::iterator*> &_Iterators) {

	for (std::list<_Expression::iterator*>::iterator _Iter=
		_Iterators.begin(),_End=_Iterators.end();_Iter!=_End;
			++_Iter) {
	
		if ((*_Iter)==_Iterator) {
		
		_Iterators.erase(_Iter);
		return;
		}
	}
}

void math_solver::write_solution(const _Expression &_Myexpression,
	const _Symbol &_Mysymbol,const _Expression::iterator &_First,
		const _Expression::iterator &_Last,steps &_Solution) {

std::string _Operation;

	//Operator
	if (_Mysymbol._Type & _Symbol::OPERATOR) {
	
		//Unary
		if (_Mysymbol._Type & _Symbol::OP_UNARY) {
		
			if (_Mysymbol._Type & _Symbol::OP_POSITIVE)
				_Operation="";
			else if (_Mysymbol._Type & _Symbol::OP_NEGATIVE)
				_Operation="";
			
			else if (_Mysymbol._Type & _Symbol::OP_NOT)
				_Operation="negate";
			else if (_Mysymbol._Type & _Symbol::OP_COMPLEMENT)
				_Operation="complement";
		}
		
		//Binary
		else if (_Mysymbol._Type & _Symbol::OP_BINARY) {
		
			if (_Mysymbol._Type & _Symbol::OP_ADD)
				_Operation="add";
			else if (_Mysymbol._Type & _Symbol::OP_SUBTRACT)
				_Operation="subtract";
			else if (_Mysymbol._Type & _Symbol::OP_MULTIPLY)
				_Operation="multiply";
			else if (_Mysymbol._Type & _Symbol::OP_DIVIDE)
				_Operation="divide";
			else if (_Mysymbol._Type & _Symbol::OP_MODULO)
				_Operation="modulo";
			else if (_Mysymbol._Type & _Symbol::OP_POWER)
				_Operation="power";
			
			else if (_Mysymbol._Type & _Symbol::OP_EQUAL)
				_Operation="equal";
			else if (_Mysymbol._Type & _Symbol::OP_DIFFERENT)
				_Operation="different";
			else if (_Mysymbol._Type & _Symbol::OP_GREATER)
				_Operation="greater";
			else if (_Mysymbol._Type & _Symbol::OP_LESS)
				_Operation="less";
			else if (_Mysymbol._Type & _Symbol::OP_GREATER_EQUAL)
				_Operation="greater-equal";
			else if (_Mysymbol._Type & _Symbol::OP_LESS_EQUAL)
				_Operation="less-equal";
			
			else if (_Mysymbol._Type & _Symbol::OP_AND)
				_Operation="and";
			else if (_Mysymbol._Type & _Symbol::OP_OR)
				_Operation="or";
			else if (_Mysymbol._Type & _Symbol::OP_BIT_AND)
				_Operation="bitwise and";
			else if (_Mysymbol._Type & _Symbol::OP_BIT_OR)
				_Operation="bitwise or";
			else if (_Mysymbol._Type & _Symbol::OP_SHIFT_LEFT)
				_Operation="left shift";
			else if (_Mysymbol._Type & _Symbol::OP_SHIFT_RIGHT)
				_Operation="right shift";
		}
		
		//Ternary
		else if (_Mysymbol._Type & _Symbol::OP_TERNARY) {
		
			if (_Mysymbol._Type & _Symbol::OP_IF)
				_Operation="true";
			else if (_Mysymbol._Type & _Symbol::OP_ELSE)
				_Operation="false";
		}
	}
	
	//Function
	else if (_Mysymbol._Type & _Symbol::FUNCTION)
		_Operation=((function*)(size_t)
			_Mysymbol._Value)->_Name;
	
	//Skip step
	if (_Operation.empty()) return;

std::stringstream _Header;
_Header<<"Step "<<_Solution.size()+1<<" ("<<_Operation<<"): ";

step _Step;
_Step.first=_Header.str();

	for (_Expression::const_iterator _Iter=_Myexpression.begin(),
		_End=_Myexpression.end();_Iter!=_End;++_Iter) {
	
		//Operation prefix
		if (_Iter==_First) _Step.second.
			first=_Step.first.size();
	
	_Step.first+=(*_Iter)._Data;
	
		//Operation suffix
		if (_Iter==_Last) _Step.second.
			second=_Step.first.size()-1;
	}

_Solution.push_back(_Step);
}

void math_solver::negate(_Symbol &_Mysymbol) {

_Mysymbol._Value=-_Mysymbol._Value; //Negate

	if (negative(_Mysymbol._Value) || _Mysymbol._Data[0]!='-')
		_Mysymbol._Data="-"+_Mysymbol._Data; //Add sign
	else _Mysymbol._Data.erase(0,1); //Remove sign
}

bool math_solver::equal(real _Value,real _Value2) {

real _Diff=_Value-_Value2;
return (_Diff<_Epsilon && _Diff>-_Epsilon);
}

bool math_solver::nan(real _Value) {
	//Temporary hack
	//Use std::isnan() when available
	return (_Value!=_Value);
}

bool math_solver::infinity(real _Value) {
	//Temporary hack
	//Use std::isinf() when available
	return (std::numeric_limits<real>::has_infinity &&
		fabs(_Value)==std::numeric_limits<real>::infinity());
}

bool math_solver::negative(real _Value) {

	//Temporary hack
	//Use std::signbit() when available
	union {
	
	real _Real;
	short _Short[4];
	int _Int[2];
	} _Union;

_Union._Real=_Value;
return (sizeof(int)==4?_Union._Int[1]<0:
	_Union._Short[3]<0);
}

//Input/output

math_solver::real math_solver::to_decimal(const std::string &_Value,
	unsigned int _Bits,bool _Negative) {

	//Nothing to do
	if (_Bits==0) return 0.0;
	
	if (_Bits==1) return from_binary(_Value,_Negative);

std::string _Result;
std::string _Binary;

	for (size_t i=0;i<_Value.size();++i) {
	
		//Skip decimal point
		if (_Value[i]=='.') {
		
		_Result+=_Value[i];
		continue;
		}
	
	unsigned char _C=std::toupper((unsigned char)_Value[i]);
	_Binary=to_binary((real)(_C>='A'?_C-'A'+10:_C-'0'));
	
		//Append zeroes to fill bit group
		if (i>0 && _Binary.size()<_Bits)
			_Result.append(_Bits-_Binary.size(),'0');
	
	_Result+=_Binary;
	}

return from_binary(_Result,_Negative);
}

std::string math_solver::from_decimal(real _Value,
	unsigned int _Bits,unsigned int _Precision) {

	//Nothing to do
	if (_Bits==0) return "0";

std::string _Binary=to_binary(_Value,_Precision);

	if (_Bits==1) return _Binary;

size_t _Size=_Binary.size();
size_t _Pos=_Binary.find('.');
_Pos=(_Pos!=std::string::npos?_Pos:_Size)%_Bits;

	if (_Pos>0) _Pos=_Bits-_Pos;

unsigned int _Sum=0;

	//Negative number
	if (_Value<0.0) {
	
		//Add bit values to fill bit group
		for (size_t i=0;i<_Pos;++i) _Sum+=(unsigned int)
			pow(2.0,(int)(_Bits-i-1));
	}

std::string _Result;

	for (size_t i=0;i<_Size;++i) {
	
		//Skip decimal point
		if (_Binary[i]=='.') {
		
		_Result+=_Binary[i];
		_Pos=_Sum=0; //Reset
		continue;
		}
		
		//Add bit value
		if (_Binary[i]=='1') _Sum+=(unsigned int)
			pow(2.0,(int)(_Bits-(_Pos%_Bits)-1));
		
		if (++_Pos%_Bits==0 || i+1==_Size) {
		
			if (_Sum<10) _Result+='0'+(unsigned char)_Sum;
			else _Result+='A'+(unsigned char)(_Sum-10);
		
		_Sum=0;
		}
	}

return _Result;
}

//Input/output (binary)

math_solver::real math_solver::from_binary(std::string _Binary,
	bool _Negative) {

	//Nothing to do
	if (_Binary.empty()) return 0.0;

size_t _Size=_Binary.size();
size_t _Pos=_Binary.find('.');
_Pos=(_Pos!=std::string::npos?_Pos:_Size);

	//Value given as 2nd complement
	if (_Negative) {
	
	size_t _Begin=0;
	
		//Complement
		for (size_t i=0;i<_Pos;++i) {
		
			if (i==_Begin) {
			
				//Ignore left-most zeroes
				if (_Binary[i]=='0') {
				
				++_Begin;
				continue;
				}
			}
		
		_Binary[i]=(_Binary[i]=='1'?'0':'1');
		}
		
		//2nd complement (add one bit)
		for (size_t i=_Pos;i>_Begin;--i) {
		
			if (_Binary[i-1]=='0') {
			
			_Binary[i-1]='1';
			break;
			}
		
		_Binary[i-1]='0';
		}
	}

real _Value=0.0;

	for (size_t i=0;i<_Size;++i) {
	
		//Skip decimal point
		if (i==_Pos) continue;
		
		if (_Binary[i]=='1') _Value+=pow((real)2.0,
			(int)(i<_Pos?_Pos-i-1:_Pos-i));
		
		//Check overflow
		if (infinity(_Value)) break;
	}

return (_Negative?-_Value:_Value);
}

std::string math_solver::to_binary(real _Value,
	unsigned int _Precision) {

	//Nothing to do
	if (equal(_Value,0.0)) return "0";

static const real _Max_value=pow((real)2.0,
	_Significand_precision)-1.0;

real _Abs_value=fabs(_Value);
bool _Conversion_limit=(_Abs_value>_Max_value);

uint64 _Natural_value=(_Conversion_limit?
	(uint64)_Max_value:(uint64)_Abs_value);
real _Mantissa_value=(_Conversion_limit?
	0.0:_Abs_value-_Natural_value);

//Convert to format: <natural number>.<mantissa>
std::string _Result;

	//Natural number
	do {
		if (_Natural_value%2) _Result+='1';
		else _Result+='0';
	
	} while (_Natural_value>>=1);
	
	//Negative number
	if (_Value<=-1.0) {
	
	size_t _Size=_Result.size();
	
		//Complement
		for (size_t i=0;i<_Size;++i)
			_Result[i]=(_Result[i]=='1'?'0':'1');
		
		//2nd complement (add one bit)
		for (size_t i=0;i<_Size;++i) {
		
			if (_Result[i]=='0') {
			
			_Result[i]='1';
			break;
			}
		
		_Result[i]='0';
		}
		
		//Add extra bit if necessary
		if (_Result[_Size-1]=='0') _Result+='1';
	}

std::reverse(_Result.begin(),_Result.end());

	//Mantissa
	if (_Mantissa_value>0.0) {
	
	_Result+='.';
	
		do {
			if ((_Mantissa_value*=2.0)>=1.0) {
			
			_Result+='1';
			_Mantissa_value-=floor(_Mantissa_value);
			}
			else _Result+='0';
		
		} while (_Mantissa_value>0.0 && --_Precision>0);
	}

return _Result;
}

//Public

math_solver::math_solver():_Answer(0.0) {
	_Threads_supported=init_pthread();
}

math_solver::math_solver(const math_solver &_Math_solver) {

_Threads_supported=init_pthread();
copy(_Math_solver);
}

math_solver::~math_solver() {

clear_processes();

clear_constants();
clear_variables();
clear_functions();

	//Destroy thread attribute
	if (_Thread_attribute) pthread_attr_destroy(&_Thread_attribute);
	
	//Destroy thread mutex
	if (_Thread_mutex) pthread_mutex_destroy(&_Thread_mutex);
}

math_solver &math_solver::operator=(const math_solver &_Math_solver) {

	//Worth doing
	if (this!=&_Math_solver) copy(_Math_solver);

return *this;
}

bool math_solver::solve(const std::string &_Input,result &_Result) {

_Result._Valid=false;

_Expression _Myexpression;
parse(_Input,_Myexpression);

	if (!validate(_Myexpression)) return false;
	if (!finalize(_Myexpression)) return false;
	if (!calculate(_Myexpression,_Result)) return false;

_Result._Valid=true;
return true;
}

bool math_solver::solve(const std::string &_Input,process *_Process) {

	//Threads not supported
	if (!_Threads_supported) return false;
	
	//Process not responding
	if (!_Process->stop()) return false;

_Process->_Input=_Input;
return init_process(_Process);
}

//Output

math_solver::real math_solver::answer() const {
	return _Answer;
}

void math_solver::error_log(string_list &_Error_log) const {
	_Error_log=this->_Error_log; //Make copy
}

std::string math_solver::hexadecimal(real _Value,
	unsigned int _Precision) {
	return from_decimal(_Value,4,_Precision);
}

std::string math_solver::octal(real _Value,
	unsigned int _Precision) {
	return from_decimal(_Value,3,_Precision);
}

std::string math_solver::quaternary(real _Value,
	unsigned int _Precision) {
	return from_decimal(_Value,2,_Precision);
}

std::string math_solver::binary(real _Value,
	unsigned int _Precision) {
	return from_decimal(_Value,1,_Precision);
}

std::string math_solver::scinotation(real _Value,
	bool _E_notation) {

real _Abs_value=fabs(_Value);
int _Exponent=0;

	if (_Abs_value>0.0) {
	
		if (_Abs_value>=1.0) {
		
			for (;_Abs_value>10.0;++_Exponent)
				_Abs_value/=10.0;
		}
		else {
		
			for (;_Abs_value<1.0;--_Exponent)
				_Abs_value*=10.0;
		}
	}

//Convert to format: <base><notation><exponent>
std::stringstream _Stream;

	if (_Value<0.0) _Stream<<"-";

_Stream<<_Abs_value;

	if (_E_notation) _Stream<<"E";
	else _Stream<<"*10^";

_Stream<<_Exponent;
return _Stream.str();
}

//Process

math_solver::process *math_solver::create_process() {

process *_Myproc=new process(this);
_Myprocesses.push_back(_Myproc);
return _Myproc;
}

math_solver::process *math_solver::create_process(const process &_Process) {

process *_Myproc=new process(this,_Process);
_Myprocesses.push_back(_Myproc);
return _Myproc;
}

void math_solver::remove_process(process *&_Process) {

	for (_Processes::iterator _Iter=_Myprocesses.begin(),
		_End=_Myprocesses.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Process) {
		
		delete _Process;
		_Process=0; //Set referenced pointer to null
		
		_Myprocesses.erase(_Iter);
		return;
		}
	}
}

void math_solver::clear_processes() {

	for (_Processes::const_iterator _Iter=_Myprocesses.begin(),
		_End=_Myprocesses.end();_Iter!=_End;++_Iter)
			delete (*_Iter);

_Myprocesses.clear();
}

math_solver::processes_iterator math_solver::processes_begin() const {
	return _Myprocesses.begin();
}

math_solver::processes_iterator math_solver::processes_end() const {
	return _Myprocesses.end();
}

//Constants

math_solver::constant *math_solver::create_constant(
	const std::string &_Name,real _Value) {

	//Constant already exist
	if (constant_pointer(_Name)) return 0;

constant *_Myconst=new constant(_Name,_Value);
_Myconstants.push_back(_Myconst);
return _Myconst;
}

math_solver::constant *math_solver::create_constant(
	const std::string &_Name,const constant &_Constant) {

	//Constant already exist
	if (constant_pointer(_Name)) return 0;

constant *_Myconst=new constant(_Name,_Constant);
_Myconstants.push_back(_Myconst);
return _Myconst;
}

void math_solver::remove_constant(constant *&_Constant) {

	for (_Constants::iterator _Iter=_Myconstants.begin(),
		_End=_Myconstants.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Constant) {
		
		delete _Constant;
		_Constant=0; //Set referenced pointer to null
		
		_Myconstants.erase(_Iter);
		return;
		}
	}
}

void math_solver::clear_constants() {

	for (_Constants::const_iterator _Iter=_Myconstants.begin(),
		_End=_Myconstants.end();_Iter!=_End;++_Iter)
			delete (*_Iter);

_Myconstants.clear();
}

math_solver::constants_iterator math_solver::constants_begin() const {
	return _Myconstants.begin();
}

math_solver::constants_iterator math_solver::constants_end() const {
	return _Myconstants.end();
}

math_solver::constant *math_solver::constant_pointer(
	const std::string &_Name) const {

	for (_Constants::const_iterator _Iter=_Myconstants.begin(),
		_End=_Myconstants.end();_Iter!=_End;++_Iter) {
	
		//Constant name exist, return pointer
		if ((*_Iter)->_Name==_Name) return (*_Iter);
	}

return 0;
}

//Variables

math_solver::variable *math_solver::create_variable(
	const std::string &_Name,const std::string &_Value) {

	//Variable already exist
	if (variable_pointer(_Name)) return 0;

variable *_Myvar=new variable(_Name,_Value);
_Myvariables.push_back(_Myvar);
return _Myvar;
}

math_solver::variable *math_solver::create_variable(
	const std::string &_Name,const variable &_Variable) {

	//Variable already exist
	if (variable_pointer(_Name)) return 0;

variable *_Myvar=new variable(_Name,_Variable);
_Myvariables.push_back(_Myvar);
return _Myvar;
}

void math_solver::remove_variable(variable *&_Variable) {

	for (_Variables::iterator _Iter=_Myvariables.begin(),
		_End=_Myvariables.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Variable) {
		
		delete _Variable;
		_Variable=0; //Set referenced pointer to null
		
		_Myvariables.erase(_Iter);
		return;
		}
	}
}

void math_solver::clear_variables() {

	for (_Variables::const_iterator _Iter=_Myvariables.begin(),
		_End=_Myvariables.end();_Iter!=_End;++_Iter)
			delete (*_Iter);

_Myvariables.clear();
}

math_solver::variables_iterator math_solver::variables_begin() const {
	return _Myvariables.begin();
}

math_solver::variables_iterator math_solver::variables_end() const {
	return _Myvariables.end();
}

math_solver::variable *math_solver::variable_pointer(
	const std::string &_Name) const {

	for (_Variables::const_iterator _Iter=_Myvariables.begin(),
		_End=_Myvariables.end();_Iter!=_End;++_Iter) {
	
		//Variable name exist, return pointer
		if ((*_Iter)->_Name==_Name) return (*_Iter);
	}

return 0;
}

//Functions

math_solver::function *math_solver::create_function(
	const std::string &_Name,function::ptr _Ptr,
		unsigned int _Parameters) {

	//Function already exist
	if (function_pointer(_Name)) return 0;

function *_Myfunc=new function(_Name,_Ptr,_Parameters);
_Myfunctions.push_back(_Myfunc);
return _Myfunc;
}

math_solver::function *math_solver::create_function(
	const std::string &_Name,const function &_Function) {

	//Function already exist
	if (function_pointer(_Name)) return 0;

function *_Myfunc=new function(_Name,_Function);
_Myfunctions.push_back(_Myfunc);
return _Myfunc;
}

void math_solver::remove_function(function *&_Function) {

	for (_Functions::iterator _Iter=_Myfunctions.begin(),
		_End=_Myfunctions.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Function) {
		
		delete _Function;
		_Function=0; //Set referenced pointer to null
		
		_Myfunctions.erase(_Iter);
		return;
		}
	}
}

void math_solver::clear_functions() {

	for (_Functions::const_iterator _Iter=_Myfunctions.begin(),
		_End=_Myfunctions.end();_Iter!=_End;++_Iter)
			delete (*_Iter);

_Myfunctions.clear();
}

math_solver::functions_iterator math_solver::functions_begin() const {
	return _Myfunctions.begin();
}

math_solver::functions_iterator math_solver::functions_end() const {
	return _Myfunctions.end();
}

math_solver::function *math_solver::function_pointer(
	const std::string &_Name) const {

	for (_Functions::const_iterator _Iter=_Myfunctions.begin(),
		_End=_Myfunctions.end();_Iter!=_End;++_Iter) {
	
		//Function name exist, return pointer
		if ((*_Iter)->_Name==_Name) return (*_Iter);
	}

return 0;
}

} //namespace lunar