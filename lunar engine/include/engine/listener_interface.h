/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: listener_interface.h
---------------------------------------------------
*/

#ifndef _LISTENER_INTERFACE_
#define _LISTENER_INTERFACE_

#include <list>
#include "listener.h"

namespace lunar {

class interface_base {

protected:

	//Protected constructor
	interface_base() {
		//Empty
	}
	
	//Protected virtual destructor
	virtual ~interface_base() {
		//Empty
	}
};

template <typename _Ty>
class listener_interface:public interface_base {

protected:

	typedef std::list<_Ty> _Listeners;
	
	_Listeners _Mylisteners; //All
	_Listeners _Mypublic_listeners; //Public
	_Listeners _Mysecured_listeners; //Secured
	
	void copy(const listener_interface<_Ty> &_Listener_interface) {
		//Empty - override default memcpy behaviour
	}

public:

	//Can't use typedef typename _Listeners::const_iterator
	//because of IntelliSense bug with Visual Studio 2005
	typedef typename std::list<_Ty>::const_iterator listeners_iter; //Keep data safe
	
	//Constructor
	listener_interface() {
		//Empty
	}
	
	//Copy constructor
	listener_interface(const listener_interface<_Ty> &_Listener_interface) {
		copy(_Listener_interface);
	}
	
	//Virtual destructor
	virtual ~listener_interface() {
	
		if (!_Mylisteners.empty()) {
		
		listener *_Listener;
		
			//Notify unsubscribed to subscribers
			for (_Listeners::const_iterator _Iter=_Mylisteners.begin(),
				_End=_Mylisteners.end();_Iter!=_End;++_Iter) {
			
			_Listener=(*_Iter); //Increase speed
			
				if (_Listener->listening()) _Listener->unsubscribed(this);
			}
		}
	
	//Compile time check for template constraints
	//Requires that _Ty is a pointer
	//Requires that _Ty is derived from listener
	_Ty _Constraint=static_cast<_Ty>(0);
	
		if (_Constraint) _Constraint->listener::~listener();
	}
	
	listener_interface &operator=(const listener_interface<_Ty> &_Listener_interface) {
	
		//Worth doing
		if (this!=&_Listener_interface) copy(_Listener_interface);
	
	return *this;
	}
	
	void subscribe(const _Ty &_Listener,bool _Secured=false) {
	
		for (_Listeners::const_iterator _Iter=_Mylisteners.begin(),
			_End=_Mylisteners.end();_Iter!=_End;++_Iter) {
		
			//No duplicates allowed
			if ((*_Iter)==_Listener) return;
		}
	
	//All listeners
	//[ <secured> -> | <public> -> ]
	
		if (_Secured) {
		
		_Listeners::iterator _Iter=_Mylisteners.begin();
		std::advance(_Iter,_Mysecured_listeners.size());
		
		_Mysecured_listeners.push_back(_Listener);
		_Mylisteners.insert(_Iter,_Listener); //Insert secured before first public
		}
		else {
		
		_Mypublic_listeners.push_back(_Listener);
		_Mylisteners.push_back(_Listener);
		}
	}
	
	void unsubscribe(const _Ty &_Listener) {
	
		for (_Listeners::iterator _Iter=_Mypublic_listeners.begin(),
			_End=_Mypublic_listeners.end();_Iter!=_End;++_Iter) {
		
			if ((*_Iter)==_Listener) {
			
				//Notify unsubscribed to subscriber
				if (_Listener->listening()) _Listener->unsubscribed(this);
			
			_Mypublic_listeners.erase(_Iter); //Erase node from public
			
				for (_Iter=_Mylisteners.begin(),_End=_Mylisteners.end();
					_Iter!=_End;++_Iter) {
				
					if ((*_Iter)==_Listener) {
					
					_Mylisteners.erase(_Iter); //Erase node from all
					break;
					}
				}
			
			return;
			}
		}
	}
	
	void clear_listeners() {
	
		if (_Mypublic_listeners.empty()) return;
	
	listener *_Listener;
	
		//Notify unsubscribed to public subscribers
		for (_Listeners::const_iterator _Iter=_Mypublic_listeners.begin(),
			_End=_Mypublic_listeners.end();_Iter!=_End;++_Iter) {
		
		_Listener=(*_Iter); //Increase speed
		
			if (_Listener->listening()) _Listener->unsubscribed(this);
		}
	
	_Mypublic_listeners.clear();
	_Mylisteners=_Mysecured_listeners; //Only secured are remaining
	}
	
	typename listeners_iter listeners_begin() const {
		return _Mypublic_listeners.begin();
	}
	
	typename listeners_iter listeners_end() const {
		return _Mypublic_listeners.end();
	}
};

} //namespace lunar

#endif