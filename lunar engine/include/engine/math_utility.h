/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: math_utility.h
---------------------------------------------------
*/

#ifndef _MATH_UTILITY_
#define _MATH_UTILITY_

#include <string>

namespace lunar {

//Forward declarations
class core;
class math_solver;

class math {

	friend core; //Grant core exclusive access to initialize this

public:

	static const float PI;
	static const float TWO_PI;
	static const float HALF_PI;
	
	static const float EPSILON;

private:

	static const float _Angle_difference;
	
	static const unsigned int _Tablesize;
	static const float _Tablefactor;
	
	static float _Sintable[];
	static float _Tantable[];
	
	static math_solver *_Math_solver;
	
	//Private constructor
	math();
	
	//Private destructor
	~math();

public:

	//Random helper functions
	
	//Returns value in range [0.0, 1.0]
	static float random();
	
	//Returns value in range [0.0, _Max]
	static float random(float _Max);
	
	//Returns value in range [_Min, _Max]
	static float random(float _Min,float _Max);
	
	//Returns value in range [0, _Max]
	static int random(int _Max);
	
	//Returns value in range [_Min, _Max]
	static int random(int _Min,int _Max);
	
	//Common math functions
	
	static bool equal(float _X,float _Y,
		float _Epsilon=EPSILON);
	
	static float fraction(float _X);
	static float round(float _X);
	
	static float factorial(unsigned int _X);
	static float choose(unsigned int _X,unsigned int _Y);
	static float permute(unsigned int _X,unsigned int _Y);
	
	static float sine(float _Radian,bool _Table=true);
	static float cosine(float _Radian,bool _Table=true);
	static float tangent(float _Radian,bool _Table=true);
	
	//Converts angle value in degree to radian
	static float degree(float _Degree);
	
	//Converts angle value in radian to degree
	static float radian(float _Radian);
	
	//Linear interpolation
	static float lerp(float _X,float _Y,float _Amount);
	
	//Spherical linear interpolation
	static float slerp(float _X,float _Y,float _Omega,
		float _Amount);
	
	//Math Solver interface
	
	//This code is based on the Math Solver source code.
	//Consult your license regarding permissions and restrictions.
	//Copyright (c) 2005-2011 by Jan Ivar Goli.
	
	//Solve a given mathematical expression
	static float solve(const std::string &_Expression);
	
	//Solve a given mathematical expression
	//Optional arguments can be used to replace variables like $a,$b,$c,...
	static float solve(std::string _Expression,float _Arg1,...);
};

} //namespace lunar

#endif