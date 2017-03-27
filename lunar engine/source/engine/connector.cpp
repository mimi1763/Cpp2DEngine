/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: connector.cpp
---------------------------------------------------
*/

#include "connector.h"

namespace lunar {

//Private

connector::connector(const std::string &_Database,const std::string &_Host,
	const std::string &_User,const std::string &_Password,
		unsigned int _Port,bool _Embedded):_Mysql(0),
			_Database(_Database),_Host(_Host),_User(_User),
				_Password(_Password),_Port(_Port),_Embedded(_Embedded) {
	//Empty
}

connector::connector(const connector &_Connector) {
	copy(_Connector);
}

connector::~connector() {
	disconnect();
}

void connector::copy(const connector &_Connector) {

_Database=_Connector._Database;
_Host=_Connector._Host;
_User=_Connector._User;
_Password=_Connector._Password;
_Port=_Connector._Port;
_Embedded=_Connector._Embedded;

_Myoptions=_Connector._Myoptions;
}

//Public

connector &connector::operator=(const connector &_Connector) {

	//Worth doing
	if (this!=&_Connector) copy(_Connector);

return *this;
}

void connector::connect() {

disconnect();
_Mysql=mysql_init(0);

	if (!_Mysql) return;
	
	//Use explicit options
	for (_Options::const_iterator _Iter=_Myoptions.begin(),
		_End=_Myoptions.end();_Iter!=_End;++_Iter) mysql_options(
			_Mysql,(*_Iter)._Option_type,(*_Iter)._Argument);
	
	if (_Embedded) {
	
	mysql_options(_Mysql,MYSQL_OPT_USE_EMBEDDED_CONNECTION,0); //Implicit option
	_Mysql=mysql_real_connect(_Mysql,0,0,0,_Database.c_str(),0,0,0);
	}
	else {
	
	mysql_options(_Mysql,MYSQL_OPT_USE_REMOTE_CONNECTION,0); //Implicit option
	_Mysql=mysql_real_connect(_Mysql,_Host.c_str(),_User.c_str(),
		_Password.c_str(),_Database.c_str(),_Port,0,0);
	}
}

void connector::disconnect() {

	if (_Mysql) {
	
	mysql_close(_Mysql);
	_Mysql=0;
	}
}

bool connector::query(const std::string &_Query) {

	if (!connected()) return false;

return (mysql_real_query(_Mysql,_Query.c_str(),
	(unsigned long)_Query.size())==0);
}

bool connector::query(const std::string &_Query,query_data &_Query_data) {

	if (!query(_Query)) return false;

MYSQL_RES *_Result=mysql_store_result(_Mysql);

	if (!_Result) return false;

unsigned int _Rows=(unsigned int)mysql_num_rows(_Result);
unsigned int _Fields=mysql_num_fields(_Result);
_Query_data.reserve(_Query_data.size()+_Rows);

MYSQL_ROW _Row;

	while ((_Row=mysql_fetch_row(_Result))) {
	
	row_data _Row_data;
	_Row_data.reserve(_Fields);
	
		for (unsigned int i=0;i<_Fields;++i)
			_Row_data.push_back(_Row[i]);
	
	_Query_data.push_back(_Row_data);
	}

mysql_free_result(_Result);
return true;
}

bool connector::connected() const {
	return (_Mysql?(mysql_ping(_Mysql)==0?true:false):false);
}

void connector::database(const std::string &_Database) {
	this->_Database=_Database;
}

const std::string &connector::database() const {
	return _Database;
}

void connector::host(const std::string &_Host) {
	this->_Host=_Host;
}

const std::string &connector::host() const {
	return _Host;
}

void connector::user(const std::string &_User) {
	this->_User=_User;
}

const std::string &connector::user() const {
	return _User;
}

void connector::password(const std::string &_Password) {
	this->_Password=_Password;
}

const std::string &connector::password() const {
	return _Password;
}

void connector::port(unsigned int _Port) {
	this->_Port=_Port;
}

unsigned int connector::port() const {
	return _Port;
}

void connector::embedded(bool _Embedded) {
	this->_Embedded=_Embedded;
}

bool connector::embedded() const {
	return _Embedded;
}

void connector::add_option(const mysql_option &_Option_type,
	const void *_Argument) {

	for (_Options::const_iterator _Iter=_Myoptions.begin(),
		_End=_Myoptions.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)._Option_type==_Option_type) return; //No duplicates allowed
	}

_Option _Myoption;
_Myoption._Option_type=_Option_type;
_Myoption._Argument=_Argument;
_Myoptions.push_back(_Myoption);
}

void connector::change_option(const mysql_option &_Option_type,
	const void *_Argument) {

	for (_Options::iterator _Iter=_Myoptions.begin(),
		_End=_Myoptions.end();_Iter!=_End;++_Iter) {
	
	_Option &_Myoption=(*_Iter); //Increase speed
	
		if (_Myoption._Option_type==_Option_type) {
		
		_Myoption._Argument=_Argument;
		return;
		}
	}
}

void connector::remove_option(const mysql_option &_Option_type) {

	for (_Options::iterator _Iter=_Myoptions.begin(),
		_End=_Myoptions.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)._Option_type==_Option_type) {
		
		_Myoptions.erase(_Iter);
		return;
		}
	}
}

void connector::clear_options() {
	_Myoptions.clear();
}

} //namespace lunar