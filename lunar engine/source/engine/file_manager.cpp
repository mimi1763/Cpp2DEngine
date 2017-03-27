/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: file_manager.cpp
---------------------------------------------------
*/

#include "file_manager.h"

//Dependencies
#include <fstream>
#include <sstream>
#include <iomanip>

#include <io.h>
#include <direct.h>
#include <sys/stat.h>

#include "sha.h"
#include "math_utility.h"
#include "string_utility.h"

namespace lunar {

//Private

std::string file_manager::_Working_directory;

file_manager::file_manager() {
	//Empty
}

file_manager::file_manager(const file_manager &_File_manager) {
	copy(_File_manager);
}

file_manager::~file_manager() {
	//Empty
}

void file_manager::copy(const file_manager &_File_manager) {
	//Empty - override default memcpy behaviour
}

//Public

file_manager &file_manager::operator=(const file_manager &_File_manager) {

	//Worth doing
	if (this!=&_File_manager) copy(_File_manager);

return *this;
}

//Plain

bool file_manager::load(const std::string &_Filename,
	string_list &_Lines) {

std::ifstream _Filestream(_Filename.c_str(),std::ios::in);

	if (_Filestream) {
	
	std::string _Line;
	
		while (!_Filestream.eof()) {
		
		std::getline(_Filestream,_Line);
		_Lines.push_back(_Line);
		}
	
	_Filestream.close();
	return true;
	}

return false;
}

bool file_manager::save(const std::string &_Filename,
	const std::string &_Data,bool _Append) {

std::ofstream _Filestream(_Filename.c_str(),_Append?
	std::ios::app:std::ios::out);

	if (_Filestream) {
	
	_Filestream<<_Data<<std::endl;
	_Filestream.close();
	return true;
	}

return false;
}

bool file_manager::save(const std::string &_Filename,
	const string_list &_Lines,bool _Append) {

std::ofstream _Filestream(_Filename.c_str(),_Append?
	std::ios::app:std::ios::out);

	if (_Filestream) {
	
		for (size_t i=0;i<_Lines.size();++i)
			_Filestream<<_Lines[i]<<std::endl;
	
	_Filestream.close();
	return true;
	}

return false;
}

//Binary

bool file_manager::load_binary(const std::string &_Filename,
	std::string &_Data) {

std::ifstream _Filestream(_Filename.c_str(),std::ios::in |
	std::ios::binary | std::ios::ate); //Eat file to end

	if (_Filestream) {
	
	std::ifstream::pos_type _Size=_Filestream.tellg();
	_Filestream.seekg(0,std::ios::beg); //Rewind
	
	char *_Bytes=new char[(unsigned int)_Size];
	_Filestream.read(_Bytes,_Size); //Read every byte, ignore special characters
	_Data.assign(_Bytes,(unsigned int)_Size);
	delete[] _Bytes;
	
	_Filestream.close();
	return true;
	}

return false;
}

bool file_manager::load_binary(const std::string &_Filename,
	unsigned int _Position,unsigned int _Size,
		std::string &_Data) {

	if (_Size==0) return false;

std::ifstream _Filestream(_Filename.c_str(),std::ios::in |
	std::ios::binary);

	if (_Filestream) {
	
	_Filestream.seekg(_Position); //Seek to position
	
	char *_Bytes=new char[_Size];
	_Filestream.read(_Bytes,_Size); //Read every byte, ignore special characters
	_Data.assign(_Bytes,_Size);
	delete[] _Bytes;
	
	_Filestream.close();
	return true;
	}

return false;
}

bool file_manager::save_binary(const std::string &_Filename,
	const std::string &_Data,bool _Append) {

std::ofstream _Filestream(_Filename.c_str(),(_Append?
	std::ios::app:std::ios::out) | std::ios::binary);

	if (_Filestream) {
	
	_Filestream.write(_Data.c_str(),(std::streamsize)_Data.size()); //Save all bytes
	_Filestream.close();
	return true;
	}

return false;
}

//Encryption

bool file_manager::load_decrypted(const std::string &_Filename,
	std::string &_Data,const std::string &_Key) {

	if (!load_binary(_Filename,_Data)) return false;

size_t _Datasize=_Data.size()/2;

	//Decrypt
	for (size_t i=0;i<_Datasize;++i) _Data[i]^=_Data[_Datasize+i];

_Data.erase(_Data.begin()+_Datasize,_Data.end());

	if (_Data.size()<72) return false;

std::string _Data_digest(_Data.substr(0,64));
std::string _Salt(_Data.substr(64,8));
_Data.erase(_Data.begin(),_Data.begin()+72);

sha _Sha;

	//Use custom key
	if (!_Key.empty()) _Sha.update((const unsigned char*)_Key.c_str(),
		(unsigned int)_Key.size());

_Sha.update((const unsigned char*)_Data.c_str(),(unsigned int)_Data.size());
_Sha.update((const unsigned char*)_Salt.c_str(),(unsigned int)_Salt.size());
const unsigned int *_Hashvalue=_Sha.final();

std::stringstream _Digest;
_Digest<<std::hex<<std::setfill('0'); //Hex mode

	//Make sure digest is always updated with 8 bytes
	for (unsigned int i=0;i<8;++i) _Digest<<std::setw(8)<<_Hashvalue[i];

return (_Digest.str()==_Data_digest);
}

bool file_manager::load_decrypted(const std::string &_Filename,
	string_list &_Lines,const std::string &_Key) {

std::string _Data;

	if (!load_decrypted(_Filename,_Data,_Key)) return false;

std::string _Line;

	for (size_t i=0;i<_Data.size();++i) {
	
		if (_Data[i]=='\n') {
		
		_Lines.push_back(_Line);
		_Line.clear();
		}
		else _Line+=_Data[i];
	}

return true;
}

bool file_manager::save_encrypted(const std::string &_Filename,
	const std::string &_Data,const std::string &_Key) {

sha _Sha;

	//Use custom key
	if (!_Key.empty()) _Sha.update((const unsigned char*)_Key.c_str(),
		(unsigned int)_Key.size());

_Sha.update((const unsigned char*)_Data.c_str(),(unsigned int)_Data.size());

std::string _Salt;

	//Generate salt
	for (unsigned int i=0;i<8;++i) _Salt.push_back((unsigned char)
		math::random(255));

_Sha.update((const unsigned char*)_Salt.c_str(),(unsigned int)_Salt.size());
const unsigned int *_Hashvalue=_Sha.final();

std::stringstream _Digest;
_Digest<<std::hex<<std::setfill('0'); //Hex mode

	//Make sure digest is always updated with 8 bytes
	for (unsigned int i=0;i<8;++i) _Digest<<std::setw(8)<<_Hashvalue[i];

std::string _Cipher_data(_Digest.str()+_Salt+_Data);
std::string _Xor_key;

	//Encrypt
	for (size_t i=0;i<_Cipher_data.size();++i) {
	
	_Xor_key.push_back((char)math::random(255));
	_Cipher_data[i]^=_Xor_key[i];
	}

_Cipher_data+=_Xor_key;
return save_binary(_Filename,_Cipher_data);
}

bool file_manager::save_encrypted(const std::string &_Filename,
	const string_list &_Lines,const std::string &_Key) {

std::string _Data;

	for (size_t i=0;i<_Lines.size();++i) _Data+=_Lines[i]+"\n";

return save_encrypted(_Filename,_Data,_Key);
}

//Directory

bool file_manager::working_directory(const std::string &_Working_directory) {

bool _Changed=(_chdir(_Working_directory.c_str())==0);

	if (_Changed) file_manager::_Working_directory=_Working_directory;
	else {
	
	//Use current directory
	char _Dir[_MAX_PATH];
	_getcwd(_Dir,_MAX_PATH);
	file_manager::_Working_directory=_Dir;
	}

return _Changed;
}

const std::string &file_manager::working_directory() {
	return _Working_directory;
}

bool file_manager::make_directory(std::string _Directory) {

	//Northing to do
	if (directory_exist(_Directory)) return true;

string_utility::format_path(_Directory);

bool _Created=false;
size_t _Pos=0;
size_t _Off=0;

	do {
	
	_Pos=_Directory.find("/",_Off);
	
		if (_Pos!=std::string::npos) {
		
		_Created=(_mkdir(_Directory.substr(0,_Pos+1).c_str())==0);
		_Off=_Pos+1;
		}
	
	} while (_Pos!=std::string::npos);

return _Created;
}

bool file_manager::directory_exist(const std::string &_Directory) {

	if (_access(_Directory.c_str(),0)!=0) return false;

struct stat _Stat;
stat(_Directory.c_str(),&_Stat);
return ((_Stat.st_mode & S_IFDIR)==S_IFDIR);
}

} //namespace lunar