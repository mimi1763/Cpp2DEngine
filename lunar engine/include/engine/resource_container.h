/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: resource_container.h
---------------------------------------------------
*/

#ifndef _RESOURCE_CONTAINER_
#define _RESOURCE_CONTAINER_

#include <string>
#include <vector>
#include <map>

namespace lunar {

//Forward declarations
class resource_manager;
class resource_container;

class resource_data {

	friend resource_container;

protected:

	std::string _Name;
	
	//Protected constructor
	explicit resource_data(const std::string &_Name);
	
	//Protected copy constructor
	resource_data(const resource_data &_Resource_data);
	
	//Protected virtual destructor
	virtual ~resource_data();
	
	resource_data &operator=(const resource_data &_Resource_data);
	
	void copy(const resource_data &_Resource_data);

public:

	void name(const std::string &_Name);
	const std::string &name() const;
};

class resource_container {

protected:

	typedef std::vector<resource_data*> _Data;
	typedef std::map<std::string,unsigned int> _Datamap;
	
	_Data _Mydata;
	_Datamap _Mydatamap;
	
	//Protected constructor
	resource_container();
	
	//Protected copy constructor
	resource_container(const resource_container &_Resource_container);
	
	//Protected virtual destructor
	virtual ~resource_container();
	
	void copy(const resource_container &_Resource_container);

public:

	typedef _Data::const_iterator data_iter; //Keep data safe
	
	resource_container &operator=(const resource_container &_Resource_container);
	
	void increase_reserve(const size_t &_Count);
	
	resource_data *data(unsigned int _Index) const;
	resource_data *data(const std::string &_Name) const;
	
	unsigned int data_index(const std::string &_Name) const;
	
	void clear();
	
	data_iter data_begin() const;
	data_iter data_end() const;
};

} //namespace lunar

#endif