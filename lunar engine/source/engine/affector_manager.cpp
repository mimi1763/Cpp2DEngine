/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: affector_manager.cpp
---------------------------------------------------
*/

#include "affector_manager.h"

namespace lunar {

//Protected

affector_manager::affector_manager() {
	//Empty
}

affector_manager::affector_manager(const affector_manager &_Affector_manager) {
	copy(_Affector_manager);
}

affector_manager::~affector_manager() {
	clear_affectors();
}

void affector_manager::copy(const affector_manager &_Affector_manager) {
	//Empty - override default memcpy behaviour
}

//Public

affector_manager &affector_manager::operator=(const affector_manager &_Affector_manager) {

	//Worth doing
	if (this!=&_Affector_manager) copy(_Affector_manager);

return *this;
}

color_fader *affector_manager::create_colorfader() {

color_fader *_Mycolor_fader=new color_fader();
_Myaffectors.push_back(_Mycolor_fader);
return _Mycolor_fader;
}

color_fader *affector_manager::create_colorfader(const color_fader &_Color_fader) {

color_fader *_Mycolor_fader=new color_fader(_Color_fader);
_Myaffectors.push_back(_Mycolor_fader);
return _Mycolor_fader;
}

direction_randomizer *affector_manager::create_directionrandomizer() {

direction_randomizer *_Mydirection_randomizer=new direction_randomizer();
_Myaffectors.push_back(_Mydirection_randomizer);
return _Mydirection_randomizer;
}

direction_randomizer *affector_manager::create_directionrandomizer(
	const direction_randomizer &_Direction_randomizer) {

direction_randomizer *_Mydirection_randomizer=new direction_randomizer(
	_Direction_randomizer);
_Myaffectors.push_back(_Mydirection_randomizer);
return _Mydirection_randomizer;
}

gravitation *affector_manager::create_gravitation() {

gravitation *_Mygravitation=new gravitation();
_Myaffectors.push_back(_Mygravitation);
return _Mygravitation;
}

gravitation *affector_manager::create_gravitation(const gravitation &_Gravitation) {

gravitation *_Mygravitation=new gravitation(_Gravitation);
_Myaffectors.push_back(_Mygravitation);
return _Mygravitation;
}

linear_force *affector_manager::create_linearforce() {

linear_force *_Mylinear_force=new linear_force();
_Myaffectors.push_back(_Mylinear_force);
return _Mylinear_force;
}

linear_force *affector_manager::create_linearforce(const linear_force &_Linear_force) {

linear_force *_Mylinear_force=new linear_force(_Linear_force);
_Myaffectors.push_back(_Mylinear_force);
return _Mylinear_force;
}

scaler *affector_manager::create_scaler() {

scaler *_Myscaler=new scaler();
_Myaffectors.push_back(_Myscaler);
return _Myscaler;
}

scaler *affector_manager::create_scaler(const scaler &_Scaler) {

scaler *_Myscaler=new scaler(_Scaler);
_Myaffectors.push_back(_Myscaler);
return _Myscaler;
}

sine_force *affector_manager::create_sineforce() {

sine_force *_Mysineforce=new sine_force();
_Myaffectors.push_back(_Mysineforce);
return _Mysineforce;
}

sine_force *affector_manager::create_sineforce(const sine_force &_Sine_force) {

sine_force *_Mysineforce=new sine_force(_Sine_force);
_Myaffectors.push_back(_Mysineforce);
return _Mysineforce;
}

void affector_manager::remove_affector(affector *&_Affector) {

	for (_Affectors::iterator _Iter=_Myaffectors.begin(),
		_End=_Myaffectors.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Affector) {
		
		delete _Affector;
		_Affector=0; //Set referenced pointer to null
		
		_Myaffectors.erase(_Iter);
		return;
		}
	}
}

void affector_manager::clear_affectors() {

	for (_Affectors::const_iterator _Iter=_Myaffectors.begin(),
		_End=_Myaffectors.end();_Iter!=_End;++_Iter) delete (*_Iter);

_Myaffectors.clear();
}

affector_manager::affectors_iter affector_manager::affectors_begin() const {
	return _Myaffectors.begin();
}

affector_manager::affectors_iter affector_manager::affectors_end() const {
	return _Myaffectors.end();
}

} //namespace lunar