/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: resource_manager.cpp
---------------------------------------------------
*/

#include "resource_manager.h"

//Dependencies
#include "system.h"
#include "file_manager.h"

#include "font_container.h"
#include "shader_container.h"
#include "sound_container.h"
#include "texture_container.h"
#include "animation_container.h"

#include "font_table.h"
#include "shader_table.h"
#include "sound_table.h"
#include "texture_table.h"
#include "animation_table.h"

#include "script_parser.h"
#include "connector.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
resource_manager *resource_manager::_Instance=0;

resource_manager::resource_manager():_Script_parser(0),_File_table(0),
	_Font_table(0),_Shader_table(0),_Sound_table(0),_Texture_table(0),
		_Animation_table(0) {

_File_container=file_container::_Instance=new file_container();
_Font_container=font_container::_Instance=new font_container();
_Shader_container=shader_container::_Instance=new shader_container();
_Sound_container=sound_container::_Instance=new sound_container();
_Texture_container=texture_container::_Instance=new texture_container();
_Animation_container=animation_container::_Instance=new animation_container();

//Add default extensions
_Font_extensions="bdf,cff,fnt,pcf,pfr,ttf";
_Shader_extensions="frag,vert";
_Sound_extensions="aif,aiff,asf,au,dls,flac,fsb,it,mid,midi,mod,mp2,mp3,ogg,rmi,s3m,snd,vag,wav,wma,xm,xma";
_Texture_extensions="bmp,cut,dib,dds,exr,g3,gif,hdr,icb,ico,iff,jbig,jng,jpg,jpeg,jpe,jif,koala,mng,pcx,pbm,pgm,ppm,pfm,png,pict,psd,pdd,raw,ras,rle,sgi,tdi,tga,tif,tiff,vda,vst,wbmp,xbm,xpm";
_Animation_extensions="3gp,amc,avi,dv,f4v,flc,flv,m2v,m4v,mkv,mp4,mpg,mpeg,mov,rm,rmvb,tg2,wmv";

//Add default data
_File_container->add_data(file_data(
	"default_particle","bin/scripts/default_particle.prog"));

_Shader_container->add_data(shader_data(
	"default_particle_vert","bin/shaders/default_particle.vert",
		shader_data::VERTEX));
_Shader_container->add_data(shader_data(
	"default_particle_frag","bin/shaders/default_particle.frag",
		shader_data::FRAGMENT));
}

resource_manager::resource_manager(const resource_manager &_Resource_manager) {

_Script_parser=0; //Not copyable
copy(_Resource_manager);
}

resource_manager::~resource_manager() {
	
	if (animation_table::_Instance) {
	
	delete animation_table::_Instance;
	animation_table::_Instance=0;
	}
	
	if (texture_table::_Instance) {
	
	delete texture_table::_Instance;
	texture_table::_Instance=0;
	}
	
	if (sound_table::_Instance) {
	
	delete sound_table::_Instance;
	sound_table::_Instance=0;
	}
	
	if (shader_table::_Instance) {
	
	delete shader_table::_Instance;
	shader_table::_Instance=0;
	}
	
	if (font_table::_Instance) {
	
	delete font_table::_Instance;
	font_table::_Instance=0;
	}
	
	if (file_table::_Instance) {
	
	delete file_table::_Instance;
	file_table::_Instance=0;
	}
	
	if (_Script_parser) delete _Script_parser;

delete animation_container::_Instance;
animation_container::_Instance=0;

delete texture_container::_Instance;
texture_container::_Instance=0;

delete sound_container::_Instance;
sound_container::_Instance=0;

delete shader_container::_Instance;
shader_container::_Instance=0;

delete font_container::_Instance;
font_container::_Instance=0;

delete file_container::_Instance;
file_container::_Instance=0;
}

resource_manager &resource_manager::operator=(const resource_manager &_Resource_manager) {

	//Worth doing
	if (this!=&_Resource_manager) copy(_Resource_manager);

return *this;
}

void resource_manager::copy(const resource_manager &_Resource_manager) {

_Font_extensions=_Resource_manager._Font_extensions;
_Shader_extensions=_Resource_manager._Shader_extensions;
_Sound_extensions=_Resource_manager._Sound_extensions;
_Texture_extensions=_Resource_manager._Texture_extensions;
_Animation_extensions=_Resource_manager._Animation_extensions;

_File_container=_Resource_manager._File_container;
_Font_container=_Resource_manager._Font_container;
_Shader_container=_Resource_manager._Shader_container;
_Sound_container=_Resource_manager._Sound_container;
_Texture_container=_Resource_manager._Texture_container;
_Animation_container=_Resource_manager._Animation_container;

_File_table=_Resource_manager._File_table;
_Font_table=_Resource_manager._Font_table;
_Shader_table=_Resource_manager._Shader_table;
_Sound_table=_Resource_manager._Sound_table;
_Texture_table=_Resource_manager._Texture_table;
_Animation_table=_Resource_manager._Animation_table;

_Mytables=_Resource_manager._Mytables;
}

void resource_manager::init_resourcescript(script_parser *_Script_parser) {

script_property *_Property;

//Resource class
script_class *_Resource_class=_Script_parser->create_class("resources");

_Property=_Resource_class->create_property("folder");
_Property->add_parameter(script_property::DT_STRING);

//Folder class
script_class *_Folder_class=_Script_parser->create_class("folder");

_Property=_Folder_class->create_property("naming");
_Property->add_parameter("file_path,file_name");

_Property=_Folder_class->create_property("ignore");
_Property->add_parameter(script_property::DT_STRING);
_Property->variable_parameters(true);

//File class
script_class *_File_class=_Script_parser->create_class("file");

_Property=_File_class->create_property("name");
_Property->add_parameter(script_property::DT_STRING);

_Property=_File_class->create_property("filename");
_Property->add_parameter(script_property::DT_STRING);

//Font class
script_class *_Font_class=_Script_parser->create_class("font");

_Property=_Font_class->create_property("face");
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Font_class->create_property("size");
_Property->add_parameter(script_property::DT_UINT32);
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Font_class->create_property("resolution");
_Property->add_parameter(script_property::DT_UINT32);
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Font_class->create_property("encoding");
_Property->add_parameter("ascii,extended_ascii");

//Shader class
script_class *_Shader_class=_Script_parser->create_class("shader");

_Property=_Shader_class->create_property("type");
_Property->add_parameter("vertex,fragment");

//Sound class
script_class *_Sound_class=_Script_parser->create_class("sound");

_Property=_Sound_class->create_property("stream");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Sound_class->create_property("software");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Sound_class->create_property("positional");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Sound_class->create_property("loop");
_Property->add_parameter(script_property::DT_BOOL);

//Texture class
script_class *_Texture_class=_Script_parser->create_class("texture");

_Property=_Texture_class->create_property("tileable");
_Property->add_parameter(script_property::DT_BOOL);

//Animation class
script_class *_Animation_class=_Script_parser->create_class("animation");

_Property=_Animation_class->create_property("stream");
_Property->add_parameter(script_property::DT_BOOL);

//Scopes
script_scope *_Resource_scope=_Script_parser->create_scope("resource");
_Resource_scope->register_class(_Resource_class);

script_scope *_Folder_scope=_Script_parser->create_scope("folder");
_Folder_scope->register_class(_Folder_class);

script_scope *_Data_scope=_Script_parser->create_scope("data");
_Data_scope->register_class(_File_class);
_Data_scope->register_class(_Font_class);
_Data_scope->register_class(_Shader_class);
_Data_scope->register_class(_Sound_class);
_Data_scope->register_class(_Texture_class);
_Data_scope->register_class(_Animation_class);

_Script_parser->register_scope(_Resource_scope);
_Resource_class->register_scope(_Folder_scope);
_Folder_class->register_scope(_Data_scope);

//Inheritance
_Font_class->register_inheritance(_File_class);
_Shader_class->register_inheritance(_File_class);
_Sound_class->register_inheritance(_File_class);
_Texture_class->register_inheritance(_File_class);
_Animation_class->register_inheritance(_File_class);
}

bool resource_manager::init_resources(const std::string &_Filename) {

	//Init script parser first time
	if (!_Script_parser) {
	
	_Script_parser=new script_parser();
	init_resourcescript(_Script_parser);
	}

script_parser::result _Result(script_parser::result::LINEAR);

	//Parse resource file
	if (!_Script_parser->parse_file(_Filename,_Result))
		return false;

struct _Folder {

	enum NAMING { FILE_PATH, FILE_NAME };
	typedef std::list<std::string> _Ignore_list;

std::string _Path;
NAMING _Naming;
_Ignore_list _Myignore_list;
};

typedef std::list<_Folder> _Folders;
typedef std::list<file_data*> _Resources;
typedef std::map<std::string,_Resources> _Custom_files;

_Folders _Myfolders;
_Custom_files _Mycustom_files;

_Folder *_Current_folder=0; //Current folder
file_data *_File_data=0; //Current data

	for (script_class::object::objects_iterator _Iter=_Result.objects_begin(),
		_End=_Result.objects_end();_Iter!=_End;++_Iter) {
	
	const script_class::object &_Object=(*_Iter); //Increase speed
	
		//Folder
		if (_Object=="folder") {
		
		std::string _Path=string_utility::trim(_Object.name());
		string_utility::format_path(_Path);
		
		_Current_folder=0;
		
			//Check if path exist
			for (_Folders::const_iterator _Fiter=_Myfolders.begin(),
				_Fend=_Myfolders.end();_Fiter!=_Fend;++_Fiter) {
			
			const _Folder &_Myfolder=(*_Fiter); //Increase speed
			
				if (string_utility::compare(_Myfolder._Path,_Path)) {
				
				_Current_folder=(_Folder*)&_Myfolder;
				break;
				}
			}
			
			//Skip folder
			if (!_Current_folder) continue;
		}
		//Data
		else if (_Object!="resources") {
		
		std::string _Name=string_utility::trim(_Object.name());
		
			//Invalid folder or filename, skip data
			if (!_Current_folder || _Name.empty()) continue;
		
		std::string _File_path=_Name.substr(0,_Current_folder->_Path.size());
		string_utility::format_path(_File_path);
		
			//Folder path located before filename
			if (string_utility::compare(_File_path,_Current_folder->_Path) &&
				_Name.size()>_Current_folder->_Path.size())
					_File_path=_Name;
			//Concatenate
			else _File_path=_Current_folder->_Path+_Name;
			
			//Use file name as resource name
			if (_Current_folder->_Naming==_Folder::FILE_NAME) {
			
			size_t _Begin_pos=_Name.find_last_of('/')+1;
			
				if (_Begin_pos==std::string::npos) _Begin_pos=0;
			
			size_t _End_pos=_Name.find_last_of('.');
			
				if (_Begin_pos<_End_pos) _Name=_Name.substr(
					_Begin_pos,_End_pos-_Begin_pos);
			}
			//Use file path as resource name (default)
			else _Name=_File_path;
		
		_File_data=0;
		
			if (_Object=="file")
				_File_data=new file_data(_Name,_File_path);
			else if (_Object=="font")
				_File_data=new font_data(_Name,_File_path);
			else if (_Object=="shader")
				_File_data=new shader_data(_Name,_File_path);
			else if (_Object=="sound")
				_File_data=new sound_data(_Name,_File_path);
			else if (_Object=="texture")
				_File_data=new texture_data(_Name,_File_path);
			else if (_Object=="animation")
				_File_data=new animation_data(_Name,_File_path);
			
			if (_File_data) {
			
			_Custom_files::iterator _Find=_Mycustom_files.end();
			
				//Search using lower case
				if (!_Mycustom_files.empty()) _Find=_Mycustom_files.find(
					string_utility::lower_case(_File_path));
				
				//Found file
				if (_Find!=_Mycustom_files.end())
					(*_Find).second.push_back(_File_data);
				else {
				
				_Custom_files::_Pairib _Pair=_Mycustom_files.insert(
					_Custom_files::value_type(string_utility::lower_case(
						_File_path),_Resources())); //Make lower case key
				(*_Pair.first).second.push_back(_File_data);
				}
			}
		}
		
		//Set properties
		for (script_class::object::data_iterator _Diter=_Object.data_begin(),
			_Dend=_Object.data_end();_Diter!=_Dend;++_Diter) {
		
		const script_property::data &_Data=(*_Diter); //Increase speed
		
			//Resource properties
			if (_Object=="resources") {
			
				if (_Data=="folder") {
				
				std::string _Path=string_utility::trim(_Data.value(0));
				string_utility::format_path(_Path);
				
				bool _Exist=false;
				
					//No duplicates allowed
					for (_Folders::const_iterator _Fiter=_Myfolders.begin(),
						_Fend=_Myfolders.end();_Fiter!=_Fend;++_Fiter) {
					
						if (string_utility::compare((*_Fiter)._Path,_Path)) {
						
						_Exist=true;
						break;
						}
					}
					
					//Add folder
					if (!_Exist) {
					
					_Folder _Myfolder;
					_Myfolder._Path=_Path;
					_Myfolder._Naming=_Folder::FILE_PATH;
					_Myfolders.push_back(_Myfolder);
					}
				}
			}
			//Folder properties
			else if (_Object=="folder") {
			
				//Invalid folder
				if (!_Current_folder) break;
				
				if (_Data=="naming") {
				
					if (string_utility::compare(_Data.value(0),"file_path"))
						_Current_folder->_Naming=_Folder::FILE_PATH;
					else if (string_utility::compare(_Data.value(0),"file_name"))
						_Current_folder->_Naming=_Folder::FILE_NAME;
				}
				else if (_Data=="ignore") {
				
					for (unsigned int i=0;i<_Data.values();++i) {
					
					std::string _Name=string_utility::trim(_Data.value(i));
					
						//Don't add name to ignore list
						if (_Name.empty()) continue;
					
					string_utility::replace(_Name,"\\","/");
					size_t _Pos=_Name.find_last_of('/');
					
						//Path detected
						if (_Pos!=std::string::npos && _Pos<_Name.size()-1)
							_Name=_Name.substr(_Pos+1);
					
					_Current_folder->_Myignore_list.push_back(_Name);
					}
				}
			}
			//Data properties
			else {
			
				//Shared property
				if (_Data=="name") {
				
				_File_data->name(_Data.value(0));
				continue;
				}
				
				if (_Object=="font") {
				
				font_data *_Font_data=static_cast<font_data*>(_File_data);
				
					if (_Data=="face")
						_Font_data->face(_Data.value<unsigned int>(0));
					else if (_Data=="size")
						_Font_data->size(_Data.value<unsigned int>(0),
							_Data.value<unsigned int>(1));
					else if (_Data=="resolution")
						_Font_data->resolution(_Data.value<unsigned int>(0),
							_Data.value<unsigned int>(1));
					else if (_Data=="encoding") {
					
						//Incase sensitive encoding compare
						if (string_utility::compare(_Data.value(0),"ascii"))
							_Font_data->encoding(font_data::ASCII);
						else if (string_utility::compare(_Data.value(0),"extended_ascii"))
							_Font_data->encoding(font_data::EXTENDED_ASCII);
					}
				}
				else if (_Object=="shader") {
				
				shader_data *_Shader_data=static_cast<shader_data*>(_File_data);
				
					if (_Data=="type") {
					
						//Incase sensitive type compare
						if (string_utility::compare(_Data.value(0),"vertex"))
							_Shader_data->type(shader_data::VERTEX);
						else if (string_utility::compare(_Data.value(0),"fragment"))
							_Shader_data->type(shader_data::FRAGMENT);
					}
				}
				else if (_Object=="sound") {
				
				sound_data *_Sound_data=static_cast<sound_data*>(_File_data);
				
					if (_Data=="stream")
						_Sound_data->stream(_Data.value<bool>(0));
					else if (_Data=="software")
						_Sound_data->software(_Data.value<bool>(0));
					else if (_Data=="positional")
						_Sound_data->positional(_Data.value<bool>(0));
					else if (_Data=="loop")
						_Sound_data->loop(_Data.value<bool>(0));
				}
				else if (_Object=="texture") {
				
				texture_data *_Texture_data=static_cast<texture_data*>(_File_data);
				
					if (_Data=="tileable")
						_Texture_data->tileable(_Data.value<bool>(0));
				}
				else if (_Object=="animation") {
				
				animation_data *_Animation_data=static_cast<animation_data*>(_File_data);
				
					if (_Data=="stream")
						_Animation_data->stream(_Data.value<bool>(0));
				}
			}
		}
	}

//Prepare extensions
string_utility::tokens _Font_ext;
string_utility::tokenize(_Font_extensions,",",_Font_ext);

string_utility::tokens _Shader_ext;
string_utility::tokenize(_Shader_extensions,",",_Shader_ext);

string_utility::tokens _Sound_ext;
string_utility::tokenize(_Sound_extensions,",",_Sound_ext);

string_utility::tokens _Texture_ext;
string_utility::tokenize(_Texture_extensions,",",_Texture_ext);

string_utility::tokens _Animation_ext;
string_utility::tokenize(_Animation_extensions,",",_Animation_ext);

system::file_list _Files;

	for (_Folders::const_iterator _Iter=_Myfolders.begin(),
		_End=_Myfolders.end();_Iter!=_End;++_Iter) {
	
	const _Folder &_Myfolder=(*_Iter); //Increase speed
	
	system::files(_Myfolder._Path,_Files);
	
		//Nothing to do
		if (_Files.empty()) continue;
		
		for (system::file_list::const_iterator _Fiter=_Files.begin(),
			_Fend=_Files.end();_Fiter!=_Fend;++_Fiter) {
		
		const system::file &_File=(*_Fiter); //Increase speed
		
		std::string _File_path=_Myfolder._Path+_File.name();
		_Custom_files::const_iterator _Find=_Mycustom_files.find(
			string_utility::lower_case(_File_path)); //Search using lower case
		
			//Found file, use custom resource data
			if (_Find!=_Mycustom_files.end()) {
			
			const _Resources &_Myresources=(*_Find).second; //Increase speed
			
				for (_Resources::const_iterator _Riter=_Myresources.begin(),
					_Rend=_Myresources.end();_Riter!=_Rend;++_Riter) {
				
				file_data *_File_data=(*_Riter); //Increase speed
				
					//Add data to the corresponding container
					if (dynamic_cast<font_data*>(_File_data))
						_Font_container->add_data(*static_cast<font_data*>(_File_data));
					else if (dynamic_cast<shader_data*>(_File_data))
						_Shader_container->add_data(*static_cast<shader_data*>(_File_data));
					else if (dynamic_cast<sound_data*>(_File_data))
						_Sound_container->add_data(*static_cast<sound_data*>(_File_data));
					else if (dynamic_cast<texture_data*>(_File_data))
						_Texture_container->add_data(*static_cast<texture_data*>(_File_data));
					else if (dynamic_cast<animation_data*>(_File_data))
						_Animation_container->add_data(*static_cast<animation_data*>(_File_data));
					//Add data as file (default)
					else _File_container->add_data(*_File_data);
				}
			}
			//Add resource with default properties
			else {
			
				//Check if file should be ignored
				if (!_Myfolder._Myignore_list.empty()) {
				
				bool _Ignore=false;
				
					for (_Folder::_Ignore_list::const_iterator _Iiter=_Myfolder.
						_Myignore_list.begin(),_Iend=_Myfolder._Myignore_list.end();
							_Iiter!=_Iend;++_Iiter) {
					
						if (string_utility::compare(*_Iiter,_File.name())) {
						
						_Ignore=true;
						break;
						}
					}
					
					//Ignore file
					if (_Ignore) continue;
				}
			
			size_t _Pos=_File.name().find_last_of('.');
			std::string _Name=(_Myfolder._Naming==_Folder::FILE_NAME?
				_File.name().substr(0,_Pos):_File_path);
			
				//File extension found, search for known file type
				if (_Pos!=std::string::npos && _Pos<_File.name().size()-1) {
				
				std::string _Extension=_File.name().substr(_Pos+1);
				
					//Check if file extension should be ignored
					if (!_Myfolder._Myignore_list.empty()) {
					
					_Extension="*."+_Extension; //Make search key
					bool _Ignore=false;
					
						for (_Folder::_Ignore_list::const_iterator _Iiter=_Myfolder.
							_Myignore_list.begin(),_Iend=_Myfolder._Myignore_list.end();
								_Iiter!=_Iend;++_Iiter) {
						
							if (string_utility::compare(*_Iiter,_Extension)) {
							
							_Ignore=true;
							break;
							}
						}
						
						//Ignore file
						if (_Ignore) continue;
					
					_Extension=_Extension.substr(2); //Revert back
					}
				
				bool _Found=false;
				
					//Font extensions
					for (size_t i=0;i<_Font_ext.size();++i) {
					
						if (string_utility::compare(_Font_ext[i],_Extension)) {
						
						_Font_container->add_data(font_data(_Name,_File_path));
						_Found=true;
						break;
						}
					}
					
					if (_Found) continue;
					
					//Shader extensions
					for (size_t i=0;i<_Shader_ext.size();++i) {
					
						if (string_utility::compare(_Shader_ext[i],_Extension)) {
						
						shader_data _Shader_data(_Name,_File_path);
						
							if (string_utility::compare(_Extension,"frag"))
								_Shader_data.type(shader_data::FRAGMENT);
						
						_Shader_container->add_data(_Shader_data);
						_Found=true;
						break;
						}
					}
					
					if (_Found) continue;
					
					//Sound extensions
					for (size_t i=0;i<_Sound_ext.size();++i) {
					
						if (string_utility::compare(_Sound_ext[i],_Extension)) {
						
						_Sound_container->add_data(sound_data(_Name,_File_path));
						_Found=true;
						break;
						}
					}
					
					if (_Found) continue;
					
					//Texture extensions
					for (size_t i=0;i<_Texture_ext.size();++i) {
					
						if (string_utility::compare(_Texture_ext[i],_Extension)) {
						
						_Texture_container->add_data(texture_data(_Name,_File_path));
						_Found=true;
						break;
						}
					}
					
					if (_Found) continue;
					
					//Animation extensions
					for (size_t i=0;i<_Animation_ext.size();++i) {
					
						if (string_utility::compare(_Animation_ext[i],_Extension)) {
						
						_Animation_container->add_data(animation_data(_Name,_File_path));
						_Found=true;
						break;
						}
					}
					
					if (_Found) continue;
				}
			
			//Add data as file (default)
			_File_container->add_data(file_data(_Name,_File_path));
			}
		}
	
	_Files.clear();
	}
	
	//Clean up temporary resources
	for (_Custom_files::const_iterator _Iter=_Mycustom_files.begin(),
		_End=_Mycustom_files.end();_Iter!=_End;++_Iter) {
	
	const _Resources &_Myresources=(*_Iter).second; //Increase speed
	
		for (_Resources::const_iterator _Riter=_Myresources.begin(),
			_Rend=_Myresources.end();_Riter!=_Rend;++_Riter)
				delete (*_Riter);
	}

return true;
}

//Public

resource_manager *resource_manager::instance() {
	return _Instance;
}

bool resource_manager::load(const std::string &_Filename) {
	return init_resources(_Filename);
}

bool resource_manager::load(connector *_Connector) {

	if (!_Connector->connected()) return false;

//Initialize unitialized instances in use
use_tables(_File_table,_Font_table,_Shader_table,_Sound_table,
	_Texture_table,_Animation_table);

bool _Loaded=false;

_Loaded|=_File_table->load(_Connector);
_Loaded|=_Font_table->load(_Connector);
_Loaded|=_Shader_table->load(_Connector);
_Loaded|=_Sound_table->load(_Connector);
_Loaded|=_Texture_table->load(_Connector);
_Loaded|=_Animation_table->load(_Connector);

	//Load data from custom tables
	for (_Tables::const_iterator _Iter=_Mytables.begin(),
		_End=_Mytables.end();_Iter!=_End;++_Iter)
			_Loaded|=(*_Iter)->load(_Connector);

return _Loaded;
}

//Extension functions

void resource_manager::font_extensions(const std::string &_Font_extensions) {
	this->_Font_extensions=_Font_extensions;
}

const std::string &resource_manager::font_extensions() const {
	return _Font_extensions;
}

void resource_manager::shader_extensions(const std::string &_Shader_extensions) {
	this->_Shader_extensions=_Shader_extensions;
}

const std::string &resource_manager::shader_extensions() const {
	return _Shader_extensions;
}

void resource_manager::sound_extensions(const std::string &_Sound_extensions) {
	this->_Sound_extensions=_Sound_extensions;
}

const std::string &resource_manager::sound_extensions() const {
	return _Sound_extensions;
}

void resource_manager::texture_extensions(const std::string &_Texture_extensions) {
	this->_Texture_extensions=_Texture_extensions;
}

const std::string &resource_manager::texture_extensions() const {
	return _Texture_extensions;
}

void resource_manager::animation_extensions(const std::string &_Animation_extensions) {
	this->_Animation_extensions=_Animation_extensions;
}

const std::string &resource_manager::animation_extensions() const {
	return _Animation_extensions;
}

//Container functions

void resource_manager::use_containers(file_container *_File_container,
	font_container *_Font_container,shader_container *_Shader_container,
		sound_container *_Sound_container,texture_container *_Texture_container,
			animation_container *_Animation_container) {

	if (_File_container) this->_File_container=_File_container;
	else this->_File_container=file_container::_Instance;
	
	if (_Font_container) this->_Font_container=_Font_container;
	else this->_Font_container=font_container::_Instance;
	
	if (_Shader_container) this->_Shader_container=_Shader_container;
	else this->_Shader_container=shader_container::_Instance;
	
	if (_Sound_container) this->_Sound_container=_Sound_container;
	else this->_Sound_container=sound_container::_Instance;
	
	if (_Texture_container) this->_Texture_container=_Texture_container;
	else this->_Texture_container=texture_container::_Instance;
	
	if (_Animation_container) this->_Animation_container=_Animation_container;
	else this->_Animation_container=animation_container::_Instance;
}

//Table functions

void resource_manager::use_tables(file_table *_File_table,font_table *_Font_table,
	shader_table *_Shader_table,sound_table *_Sound_table,texture_table *_Texture_table,
		animation_table *_Animation_table) {

	if (_File_table) this->_File_table=_File_table;
	else {
	
		if (!file_table::_Instance) file_table::
			_Instance=new file_table("file",file_container::_Instance);
	
	this->_File_table=file_table::_Instance;
	}
	
	if (_Font_table) this->_Font_table=_Font_table;
	else {
	
		if (!font_table::_Instance) font_table::
			_Instance=new font_table(font_container::_Instance);
	
	this->_Font_table=font_table::_Instance;
	}
	
	if (_Shader_table) this->_Shader_table=_Shader_table;
	else {
	
		if (!shader_table::_Instance) shader_table::
			_Instance=new shader_table(shader_container::_Instance);
	
	this->_Shader_table=shader_table::_Instance;
	}
	
	if (_Sound_table) this->_Sound_table=_Sound_table;
	else {
	
		if (!sound_table::_Instance) sound_table::
			_Instance=new sound_table(sound_container::_Instance);
	
	this->_Sound_table=sound_table::_Instance;
	}
	
	if (_Texture_table) this->_Texture_table=_Texture_table;
	else {
	
		if (!texture_table::_Instance) texture_table::
			_Instance=new texture_table(texture_container::_Instance);
	
	this->_Texture_table=texture_table::_Instance;
	}
	
	if (_Animation_table) this->_Animation_table=_Animation_table;
	else {
	
		if (!animation_table::_Instance) animation_table::
			_Instance=new animation_table(animation_container::_Instance);
	
	this->_Animation_table=animation_table::_Instance;
	}
}

void resource_manager::add_table(table *_Table) {

	for (_Tables::const_iterator _Iter=_Mytables.begin(),
		_End=_Mytables.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Table) return; //No duplicates allowed
	}

_Mytables.push_back(_Table);
}

void resource_manager::remove_table(table *_Table) {

	for (_Tables::iterator _Iter=_Mytables.begin(),
		_End=_Mytables.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Table) {
		
		_Mytables.erase(_Iter); //Only erase node
		return;
		}
	}
}

resource_manager::tables_iter resource_manager::tables_begin() const {
	return _Mytables.begin();
}

resource_manager::tables_iter resource_manager::tables_end() const {
	return _Mytables.end();
}

void resource_manager::clear_tables() {
	_Mytables.clear();
}

} //namespace lunar