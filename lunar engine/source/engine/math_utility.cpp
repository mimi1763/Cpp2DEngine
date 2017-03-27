/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: math_utility.cpp
---------------------------------------------------
*/

#include "math_utility.h"

//Dependencies
#include <cmath>
#include <cstdarg>
#include <vector>
#include <list>
#include <limits>
#include <utility>

#include "random.h"
#include "any_cast.h"

#include "math_solver.h"

namespace lunar {

//Public

const float math::PI=3.14159265358979323846f;
const float math::TWO_PI=PI*2.0f;
const float math::HALF_PI=PI*0.5f;

const float math::EPSILON=std::numeric_limits<float>::epsilon();

//Private

const float math::_Angle_difference=180.0f/PI;

const unsigned int math::_Tablesize=4096;
const float math::_Tablefactor=_Tablesize/TWO_PI;

float math::_Sintable[_Tablesize]={0};
float math::_Tantable[_Tablesize]={0};

math_solver *math::_Math_solver=0;

math::math() {

	//Initialize tables
	for (unsigned int i=0;i<_Tablesize;++i) {
	
	float _Angle=TWO_PI*i/_Tablesize;
    _Sintable[i]=sin(_Angle);
    _Tantable[i]=tan(_Angle);
	}
}

math::~math() {
	//Empty
}

//Public

//Random helper functions

float math::random() {
	return lunar::random<float>();
}

int math::random(int _Max) {
	return lunar::random<unsigned int>()%(_Max+1);
}

float math::random(float _Max) {
	return lunar::random<float>()*_Max;
}

int math::random(int _Min,int _Max) {
	return (lunar::random<unsigned int>()%(_Max-_Min+1))+_Min;
}

float math::random(float _Min,float _Max) {
	return lunar::random<float>()*(_Max-_Min)+_Min;
}

//Common math functions

bool math::equal(float _X,float _Y,float _Epsilon) {
	return (fabs(_X-_Y)<_Epsilon);
}

float math::fraction(float _X) {
	return _X-(_X<0.0f?ceil(_X):floor(_X));
}

float math::round(float _X) {
	return (fabs(fraction(_X))<0.5f?_X<0.0f?ceil(_X):
		floor(_X):_X<0.0f?floor(_X):ceil(_X));
}

float math::factorial(unsigned int _X) {

float _Value=1.0f;

	while (_X>1) _Value*=_X--;

return _Value;
}

float math::choose(unsigned int _X,unsigned int _Y) {

	if (_Y>_X) return 0.0f;

return factorial(_X)/(factorial(_Y)*factorial(_X-_Y));
}

float math::permute(unsigned int _X,unsigned int _Y) {

	if (_Y>_X) return 0.0f;

return (factorial(_X)/factorial(_X-_Y));
}

float math::sine(float _Radian,bool _Table) {
	return (_Table?_Sintable[int(_Radian*_Tablefactor)%_Tablesize]:
		sin(_Radian));
}

float math::cosine(float _Radian,bool _Table) {
	return (_Table?_Sintable[int((_Radian+HALF_PI)*_Tablefactor)%_Tablesize]:
		cos(_Radian));
}

float math::tangent(float _Radian,bool _Table) {
	return (_Table?_Tantable[int(_Radian*_Tablefactor)%_Tablesize]:
		tan(_Radian));
}

float math::degree(float _Degree) {
	return (_Degree/_Angle_difference);
}

float math::radian(float _Radian) {
	return (_Radian*_Angle_difference);
}

float math::lerp(float _X,float _Y,float _Amount) {
	return (_X+(_Y-_X)*_Amount);
}

float math::slerp(float _X,float _Y,float _Omega,
	float _Amount) {
	return (_X*sine(_Omega*(1.0f-_Amount))+_Y*sine(_Omega*_Amount))/
		sine(_Omega);
}

//Math Solver interface

math_solver::real sin(const math_solver::function::arg &_Arg) {
	return math::sine((float)_Arg[0]);
}

math_solver::real cos(const math_solver::function::arg &_Arg) {
	return math::cosine((float)_Arg[0]);
}

math_solver::real tan(const math_solver::function::arg &_Arg) {
	return math::tangent((float)_Arg[0]);
}

math_solver::real sqrt(const math_solver::function::arg &_Arg) {
	return sqrtl(_Arg[0]);
}

float math::solve(const std::string &_Expression) {

	//Initialize default math solver instance
	if (!_Math_solver) {
	
	static math_solver _Instance=math_solver();
	_Math_solver=&_Instance;
	
	_Math_solver->create_constant("pi",PI);
	
	_Math_solver->create_function("sin",&sin,1);
	_Math_solver->create_function("cos",&cos,1);
	_Math_solver->create_function("tan",&tan,1);
	_Math_solver->create_function("sqrt",&sqrt,1);
	}

math_solver::result _Result;
_Result.generate_solution(false);

_Math_solver->solve(_Expression,_Result);
return (float)(_Result?_Result.answer():math_solver::real());
}

float math::solve(std::string _Expression,float _Arg1,...) {

typedef std::pair<std::string,size_t> _Variable;
typedef std::list<_Variable> _Variables;

typedef std::pair<_Variable*,size_t> _Instance;
typedef std::list<_Instance> _Instances;

_Variables _Myvariables;
_Instances _Myinstances;

_Variable _Myvariable;
_Instance _Myinstance;

	//Search for variables
	for (size_t i=0;i+1<_Expression.size();++i) {
	
		//Variable prefix
		if (_Expression[i]!='$') continue;
		
		//Extract variable name
		for (size_t j=i+1;j<_Expression.size();++j) {
		
		char _Elem=_Expression[j]; //Increase speed
		
			if ((_Elem>='a' && _Elem<='z') || (_Elem>='A' && _Elem<='Z')) {
			
				if (_Myvariable.first.empty()) _Myinstance.second=i;
			
			_Myvariable.first+=_Elem;
			continue;
			}
		
		break;
		}
		
		//Found valid variable name
		if (!_Myvariable.first.empty()) {
		
		_Variables::iterator _Iter=_Myvariables.begin();
		
		bool _Unique=true;
		
			//Find correct iterator (alpha sorted)
			for (_Variables::const_iterator _End=_Myvariables.end();
				_Iter!=_End;++_Iter) {
			
			const _Variable &_Var=(*_Iter); //Increase speed
			
				//Don't add duplicates
				if (_Myvariable.first==_Var.first) {
				
				_Unique=false;
				break;
				}
				
				if (_Myvariable.first<_Var.first) break;
			}
			
			//Add variable
			if (_Unique) _Iter=_Myvariables.insert(_Iter,_Myvariable);
		
		//Add variable instance
		_Myinstance.first=&(*_Iter);
		_Myinstances.push_back(_Myinstance);
		
		i+=_Myvariable.first.size(); //Skip
		_Myvariable.first.clear();
		}
	}
	
	//Found one or more variables
	if (!_Myvariables.empty()) {
	
	std::vector<std::string> _Arguments;
	_Arguments.reserve(_Myvariables.size());
	_Variables::iterator _Iter=_Myvariables.begin();
	
	//Extract arguments
	va_list _List;
	va_start(_List,_Expression);
	_Arguments.push_back(any_cast(*va_arg(_List,float*))); //float&
	
		for (size_t i=1;i<_Myvariables.size();++i) {
		
		_Arguments[i]=any_cast((float)va_arg(_List,double)); //...
		(*++_Iter).second=i;
		}
	
	va_end(_List);
	
	int _Off=0;
	
		//Replace variable instances with the corresponding argument values
		for (_Instances::const_iterator _Iter=_Myinstances.begin(),
			_End=_Myinstances.end();_Iter!=_End;++_Iter) {
		
		const _Instance &_Inst=(*_Iter); //Increase speed
		
		_Expression.replace(_Inst.second+_Off,_Inst.first->first.size()+1,
			_Arguments[_Inst.first->second]);
		_Off+=(int)_Arguments[_Inst.first->second].size()-
			((int)_Inst.first->first.size()+1);
		}
	}

return solve(_Expression);
}

} //namespace lunar