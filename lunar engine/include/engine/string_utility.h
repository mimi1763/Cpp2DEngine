/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: string_utility.h
---------------------------------------------------
*/

#ifndef _STRING_UTILITY_
#define _STRING_UTILITY_

#include <string>
#include <vector>

namespace lunar {

class string_utility {

private:

	//Private constructor
	string_utility();
	
	//Private destructor
	~string_utility();

public:

	typedef std::vector<std::string> tokens;
	
	static std::string trim(const std::string &_String);
	static std::string trim_left(const std::string &_String);
	static std::string trim_right(const std::string &_String);
	
	static std::string lower_case(const std::string &_String);
	static std::string upper_case(const std::string &_String);
	
	static std::string field(const std::string &_String,
		const size_t &_Width,char _Fill_char=' ',
			bool _Pad_left=true);
	static std::string fill(const std::string &_String,
		const size_t &_Width,char _Fill_char=' ',
			bool _Pad_left=true);
	
	static std::string format(std::string _Format,
		float _Value);
	
	//Incase sensitive compare
	static bool compare(const std::string &_String,
		const std::string &_String2);
	
	//Replace all occurrences of elem with elem 2
	static unsigned int replace(std::string &_String,
		const std::string &_Elem,const std::string &_Elem2);
	
	static void format_path(std::string &_Path,
		const std::string &_Delimiter="/");
	
	static void tokenize(const std::string &_String,
		const std::string &_Delimiter,tokens &_Tokens);
	
	template <typename _Ty>
	static std::string serialize(const _Ty &_Value) {
	
	std::string _String(sizeof(_Ty),0);
	memcpy(&_String[0],(const unsigned char*)&_Value,_String.size());
	return _String;
	}
	
	template <typename _Ty>
	static _Ty unserialize(const std::string &_String) {
	
	_Ty _Value;
	memcpy((unsigned char*)&_Value,&_String[0],_String.size());
	return _Value;
	}
};

} //namespace lunar

#endif