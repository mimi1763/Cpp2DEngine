/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: script_property.h
---------------------------------------------------
*/

#ifndef _SCRIPT_PROPERTY_
#define _SCRIPT_PROPERTY_

#include <vector>
#include "lexical_cast.h"

namespace lunar {

//Forward declarations
class script_parser;
class script_class;

class script_property {

	friend script_parser;
	friend script_class;

public:

	enum DATA_TYPE { DT_CUSTOM,
					 DT_BOOL,		//1/0, true/false, yes/no, on/off
					 DT_INT16,		//[-32 768, 32 767]
					 DT_UINT16,		//[0, 65 535]
					 DT_INT32,		//[-2 147 483 648, 2 147 483 647]
					 DT_UINT32,		//[0, 4 294 967 295]
					 DT_INT64,		//[-9 223 372 036 854 775 808, 9 223 372 036 854 775 807]
					 DT_UINT64,		//[0, 18 446 744 073 709 551 615]
					 DT_FLOAT,		//[-3.402823466e+38F, 3.402823466e+38F]
					 DT_DOUBLE,		//[-1.7976931348623158e+308, 1.7976931348623158e+308]
					 DT_STRING,		//"string"
					 DT_COLOR,		//<r,g,b,a>, <r,g,b> or hex rgba value
					 DT_VECTOR2 };	//<x,y> or scalar value
	
	//A struct that describes the property data
	struct data {
	
		friend script_parser;
	
	private:
	
		typedef std::vector<std::string> _Values;
		
		std::string _Property_name;
		_Values _Myvalues;
		
		void copy(const data &_Data);
	
	public:
	
		//Constructor
		data();
		
		//Copy constructor
		data(const data &_Data);
		
		//Destructor
		~data();
		
		data &operator=(const data &_Data);
		
		bool operator==(const std::string &_Property_name) const;
		bool operator!=(const std::string &_Property_name) const;
		
		const std::string &property_name() const;
		unsigned int values() const;
		
		std::string value(unsigned int _Index) const;
		
		template <typename _Ty>
		_Ty value(unsigned int _Index) const {
			return (_Index<_Myvalues.size()?lexical_cast<_Ty>(
				_Myvalues[_Index]):_Ty());
		}
	};

private:

	struct _Parameter {
	
	DATA_TYPE _Data_type;
	std::string _Custom_values;
	};
	
	typedef std::vector<_Parameter> _Parameters;
	
	std::string _Name;
	_Parameters _Myparameters;
	
	unsigned int _Required_parameters;
	bool _Variable_parameters;
	
	//Private constructor
	script_property(const std::string &_Name);
	
	//Private copy constructor
	script_property(const script_property &_Script_property);
	
	//Private alternative copy constructor
	script_property(const std::string &_Name,
		const script_property &_Script_property);
	
	//Private destructor
	~script_property();
	
	void copy(const script_property &_Script_property);

public:

	script_property &operator=(const script_property &_Script_property);
	
	const std::string &name() const;
	unsigned int parameters() const;
	
	unsigned int add_parameter(const DATA_TYPE &_Data_type,
		const std::string &_Custom_values="");
	unsigned int add_parameter(const std::string &_Custom_values);
	
	void remove_parameter(unsigned int _Index);
	void clear_parameters();
	
	void required_parameters(unsigned int _Required_parameters);
	unsigned int required_parameters() const;
	
	void variable_parameters(bool _Variable_parameters);
	bool variable_parameters() const;
};

} //namespace lunar

#endif