/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: database_server.h
---------------------------------------------------
*/

#ifndef _DATABASE_SERVER_
#define _DATABASE_SERVER_

#include "connector.h"

namespace lunar {

class database_server {

private:

	typedef std::list<connector*> _Connectors;
	
	_Connectors _Myconnectors;
	
	std::string _Data_path;
	std::string _Language_path;
	bool _Initialized;
	
	unsigned int _Connect_timeout;
	unsigned int _Read_timeout;
	unsigned int _Write_timeout;
	char _Reconnect;
	
	//Private constructor
	database_server();
	
	//Private copy constructor
	database_server(const database_server &_Database_server);
	
	//Private destructor
	~database_server();
	
	database_server &operator=(const database_server &_Database_server);
	
	void copy(const database_server &_Database_server);

public:

	typedef _Connectors::const_iterator connectors_iter; //Keep data safe
	
	//Singleton
	static database_server *instance();
	
	bool init(const std::string &_Data_path,const std::string &_Language_path);
	bool init();
	
	void close();
	
	void data_path(const std::string &_Data_path);
	const std::string &data_path() const;
	
	void language_path(const std::string &_Language_path);
	const std::string &language_path() const;
	
	void connect_timeout(unsigned int _Connect_timeout);
	unsigned int connect_timeout() const;
	
	void read_timeout(unsigned int _Read_timeout);
	unsigned int read_timeout() const;
	
	void write_timeout(unsigned int _Write_timeout);
	unsigned int write_timeout() const;
	
	void reconnect(bool _Reconnect);
	bool reconnect() const;
	
	connector *create_connector(const std::string &_Database="",
		const std::string &_Host="",const std::string &_User="",
			const std::string &_Password="",unsigned int _Port=0,
				bool _Embedded=true);
	
	connector *create_connector(const connector &_Connector);
	
	void remove_connector(connector *&_Connector);
	void clear_connectors();
	
	connectors_iter connectors_begin() const;
	connectors_iter connectors_end() const;
};

} //namespace lunar

#endif