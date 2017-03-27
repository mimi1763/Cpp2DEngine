/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: database_server.cpp
---------------------------------------------------
*/

#include "database_server.h"

//Dependencies
#include "file_manager.h"

namespace lunar {

//Private

database_server::database_server():_Connect_timeout(10),
	_Read_timeout(10),_Write_timeout(10),_Reconnect(1),
		_Initialized(false) {
	//Empty
}

database_server::database_server(const database_server &_Database_server) {

_Initialized=false; //Not copyable
copy(_Database_server);
}

database_server::~database_server() {

clear_connectors();	
close();
}

database_server &database_server::operator=(const database_server &_Database_server) {

	//Worth doing
	if (this!=&_Database_server) copy(_Database_server);

return *this;
}

void database_server::copy(const database_server &_Database_server) {

_Data_path=_Database_server._Data_path;
_Language_path=_Database_server._Language_path;

_Connect_timeout=_Database_server._Connect_timeout;
_Read_timeout=_Database_server._Read_timeout;
_Write_timeout=_Database_server._Write_timeout;
_Reconnect=_Database_server._Reconnect;
}

//Public

database_server *database_server::instance() {

static database_server _Database_server;
return &_Database_server;
}

bool database_server::init(const std::string &_Data_path,
	const std::string &_Language_path) {

data_path(_Data_path);
language_path(_Language_path);
return init();
}

bool database_server::init() {

	if (_Data_path.empty() || _Language_path.empty()) return false;
	
	//Relative path
	if (_Data_path.size()<2 || _Data_path[1]!=':') {
	
	_Data_path=file_manager::working_directory()+_Data_path;
	_Language_path=file_manager::working_directory()+_Language_path;
	}
	
	//Check if directories exists
	if (!file_manager::directory_exist(_Data_path) ||
		!file_manager::directory_exist(_Language_path)) return false;

close(); //Reset database server

std::string _Data("--datadir="+_Data_path);
std::string _Language("--language="+_Language_path);

char *_Argv[]={"Lunar Engine",(char*)_Data.c_str(),
	(char*)_Language.c_str(),"--skip-innodb"};
char *_Groups[]={"server","client",0};

	if (mysql_library_init(sizeof(_Argv)/sizeof(char*),_Argv,_Groups)==0)
		_Initialized=true;

return _Initialized;
}

void database_server::close() {

	if (_Initialized) {
	
		for (_Connectors::const_iterator _Iter=_Myconnectors.begin(),
			_End=_Myconnectors.end();_Iter!=_End;++_Iter)
				(*_Iter)->disconnect();
	
	mysql_library_end(); //Function may take some time to complete
	_Initialized=false;
	}
}

void database_server::data_path(const std::string &_Data_path) {
	this->_Data_path=_Data_path;
}

const std::string &database_server::data_path() const {
	return _Data_path;
}

void database_server::language_path(const std::string &_Language_path) {
	this->_Language_path=_Language_path;
}

const std::string &database_server::language_path() const {
	return _Language_path;
}

void database_server::connect_timeout(unsigned int _Connect_timeout) {
	this->_Connect_timeout=_Connect_timeout;
}

unsigned int database_server::connect_timeout() const {
	return _Connect_timeout;
}

void database_server::read_timeout(unsigned int _Read_timeout) {
	this->_Read_timeout=_Read_timeout;
}

unsigned int database_server::read_timeout() const {
	return _Read_timeout;
}

void database_server::write_timeout(unsigned int _Write_timeout) {
	this->_Write_timeout=_Write_timeout;
}

unsigned int database_server::write_timeout() const {
	return _Write_timeout;
}

void database_server::reconnect(bool _Reconnect) {
	this->_Reconnect=(_Reconnect!=0);
}

bool database_server::reconnect() const {
	return (_Reconnect!=0);
}

connector *database_server::create_connector(const std::string &_Database,
	const std::string &_Host,const std::string &_User,const std::string &_Password,
		unsigned int _Port,bool _Embedded) {

connector *_Myconnector=new connector(_Database,_Host,_User,
	_Password,_Port,_Embedded);

//Add default server options
_Myconnector->add_option(MYSQL_OPT_CONNECT_TIMEOUT,&_Connect_timeout);
_Myconnector->add_option(MYSQL_OPT_READ_TIMEOUT,&_Read_timeout);
_Myconnector->add_option(MYSQL_OPT_WRITE_TIMEOUT,&_Write_timeout);
_Myconnector->add_option(MYSQL_OPT_RECONNECT,&_Reconnect);

_Myconnectors.push_back(_Myconnector);
return _Myconnector;
}

connector *database_server::create_connector(const connector &_Connector) {

connector *_Myconnector=new connector(_Connector);
_Myconnectors.push_back(_Myconnector);
return _Myconnector;
}

void database_server::remove_connector(connector *&_Connector) {

	for (_Connectors::iterator _Iter=_Myconnectors.begin(),
		_End=_Myconnectors.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Connector) {
		
		delete _Connector;
		_Connector=0; //Set referenced pointer to null
		
		_Myconnectors.erase(_Iter);
		return;
		}
	}
}

void database_server::clear_connectors() {

	for (_Connectors::const_iterator _Iter=_Myconnectors.begin(),
		_End=_Myconnectors.end();_Iter!=_End;++_Iter) delete (*_Iter);

_Myconnectors.clear();
}

database_server::connectors_iter database_server::connectors_begin() const {
	return _Myconnectors.begin();
}

database_server::connectors_iter database_server::connectors_end() const {
	return _Myconnectors.end();
}

} //namespace lunar