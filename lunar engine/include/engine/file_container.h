/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: file_container.h
---------------------------------------------------
*/

#ifndef _FILE_CONTAINER_
#define _FILE_CONTAINER_

#include "resource_container.h"

namespace lunar {

class file_data:public resource_data {

protected:

	std::string _Filename;
	
	void copy(const file_data &_File_data);

public:

	//Constructor
	file_data(const std::string &_Name,const std::string &_Filename);
	
	//Copy constructor
	file_data(const file_data &_File_data);
	
	//Virtual destructor
	virtual ~file_data();
	
	file_data &operator=(const file_data &_File_data);
	
	void filename(const std::string &_Filename);
	const std::string &filename() const;
};

class file_container:public resource_container {

	friend resource_manager;

private:

	static file_container *_Instance; //Grant resource_manager exclusive access

protected:

	struct _File {
	
	unsigned int _Datafile;
	unsigned int _Position;
	unsigned int _Bytes;
	};
	
	typedef std::map<std::string,_File> _Filemap;
	
	_Filemap _Myfilemap;
	std::string _Datafilename_prefix;
	std::string _Datafilename_suffix;
	
	void copy(const file_container &_File_container);
	
	void split_filename(const std::string &_Filename,
		std::string &_Filename_prefix,std::string &_Filename_suffix) const;

public:

	//Constructor
	file_container();
	
	//Copy constructor
	file_container(const file_container &_File_container);
	
	//Virtual destructor
	virtual ~file_container();
	
	file_container &operator=(const file_container &_File_container);
	
	static file_container *instance();
	
	file_data *add_data(const file_data &_File_data);
	
	bool load_datafile(const std::string &_Filename);
	bool save_datafile(const std::string &_Filename) const;
	
	bool load_bytes(file_data *_File_data,std::string &_Bytes) const;
};

} //namespace lunar

#endif