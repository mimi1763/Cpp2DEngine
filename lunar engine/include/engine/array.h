/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: array.h
---------------------------------------------------
*/

#ifndef _ARRAY_
#define _ARRAY_

#include <cstring>
#include <cstdarg>

namespace lunar {

template <typename _Ty,unsigned int _Size>
class array {

private:

	_Ty _Arr[_Size];
	
	void copy(const array &_Array) {
		memcpy(_Arr,_Array._Arr,sizeof(_Arr));
	}
	
	void copy(_Ty _Arr[_Size]) {
		memcpy(this->_Arr,_Arr,sizeof(this->_Arr));
	}

public:

	//Constructor
	array() {
		//Empty
	}
	
	//Alternative constructor
	explicit array(_Ty _Value,...) {
	
	_Arr[0]=_Value;
	
	va_list _List;
	va_start(_List,_Value);
	
		for (unsigned int i=1;i<_Size;++i)
			_Arr[i]=va_arg(_List,_Ty);
	
	va_end(_List);
	}
	
	//Copy constructor
	array(const array &_Array) {
		copy(_Array);
	}
	
	//Alternative copy constructor
	array(_Ty _Arr[_Size]) {
		copy(_Arr);
	}
	
	//Destructor
	~array() {
		//Empty
	}
	
	array &operator=(const array &_Array) {
	
		if (this!=&_Array) copy(_Array);
	
	return *this;
	}
	
	array &operator=(_Ty _Arr[_Size]) {
	
	copy(_Arr);
	return *this;
	}
	
	_Ty &operator[](unsigned int _Off) {
		return _Arr[_Off];
	}
	
	_Ty &front(unsigned int _Off) {
		return _Arr[0];
	}
	
	_Ty &back(unsigned int _Off) {
		return _Arr[_Size-1];
	}
	
	unsigned int size() const {
		return _Size;
	}
};

} //namespace lunar

#endif