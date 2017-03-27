/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: manager.cpp
---------------------------------------------------
*/

#include "manager.h"

namespace lunar {

//Protected

void manager::copy(const manager &_Manager) {
	//Empty - override default memcpy behaviour
}

void manager::tidy() {

manageable *_Manageable;

	for (_Manageables::const_iterator _Iter=_Mymanageables.begin(),
		_End=_Mymanageables.end();_Iter!=_End;++_Iter) {
	
	_Manageable=(*_Iter); //Increase speed
	
	removed(_Manageable);
	delete _Manageable;
	}

_Mymanageables.clear();
_Mypublic_manageables.clear();
_Mysecured_manageables.clear();
}

void manager::removed(manageable *_Manageable) {
	//Empty
}

void manager::add(manageable *_Manageable,bool _Secured) {

//All manageables
//[ <secured> -> | <public> -> ]

	if (_Secured) {
	
	_Manageables::iterator _Iter=_Mymanageables.begin();
	std::advance(_Iter,_Mysecured_manageables.size());
	
	_Mysecured_manageables.push_back(_Manageable);
	_Mymanageables.insert(_Iter,_Manageable); //Insert secured before first public
	}
	else {
	
	_Mypublic_manageables.push_back(_Manageable);
	_Mymanageables.push_back(_Manageable);
	}
}

void manager::status(manageable *_Manageable,const manageable::STATUS &_Status) {
	_Manageable->status(_Status); //manager has friend access to manageable
}

//Public

manager::manager() {
	//Empty
}

manager::manager(const manager &_Manager) {
	copy(_Manager);
}

manager::~manager() {

//Warning. tidy should be called from all derived managers because
//the vtables are cleaned before the virtual function is called

	//If tidy is not called from one of the derived manager,
	//it's called without vtable to make sure memory is deallocated
	if (!_Mymanageables.empty()) tidy();
}

manager &manager::operator=(const manager &_Manager) {

	//Worth doing
	if (this!=&_Manager) copy(_Manager);

return *this;
}

void manager::update() {

	if (!_Mymanageables.empty()) {
	
		for (_Manageables::const_iterator _Iter=_Mymanageables.begin(),
			_End=_Mymanageables.end();_Iter!=_End;++_Iter) update(*_Iter);
	}
}

void manager::remove(manageable *&_Manageable) {

	for (_Manageables::iterator _Iter=_Mypublic_manageables.begin(),
		_End=_Mypublic_manageables.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Manageable) {
		
		_Mypublic_manageables.erase(_Iter); //Erase node from public
		
			for (_Iter=_Mymanageables.begin(),_End=_Mymanageables.end();
				_Iter!=_End;++_Iter) {
			
				if ((*_Iter)==_Manageable) {
				
				_Mymanageables.erase(_Iter); //Erase node from all
				break;
				}
			}
		
		removed(_Manageable);
		delete _Manageable;
		_Manageable=0; //Set referenced pointer to null
		return;
		}
	}
}

void manager::remove(manageable *_Manageable,unsigned int _Count) {

	//Nothing to do
	if (_Count==0) return;

unsigned int _Nodes=0;

	for (_Manageables::iterator _Iter=_Mypublic_manageables.begin(),
		_End=_Mypublic_manageables.end();_Iter!=_End;++_Iter,++_Nodes) {
	
		//Find first manageable to remove
		if ((*_Iter)==_Manageable) {
		
		_Nodes=(unsigned int)_Mypublic_manageables.size()-_Nodes; //Calculate remaining nodes
		
			if (_Count>_Nodes) _Count=_Nodes;
			
			for (_Manageables::iterator _Aiter=_Mymanageables.begin(),
				_Aend=_Mymanageables.end();_Aiter!=_Aend;++_Aiter) {
			
				if ((*_Aiter)==_Manageable) {
				
				std::advance(_Aend=_Aiter,_Count);
				
				//Erase range from all
				//This is safe because secured are stored first
				_Mymanageables.erase(_Aiter,_Aend); 
				break;
				}
			}
		
		_End=_Iter;
		
			do {
			
			_Manageable=(*_End++); //Increase speed
			
			removed(_Manageable);
			delete _Manageable;
			} while (--_Count>0);
		
		_Mypublic_manageables.erase(_Iter,_End); //Erase range from public
		return;
		}
	}
}

void manager::clear() {

manageable *_Manageable;

	for (_Manageables::const_iterator _Iter=_Mypublic_manageables.begin(),
		_End=_Mypublic_manageables.end();_Iter!=_End;++_Iter) {
	
	_Manageable=(*_Iter); //Increase speed
	
	removed(_Manageable);
	delete _Manageable;
	}

_Mypublic_manageables.clear();
_Mymanageables=_Mysecured_manageables; //Only secured are remaining
}

manager::manageables_iter manager::manageables_begin() const {
	return _Mypublic_manageables.begin();
}

manager::manageables_iter manager::manageables_end() const {
	return _Mypublic_manageables.end();
}

} //namespace lunar