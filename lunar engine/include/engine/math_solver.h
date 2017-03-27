/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: math_solver.h
---------------------------------------------------
*/

//Math Solver API
//Consult your license regarding permissions and restrictions.
//Copyright (c) 2005-2011 by Jan Ivar Goli.

#ifndef _MATH_SOLVER_
#define _MATH_SOLVER_

#include <string>
#include <vector>
#include <list>

#define HAVE_STRUCT_TIMESPEC
#include "pthread.h"

namespace lunar {

class math_solver {

public:

	//The highest floating point precision supported by the compiler.
	//On the x86 architecture, most compilers implement long double
	//as the 80-bit extended precision type supported by that hardware.
	//
	//An exception is Microsoft Visual C++ for x86,
	//which makes long double a synonym for double
	typedef long double real;
	
	//In computer science, a longeger is a data type that can represent
	//a whole number which may have a larger range, while taking more storage,
	//compared with a standard integer on the same machine.
	//
	//For c++03 longeger is only supported through language extension.
	//Long integer is natively supported in C++0x as long long.
	typedef long long int64;
	typedef unsigned long long uint64;
	
	typedef std::list<std::string> string_list;
	
	typedef std::pair<std::string,std::pair<size_t,size_t>> step;
	typedef std::list<step> steps;

private:

	struct _Symbol {
	
		friend math_solver;
	
	private:
	
		//---------------
		//Supported types
		//---------------
		
		static const unsigned int UNKNOWN=0x0;
		static const unsigned int NUMBER=0x1;
		static const unsigned int ASCII=0x2;
		static const unsigned int OPERATOR=0x4;
		static const unsigned int PARENTHESIS=0x8;
		static const unsigned int DELIMITER=0x10;
		static const unsigned int FUNCTION=0x20;
		
		//Number
		static const unsigned int NUM_HEXADECIMAL=0x40;
		static const unsigned int NUM_DECIMAL=0x80;
		static const unsigned int NUM_OCTAL=0x100;
		static const unsigned int NUM_QUATERNARY=0x200;
		static const unsigned int NUM_BINARY=0x400;
		
		//Operator
		static const unsigned int OP_UNARY=0x40;
		static const unsigned int OP_BINARY=0x80;
		static const unsigned int OP_TERNARY=0x100;
		
		//Operator - Unary
		static const unsigned int OP_POSITIVE=0x200;
		static const unsigned int OP_NEGATIVE=0x400;
		
		static const unsigned int OP_NOT=0x800;
		static const unsigned int OP_COMPLEMENT=0x1000;
		
		//Operator - Binary
		static const unsigned int OP_ADD=0x200;
		static const unsigned int OP_SUBTRACT=0x400;
		static const unsigned int OP_MULTIPLY=0x800;
		static const unsigned int OP_DIVIDE=0x1000;
		static const unsigned int OP_MODULO=0x2000;
		static const unsigned int OP_POWER=0x4000;
		
		static const unsigned int OP_EQUAL=0x8000;
		static const unsigned int OP_DIFFERENT=0x10000;
		static const unsigned int OP_GREATER=0x20000;
		static const unsigned int OP_LESS=0x40000;
		static const unsigned int OP_GREATER_EQUAL=0x80000;
		static const unsigned int OP_LESS_EQUAL=0x100000;
		
		static const unsigned int OP_AND=0x200000;
		static const unsigned int OP_OR=0x400000;
		static const unsigned int OP_BIT_AND=0x800000;
		static const unsigned int OP_BIT_OR=0x1000000;
		static const unsigned int OP_SHIFT_LEFT=0x2000000;
		static const unsigned int OP_SHIFT_RIGHT=0x4000000;
		
		//Operator - Ternary
		static const unsigned int OP_IF=0x200;
		static const unsigned int OP_ELSE=0x400;
		
		//Parenthesis
		static const unsigned int PAR_LEFT=0x40;
		static const unsigned int PAR_RIGHT=0x80;
		
		//---------------
		
		unsigned int _Type;
		std::string _Data;
		real _Value;
	
	public:
	
		//Constructor
		_Symbol();
		
		//Destructor
		~_Symbol();
	};
	
	typedef std::list<_Symbol> _Expression;
	
	struct _Range;
	typedef std::list<_Range*> _Ranges;
	
	struct _Range {
	
		friend math_solver;
	
	protected:
	
		//Protected constructor
		_Range();
		
		//Protected virtual destructor
		virtual ~_Range();
		
		unsigned int _Type;
		unsigned int _Level;
		_Range *_Parent;
		
		_Expression::iterator _Begin;
		_Expression::iterator _End;
		
		_Ranges _Myranges;
	};
	
	struct _Conditional:public _Range {
	
		friend math_solver;
	
	protected:
	
		//Protected constructor
		_Conditional();
		
		//Protected virtual destructor
		virtual ~_Conditional();
		
		_Range _If_range;
		_Range _Else_range;
	};

public:

	struct result {
	
		friend math_solver;
	
	private:
	
		real _Answer;
		steps _Solution;
		bool _Generate_solution;
		
		bool _Valid;
		
		void copy(const result &_Result);
	
	public:
	
		//Constructor
		result();
		
		//Copy constructor
		result(const result &_Result);
		
		//Destructor
		~result();
		
		result &operator=(const result &_Result);
		
		//Returns true if the result is valid
		operator bool() const;
		
		real answer() const;
		
		void solution(steps &_Solution) const;
		
		void generate_solution(bool _Generate_solution);
		bool generate_solution() const;
	};
	
	struct process {
	
		friend math_solver;
	
	private:
	
		math_solver *_Owner;
		pthread_t *_Thread;
		
		std::string _Input;
		_Expression _Myexpression;
		result _Result;
		
		bool _Working;
		
		void copy(const process &_Process);
	
	public:
	
		//Constructor
		process(math_solver *_Owner);
		
		//Copy constructor
		process(const process &_Process);
		
		//Alternative copy constructor
		process(math_solver *_Owner,const process &_Process);
		
		//Destructor
		~process();
		
		process &operator=(const process &_Process);
		
		bool stop();
		bool working() const;
		
		const std::string &input() const;
		result &output();
	};
	
	struct object {
	
		friend math_solver;
	
	protected:
	
		std::string _Name;
		std::string _Category;
		std::string _Description;
		
		//Protected constructor
		object(const std::string &_Name);
		
		//Protected copy constructor
		object(const object &_Object);
		
		//Protected alternative copy constructor
		object(const std::string &_Name,const object &_Object);
		
		//Protected destructor
		~object();
		
		void copy(const object &_Object);
	
	public:
	
		object &operator=(const object &_Object);
		
		const std::string &name() const;
		
		void category(const std::string &_Category);
		const std::string &category() const;
		
		void description(const std::string &_Description);
		const std::string &description() const;
	};
	
	struct constant:public object {
	
		friend math_solver;
	
	protected:
	
		real _Value;
		
		//Protected constructor
		constant(const std::string &_Name,real _Value);
		
		//Protected copy constructor
		constant(const constant &_Constant);
		
		//Protected alternative copy constructor
		constant(const std::string &_Name,const constant &_Constant);
		
		//Protected destructor
		~constant();
		
		void copy(const constant &_Constant);
	
	public:
	
		constant &operator=(const constant &_Constant);
		
		void value(real _Value);
		real value() const;
	};
	
	struct variable:public object {
	
		friend math_solver;
	
	protected:
	
		std::string _Value;
		
		//Protected constructor
		variable(const std::string &_Name,const std::string &_Value);
		
		//Protected copy constructor
		variable(const variable &_Variable);
		
		//Protected alternative copy constructor
		variable(const std::string &_Name,const variable &_Variable);
		
		//Protected destructor
		~variable();
		
		void copy(const variable &_Variable);
	
	public:
	
		variable &operator=(const variable &_Variable);
		
		void value(const std::string &_Value);
		const std::string &value() const;
	};
	
	struct function:public object {
	
		friend math_solver;
	
	public:
	
		typedef std::vector<real> arg;
		typedef real (*ptr)(const arg&);
	
	protected:
	
		ptr _Ptr;
		unsigned int _Parameters;
		unsigned int _Parameters_required;
		
		//Protected constructor
		function(const std::string &_Name,ptr _Ptr,
			unsigned int _Parameters);
		
		//Protected copy constructor
		function(const function &_Function);
		
		//Protected alternative copy constructor
		function(const std::string &_Name,const function &_Function);
		
		//Protected destructor
		~function();
		
		void copy(const function &_Function);
	
	public:
	
		function &operator=(const function &_Function);
		
		real operator()(const arg &_Arg);
		
		void pointer(ptr _Ptr);
		ptr pointer() const;
		
		void parameters(unsigned int _Parameters);
		unsigned int parameters() const;
		
		void parameters_required(unsigned int _Parameters_required);
		unsigned int parameters_required() const;
	};

private:

	typedef std::list<process*> _Processes;
	
	typedef std::list<constant*> _Constants;
	typedef std::list<variable*> _Variables;
	typedef std::list<function*> _Functions;
	
	//The difference between 1 and the smallest value greater
	//than 1 that is representable for the data type.
	static const real _Epsilon;
	
	//For double the significant is 53-bit (total 64-bit)
	//For long double the significant is 64-bit (total 80-bit)
	static const int _Significand_precision;
	
	real _Answer;
	string_list _Error_log;
	
	_Processes _Myprocesses;
	
	_Constants _Myconstants;
	_Variables _Myvariables;
	_Functions _Myfunctions;
	
	pthread_attr_t _Thread_attribute;
	pthread_mutex_t _Thread_mutex;
	bool _Threads_supported;
	
	void copy(const math_solver &_Math_solver);
	
	void log(const std::string &_Error_message);
	
	//Thread specific
	
	bool init_pthread();
	bool init_process(process *_Process) const;
	
	bool mutex_lock();
	bool mutex_unlock();
	
	static void *start_solve(void *_Data);
	
	//Step 1 (parse)
	static void parse(std::string _Data,_Expression &_Myexpression);
	
	static bool hexadecimal_number(unsigned char _Value);
	static bool decimal_number(unsigned char _Value);
	static bool octal_number(unsigned char _Value);
	static bool quaternary_number(unsigned char _Value);
	static bool binary_number(unsigned char _Value);
	static bool decimal_point(unsigned char _Value);
	
	//Step 2 (validate)
	bool validate(_Expression &_Myexpression);
	
	//Step 3 (finalize)
	bool finalize(_Expression &_Myexpression);
	
	//Step 4 (calculate)
	bool calculate(_Expression &_Myexpression,result &_Result);
	
	static void erase_elements(_Expression &_Myexpression,
		_Expression::iterator &_First,_Expression::iterator &_Last,
			std::list<_Expression::iterator*> &_Iterators);
	
	static void erase_iterator(_Expression::iterator &_Iterator,
		std::list<_Expression::iterator*> &_Iterators);
	
	static void erase_iterator(_Expression::iterator *_Iterator,
		std::list<_Expression::iterator*> &_Iterators);
	
	static void write_solution(const _Expression &_Myexpression,
		const _Symbol &_Mysymbol,const _Expression::iterator &_First,
			const _Expression::iterator &_Last,steps &_Solution);
	
	static void negate(_Symbol &_Mysymbol);
	static bool equal(real _Value,real _Value2);
	
	static bool nan(real _Value);
	static bool infinity(real _Value);
	static bool negative(real _Value);
	
	//Input/output
	static real to_decimal(const std::string &_Value,
		unsigned int _Bits,bool _Negative=false);
	static std::string from_decimal(real _Value,
		unsigned int _Bits,unsigned int _Precision=32);
	
	//Input/output (binary)
	static real from_binary(std::string _Binary,bool _Negative=false);
	static std::string to_binary(real _Value,unsigned int _Precision=32);

public:

	typedef _Processes::const_iterator processes_iterator; //Keep data safe
	
	typedef _Constants::const_iterator constants_iterator; //Keep data safe
	typedef _Variables::const_iterator variables_iterator; //Keep data safe
	typedef _Functions::const_iterator functions_iterator; //Keep data safe
	
	//Constructor
	math_solver();
	
	//Copy constructor
	math_solver(const math_solver &_Math_solver);
	
	//Destructor
	~math_solver();
	
	math_solver &operator=(const math_solver &_Math_solver);
	
	//Solve the given data as a numerical expression
	bool solve(const std::string &_Input,result &_Result);
	
	//Create a new process and
	//solve the given data as a numerical expression
	bool solve(const std::string &_Input,process *_Process);
	
	//Output
	
	real answer() const;
	
	void error_log(string_list &_Error_log) const;
	
	static std::string hexadecimal(real _Value,
		unsigned int _Precision=32);
	static std::string octal(real _Value,
		unsigned int _Precision=32);
	static std::string quaternary(real _Value,
		unsigned int _Precision=32);
	static std::string binary(real _Value,
		unsigned int _Precision=32);
	
	static std::string scinotation(real _Value,
		bool _E_notation=false);
	
	//Process
	
	process *create_process();
	process *create_process(const process &_Process);
	
	void remove_process(process *&_Process);
	void clear_processes();
	
	processes_iterator processes_begin() const;
	processes_iterator processes_end() const;
	
	//Constants
	
	constant *create_constant(const std::string &_Name,
		real _Value);
	constant *create_constant(const std::string &_Name,
		const constant &_Constant);
	
	void remove_constant(constant *&_Constant);
	void clear_constants();
	
	constants_iterator constants_begin() const;
	constants_iterator constants_end() const;
	
	constant *constant_pointer(const std::string &_Name) const;
	
	//Variables
	
	variable *create_variable(const std::string &_Name,
		const std::string &_Value);
	variable *create_variable(const std::string &_Name,
		const variable &_Variable);
	
	void remove_variable(variable *&_Variable);
	void clear_variables();
	
	variables_iterator variables_begin() const;
	variables_iterator variables_end() const;
	
	variable *variable_pointer(const std::string &_Name) const;
	
	//Functions
	
	function *create_function(const std::string &_Name,
		function::ptr _Ptr,unsigned int _Parameters);
	function *create_function(const std::string &_Name,
		const function &_Function);
	
	void remove_function(function *&_Function);
	void clear_functions();
	
	functions_iterator functions_begin() const;
	functions_iterator functions_end() const;
	
	function *function_pointer(const std::string &_Name) const;
};

} //namespace lunar

#endif