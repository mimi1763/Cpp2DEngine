/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: file_manager.h
---------------------------------------------------
*/

#ifndef _FILE_MANAGER_
#define _FILE_MANAGER_

#include <string>
#include <vector>

namespace lunar {

class file_manager {

private:

	static std::string _Working_directory;
	
	//Private constructor
	file_manager();
	
	//Private copy constructor
	file_manager(const file_manager &_File_manager);
	
	//Private destructor
	~file_manager();
	
	void copy(const file_manager &_File_manager);

public:

	typedef std::vector<std::string> string_list;
	
	file_manager &operator=(const file_manager &_File_manager);
	
	//Plain
	
	static bool load(const std::string &_Filename,
		string_list &_Lines);
	
	static bool save(const std::string &_Filename,
		const std::string &_Data,bool _Append=false);
	
	static bool save(const std::string &_Filename,
		const string_list &_Lines,bool _Append=false);
	
	//Binary
	
	static bool load_binary(const std::string &_Filename,
		std::string &_Data);
	
	static bool load_binary(const std::string &_Filename,
		unsigned int _Position,unsigned int _Size,
			std::string &_Data);
	
	static bool save_binary(const std::string &_Filename,
		const std::string &_Data,bool _Append=false);
	
	//Encryption
	
	static bool load_decrypted(const std::string &_Filename,
		std::string &_Data,const std::string &_Key="");
	
	static bool load_decrypted(const std::string &_Filename,
		string_list &_Lines,const std::string &_Key="");
	
	static bool save_encrypted(const std::string &_Filename,
		const std::string &_Data,const std::string &_Key="");
	
	static bool save_encrypted(const std::string &_Filename,
		const string_list &_Lines,const std::string &_Key="");
	
	//Directory
	
	static bool working_directory(const std::string &_Working_directory);
	static const std::string &working_directory();
	
	static bool make_directory(std::string _Directory);
	static bool directory_exist(const std::string &_Directory);
};

} //namespace lunar

#endif