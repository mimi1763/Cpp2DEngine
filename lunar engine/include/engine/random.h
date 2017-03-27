/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: random.h
---------------------------------------------------
*/

#ifndef _RANDOM_
#define _RANDOM_

#include "random_manager.h"
#include "type_traits.h"

namespace lunar {

template <typename _Ty>
_Ty random_helper(tags::type_tag) {
	return random_manager::generate_int32(); //Default generate
}

template <typename _Ty>
_Ty random_helper(tags::bool_tag) {
	return (random_manager::generate_int32()%2==1);
}

template <typename _Ty>
_Ty random_helper(tags::char_tag) {
	return (char)(random_manager::generate_int32()%
		(unsigned char(-1)+1));
}

template <typename _Ty>
_Ty random_helper(tags::uchar_tag) {
	return (unsigned char)(random_manager::generate_int32()%
		(unsigned char(-1)+1));
}

template <typename _Ty>
_Ty random_helper(tags::wchar_tag) {
	return (wchar_t)(random_manager::generate_int32()%
		(wchar_t(-1)+1));
}

template <typename _Ty>
_Ty random_helper(tags::short_tag) {
	return (short)(random_manager::generate_int32()%
		(unsigned short(-1)+1));
}

template <typename _Ty>
_Ty random_helper(tags::ushort_tag) {
	return (unsigned short)(random_manager::generate_int32()%
		(unsigned short(-1)+1));
}

template <typename _Ty>
_Ty random_helper(tags::longlong_tag) {
	return random_manager::generate_int64();
}

template <typename _Ty>
_Ty random_helper(tags::ulonglong_tag) {
	return random_manager::generate_int64();
}

template <typename _Ty>
_Ty random_helper(tags::float_tag) {
	return random_manager::generate_single();
}

template <typename _Ty>
_Ty random_helper(tags::double_tag) {
	return random_manager::generate_double();
}

template <typename _Ty>
_Ty random_helper(tags::longdouble_tag) {
	return random_manager::generate_double();
}

template <typename _Ty>
_Ty random() {
	return random_helper<_Ty>(type_traits<_Ty>::category());
}

} //namespace

#endif