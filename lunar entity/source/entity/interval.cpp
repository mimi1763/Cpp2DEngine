/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: interval.cpp
---------------------------------------------------
*/

#include "interval.h"

//Private

void interval::copy(const interval &_Interval) {

_From=_Interval._From;
_To=_Interval._To;
_Policy=_Interval._Policy;
}

//Public

interval::interval():_From(0),_To(0),_Policy(HIGHEST) {
	//Empty
}

interval::interval(int _Value):_From(_Value),_To(_Value),
	_Policy(HIGHEST) {
	//Empty
}

interval::interval(int _From,int _To,const POLICY &_Policy):
	_From(_From>_To?_To:_From),_To(_To<_From?_From:_To),_Policy(_Policy) {
	//Empty
}

interval::interval(const interval &_Interval) {
	copy(_Interval);
}

interval::~interval() {
	//Empty
}

interval &interval::operator=(const interval &_Interval) {

	//Worth doing
	if (this!=&_Interval) copy(_Interval);

return *this;
}

interval interval::operator+(int _Value) const {
	return interval(_From+_Value,_To+_Value,_Policy);
}

interval interval::operator+(const interval &_Interval) const {
	return interval(_From+_Interval._From,_To+_Interval._To,_Policy);
}

interval interval::operator-(int _Value) const {
	return interval(_From-_Value,_To-_Value,_Policy);
}

interval interval::operator-(const interval &_Interval) const {
	return interval(_From-_Interval._From,_To-_Interval._To,_Policy);
}

interval &interval::operator+=(int _Value) {

_From+=_Value;
_To+=_Value;
return *this;
}

interval &interval::operator+=(const interval &_Interval) {

_From+=_Interval._From;
_To+=_Interval._To;
return *this;
}

interval &interval::operator-=(int _Value) {

_From-=_Value;
_To-=_Value;
return *this;
}

interval &interval::operator-=(const interval &_Interval) {

_From-=_Interval._From;
_To-=_Interval._To;
return *this;
}

void interval::range(int _From,int _To) {

this->_From=(_From>_To?_To:_From);
this->_To=(_To<_From?_From:_To);
}

int interval::from() const {
	return _From;
}

int interval::to() const {
	return _To;
}

void interval::policy(const POLICY &_Policy) {
	this->_Policy=_Policy;
}

interval::POLICY interval::policy() const {
	return _Policy;
}