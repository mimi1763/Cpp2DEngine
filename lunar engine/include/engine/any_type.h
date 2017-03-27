/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: any_type.h
---------------------------------------------------
*/

#ifndef _ANY_TYPE_
#define _ANY_TYPE_

#include <utility>

namespace lunar {

class any_type {

private:

	class placeholder {
	
	public:
	
		//Constructor
		placeholder() {
			//Empty
		}
		
		//Virtual destructor
		virtual ~placeholder() {
			//Empty
		}
		
		virtual placeholder *clone() const=0;
	};
	
	template <typename _Ty>
	class holder:public placeholder {
	
	public:
	
		_Ty _Data;
		
		//Constructor
		explicit holder(const _Ty &_Data):_Data(_Data) {
			//Empty
		}
		
		//Virtual destructor
		virtual ~holder() {
			//Empty
		}
		
		virtual placeholder *clone() const {
			return new holder(_Data);
		}
	};
	
	placeholder *_Placeholder;

public:

	//Constructor
	any_type():_Placeholder(0) {
		//Empty
	}
	
	//Alternative constructor
	template <typename _Ty>
	explicit any_type(const _Ty &_Data):_Placeholder(new holder<_Ty>(_Data)) {
		//Empty
	}
	
	//Copy constructor
	any_type(const any_type &_Any_type):_Placeholder(_Any_type._Placeholder?
		_Any_type._Placeholder->clone():0) {
		//Empty
	}
	
	//Destructor
	~any_type() {
	
		if (_Placeholder) delete _Placeholder;
	}
	
	any_type &operator=(const any_type &_Any_type) {
	
		//Worth doing
		if (this!=&_Any_type) any_type(_Any_type).swap(*this);
	
	return *this;
	}
	
	template <typename _Ty>
	any_type &operator=(const _Ty &_Data) {
	
	any_type(_Data).swap(*this);
	return *this;
	}
	
	any_type &swap(any_type &_Any_type) {
	
	std::swap(_Placeholder,_Any_type._Placeholder);
	return *this;
	}
	
	template <typename _Ty>
	void data(const _Ty &_Data) {
		*this=_Data;
	}
	
	template <typename _Ty>
	const _Ty &data() const {
		return static_cast<any_type::holder<_Ty>*>(_Placeholder)->_Data;
	}
	
	bool empty() const {
		return !_Placeholder;
	}
};

} //namespace lunar

#endif