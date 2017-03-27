/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: file_container.cpp
---------------------------------------------------
*/

#include "file_container.h"

//Dependencies
#include "file_manager.h"
#include "string_utility.h"

#include "any_cast.h"

namespace lunar {

//File data
//Protected

void file_data::copy(const file_data &_File_data) {
	_Filename=_File_data._Filename;
}

//Public

file_data::file_data(const std::string &_Name,const std::string &_Filename):
	resource_data(_Name),_Filename(_Filename) {
	//Empty
}

file_data::file_data(const file_data &_File_data):
	resource_data(_File_data) {
	copy(_File_data);
}

file_data::~file_data() {
	//Empty
}

file_data &file_data::operator=(const file_data &_File_data) {

	//Worth doing
	if (this!=&_File_data) {
	
	resource_data::operator=(_File_data); //Invoke base operator=
	copy(_File_data);
	}

return *this;
}

void file_data::filename(const std::string &_Filename) {
	this->_Filename=_Filename;
}

const std::string &file_data::filename() const {
	return _Filename;
}

//File container
//Private

//This instance is allocated when resource_manager is initialized
//This instance is deallocated when resource_manager is destroyed
file_container *file_container::_Instance=0;

//Protected

void file_container::copy(const file_container &_File_container) {

_Datafilename_prefix=_File_container._Datafilename_prefix;
_Datafilename_suffix=_File_container._Datafilename_suffix;
}

void file_container::split_filename(const std::string &_Filename,
	std::string &_Filename_prefix,std::string &_Filename_suffix) const {

size_t _Position=_Filename.rfind('.');

	//Found '.'
	if (_Position!=std::string::npos) {
	
	_Filename_prefix=_Filename.substr(0,_Position);
	_Filename_suffix=_Filename.substr(_Position);
	}
	else {
	
	_Filename_prefix=_Filename;
	_Filename_suffix.clear();
	}
}

//Public

file_container::file_container() {
	//Empty
}

file_container::file_container(const file_container &_File_container):
	resource_container(_File_container) {
	copy(_File_container);
}

file_container::~file_container() {
	//Empty
}

file_container &file_container::operator=(const file_container &_File_container) {

	//Worth doing
	if (this!=&_File_container) {
	
	resource_container::operator=(_File_container); //Invoke base operator=
	copy(_File_container);
	}

return *this;
}

file_container *file_container::instance() {
	return _Instance;
}

file_data *file_container::add_data(const file_data &_File_data) {

file_data *_Data=static_cast<file_data*>(
	data(_File_data.name()));

	//Data already exists
	if (_Data) {
	
	*_Data=_File_data; //Replace with new data
	return _Data;
	}

_Data=new file_data(_File_data); //Make copy
_Mydata.push_back(_Data);

_Mydatamap.insert(_Datamap::value_type(_Data->name(),
	(unsigned int)_Mydata.size()-1));
return _Data;
}

bool file_container::load_datafile(const std::string &_Filename) {

std::string _Bytes;

	//File doesn't exist or checksum doesn't match
	if (!file_manager::load_decrypted(_Filename,_Bytes)) return false;

_Myfilemap.clear(); //Make ready for new mapping

string_utility::tokens _Tokens;
string_utility::tokenize(_Bytes,"#",_Tokens);

	for (size_t i=0;i<_Tokens.size();i+=4) {
	
		//There are one or more files left
		if (i+4<=_Tokens.size()) {
		
		_File _Myfile;
		_Myfile._Datafile=any_cast<unsigned int>(_Tokens[i+1]);
		_Myfile._Position=any_cast<unsigned int>(_Tokens[i+2]);
		_Myfile._Bytes=any_cast<unsigned int>(_Tokens[i+3]);
		_Myfilemap.insert(_Filemap::value_type(_Tokens[i],_Myfile));
		}
	}

split_filename(_Filename,_Datafilename_prefix,_Datafilename_suffix);
return true;
}

bool file_container::save_datafile(const std::string &_Filename) const {

	//Nothing to do
	if (_Mydata.empty()) return false;

std::string _Filename_prefix;
std::string _Filename_suffix;
split_filename(_Filename,_Filename_prefix,_Filename_suffix);

_Filemap _Saved_files;
std::stringstream _Dataheader;
std::string _Bytes;

unsigned int _Datafile=1;
unsigned int _Total_bytes=0;
bool _Append=false; //Start clean file

file_data *_File_data;

	for (_Data::const_iterator _Iter=_Mydata.begin(),
		_End=_Mydata.end();_Iter!=_End;++_Iter) {
	
	_File_data=static_cast<file_data*>(*_Iter); //Increase speed
	
	//Check if the physical file is already added to the data file
	_Filemap::const_iterator _Siter=_Saved_files.find(
		_File_data->filename());
	
		//Reuse saved file
		if (_Siter!=_Saved_files.end()) {
		
		const _File &_Saved_file=(*_Siter).second; //Increase speed
		
		_Dataheader<<_File_data->name()<<"#"<<_Saved_file._Datafile<<
			"#"<<_Saved_file._Position<<"#"<<_Saved_file._Bytes<<"#";
		continue;
		}
		
		if (file_manager::load_binary(_File_data->filename().
			c_str(),_Bytes)) {
		
			//Check for max byte limit (4 GB per data file)
			if (_Total_bytes+(unsigned int)_Bytes.size()<_Total_bytes) {
			
			++_Datafile; //Create new data file
			_Total_bytes=0;
			_Append=false; //Start clean file
			}
			
			if (file_manager::save_binary(_Filename_prefix+
				any_cast(_Datafile)+_Filename_suffix,_Bytes,_Append)) {
			
			_File _Saved_file;
			_Saved_file._Datafile=_Datafile;
			_Saved_file._Position=_Total_bytes;
			_Saved_file._Bytes=(unsigned int)_Bytes.size();
			_Saved_files.insert(_Filemap::value_type(_File_data->
				filename(),_Saved_file));
			
			_Dataheader<<_File_data->name()<<"#"<<_Datafile<<"#"<<
				_Total_bytes<<"#"<<_Bytes.size()<<"#";
			_Total_bytes+=(unsigned int)_Bytes.size();
			_Append=true; //Start appending bytes to the current file
			}
		}
	}

_Bytes=_Dataheader.str();

//Data header is empty
return (!_Bytes.empty() && file_manager::
	save_encrypted(_Filename,_Bytes));
}

bool file_container::load_bytes(file_data *_File_data,std::string &_Bytes) const {

_Filemap::const_iterator _Iter=_Myfilemap.find(_File_data->name());

	if (_Iter==_Myfilemap.end()) return false;

const _File &_Myfile=(*_Iter).second; //Increase speed
return file_manager::load_binary(_Datafilename_prefix+
	any_cast(_Myfile._Datafile)+_Datafilename_suffix,
		_Myfile._Position,_Myfile._Bytes,_Bytes);
}

} //namespace lunar