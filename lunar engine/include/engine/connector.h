/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: connector.h
---------------------------------------------------
*/

#ifndef _CONNECTOR_
#define _CONNECTOR_

#include <string>
#include <vector>
#include <list>

#define HAVE_STRTOK_R //Disable warning

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT //Disable warning
#endif

#include "my_global.h"
#include "mysql.h"

//'config_win.h' has defined bool as BOOL. BOOL is defined as int by 
//'windef.h'. This mess causes all our bool's to be handled as int's, 
//and we get unresolved externals when using bool functions.
#ifdef bool
#undef bool
#endif

namespace lunar {

//Forward declarations
class database_server;

class connector {

	friend database_server;

private:

	struct _Option {
	
	mysql_option _Option_type;
	const void *_Argument;
	};
	
	typedef std::list<_Option> _Options;  
	
	MYSQL *_Mysql;
	
	std::string _Database;
	std::string _Host;
	std::string _User;
	std::string _Password;
	unsigned int _Port;
	bool _Embedded;
	
	_Options _Myoptions;
	
	//Private constructor
	explicit connector(const std::string &_Database="",
		const std::string &_Host="",const std::string &_User="",
			const std::string &_Password="",unsigned int _Port=0,
				bool _Embedded=true);
	
	//Private copy constructor
	connector(const connector &_Connector);
	
	//Private destructor
	~connector();
	
	void copy(const connector &_Connector);

public:

	typedef std::vector<std::string> row_data;
	typedef row_data::const_iterator row_iter;
	
	typedef std::vector<row_data> query_data;
	typedef query_data::const_iterator query_iter;
	
	connector &operator=(const connector &_Connector);
	
	void connect();
	void disconnect();
	
	bool query(const std::string &_Query);
	bool query(const std::string &_Query,query_data &_Query_data);
	
	bool connected() const;
	
	void database(const std::string &_Database);
	const std::string &database() const;
	
	void host(const std::string &_Host);
	const std::string &host() const;
	
	void user(const std::string &_User);
	const std::string &user() const;
	
	void password(const std::string &_Password);
	const std::string &password() const;
	
	void port(unsigned int _Port);
	unsigned int port() const;
	
	void embedded(bool _Embedded);
	bool embedded() const;
	
	void add_option(const mysql_option &_Option_type,const void *_Argument);
	void change_option(const mysql_option &_Option_type,const void *_Argument);
	
	void remove_option(const mysql_option &_Option_type);
	void clear_options();
};

} //namespace lunar

#endif