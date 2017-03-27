/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: callback.h
---------------------------------------------------
*/

#ifndef _CALLBACK_
#define _CALLBACK_

namespace lunar {

template <typename _Arg>
class callback {

protected:

	void copy(const callback &_Callback) {
		//Empty - override default memcpy behaviour
	}

public:

	//Constructor
	callback() {
		//Empty
	}
	
	//Copy constructor
	callback(const callback &_Callback) {
		copy(_Callback);
	}
	
	//Virtual destructor
	virtual ~callback() {
		//Empty
	}
	
	callback &operator=(const callback &_Callback) {
	
		//Worth doing
		if (this!=&_Callback) copy(_Callback);
	
	return *this;
	}
	
	virtual void operator()(_Arg _Myarg)=0;
};

template <typename _Arg>
class static_callback:public callback<_Arg> {

public:

	typedef void (*function)(_Arg);

protected:

	function _Function;
	
	void copy(const static_callback &_Static_callback) {
		_Function=_Static_callback._Function;
	}

public:

	//Constructor
	static_callback(function _Function):
		_Function(_Function) {
		//Empty
	}
	
	//Copy constructor
	static_callback(const static_callback &_Static_callback) {
		copy(_Static_callback);
	}
	
	//Virtual destructor
	virtual ~static_callback() {
		//Empty
	}
	
	static_callback &operator=(const static_callback &_Static_callback) {
	
		//Worth doing
		if (this!=&_Static_callback) {
		
		callback::operator=(_Static_callback); //Invoke base operator=
		copy(_Static_callback);
		}
	
	return *this;
	}
	
	void operator()(_Arg _Myarg) {
		_Function(_Myarg);
	}
};

template <typename _Ty,typename _Arg>
class member_callback:public callback<_Arg> {

public:

	typedef void (_Ty::*function)(_Arg);

protected:

	function _Function;
	_Ty *_Class;
	
	void copy(const member_callback &_Member_callback) {
	
	_Function=_Member_callback._Function;
	_Class=_Member_callback._Class;
	}

public:

	//Constructor
	member_callback(function _Function,_Ty *_Class):
		_Function(_Function),_Class(_Class) {
		//Empty
	}
	
	//Copy constructor
	member_callback(const member_callback &_Member_callback) {
		copy(_Member_callback);
	}
	
	//Virtual destructor
	virtual ~member_callback() {
		//Empty
	}
	
	member_callback &operator=(const member_callback &_Member_callback) {
	
		//Worth doing
		if (this!=&_Member_callback) {
		
		callback::operator=(_Member_callback); //Invoke base operator=
		copy(_Member_callback);
		}
	
	return *this;
	}
	
	void operator()(_Arg _Myarg) {
		(_Class->*_Function)(_Myarg);
	}
};

} //namespace lunar

#endif