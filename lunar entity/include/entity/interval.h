/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: interval.h
---------------------------------------------------
*/

#ifndef _INTERVAL_
#define _INTERVAL_

class interval {

public:

	enum POLICY { HIGHEST, LOWEST };

private:

	int _From;
	int _To;
	POLICY _Policy;
	
	void copy(const interval &_Interval);

public:

	//Constructor
	interval();
	
	//Alternative constructor
	interval(int _Value);
	
	//Alternative constructor
	interval(int _From,int _To,const POLICY &_Policy=HIGHEST);
	
	//Copy constructor
	interval(const interval &_Interval);
	
	//Destructor
	~interval();
	
	interval &operator=(const interval &_Interval);
	
	interval operator+(int _Value) const;
	interval operator+(const interval &_Interval) const;
	
	interval operator-(int _Value) const;
	interval operator-(const interval &_Interval) const;
	
	interval &operator+=(int _Value);
	interval &operator+=(const interval &_Interval);
	
	interval &operator-=(int _Value);
	interval &operator-=(const interval &_Interval);
	
	void range(int _From,int _To);
	int from() const;
	int to() const;
	
	void policy(const POLICY &_Policy);
	POLICY policy() const;
};

#endif