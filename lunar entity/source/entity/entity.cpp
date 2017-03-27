/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: entity.cpp
---------------------------------------------------
*/

#include "entity.h"

//Dependencies (engine)
#include "render_node.h"
#include "math_utility.h"
#include "animation.h"
#include "particle_system.h"

//Public

//Action
//Protected

entity::action::action(action_group *_Owner,bool _Enabled):
	_Owner(_Owner),_Type(CUSTOM),_Scope(TREE),_Start(0.0f),
		_Enabled(_Enabled) {
	//Empty
}

entity::action::action(const action &_Action):_Owner(_Action._Owner) {
	copy(_Action);
}

entity::action::action(action_group *_Owner,const action &_Action):
	_Owner(_Owner) {
	copy(_Action);
}

entity::action::~action() {
	//Empty
}

void entity::action::copy(const action &_Action) {

_Type=_Action._Type;
_Scope=_Action._Scope;
_Start=_Action._Start;
_Enabled=_Action._Enabled;

_Object_name=_Action._Object_name;
_Data=_Action._Data;
}

//Public

entity::action &entity::action::operator=(const action &_Action) {

	//Worth doing
	if (this!=&_Action) copy(_Action);

return *this;
}

entity::action_group *entity::action::owner() const {
	return _Owner;
}

entity::action::TYPE entity::action::type() const {
	return _Type;
}

void entity::action::scope(const SCOPE &_Scope) {
	this->_Scope=_Scope;
}

void entity::action::scope(const SCOPE &_Scope,const std::string &_Object_name) {

this->_Scope=_Scope;
this->_Object_name=_Object_name;
}

entity::action::SCOPE entity::action::scope() const {
	return _Scope;
}

void entity::action::start(float _Start) {
	this->_Start=_Start;
}

float entity::action::start() const {
	return _Start;
}

void entity::action::enabled(bool _Enabled) {
	this->_Enabled=_Enabled;
}

bool entity::action::enabled() const {
	return _Enabled;
}

const any_type &entity::action::data() const {
	return _Data;
}

//Type/data

void entity::action::translate(const vector2 &_Translate) {

_Type=TRANSLATE;
_Data=_Translate;
}

void entity::action::rotate(float _Rotate) {

_Type=ROTATE;
_Data=_Rotate;
}

void entity::action::scale(const vector2 &_Scale) {

_Type=SCALE;
_Data=_Scale;
}

void entity::action::position(const vector2 &_Position) {

_Type=POSITION;
_Data=_Position;
}

void entity::action::rotation(float _Rotation) {

_Type=ROTATION;
_Data=_Rotation;
}

void entity::action::scaling(const vector2 &_Scaling) {

_Type=SCALING;
_Data=_Scaling;
}

void entity::action::color_channel(const color &_Color) {

_Type=COLOR;
_Data=_Color;
}

void entity::action::opacity(float _Opacity) {

_Type=OPACITY;
_Data=_Opacity;
}

void entity::action::visible(bool _Visible) {

_Type=VISIBLE;
_Data=_Visible;
}

void entity::action::sound_enabled(bool _Enabled) {

_Type=SOUND_ENABLED;
_Data=_Enabled;
}

void entity::action::sound_volume(float _Volume) {

_Type=SOUND_VOLUME;
_Data=_Volume;
}

void entity::action::light_enabled(bool _Enabled) {

_Type=LIGHT_ENABLED;
_Data=_Enabled;
}

void entity::action::light_brightness(float _Brightness) {

_Type=LIGHT_BRIGHTNESS;
_Data=_Brightness;
}

void entity::action::animation_enabled(bool _Enabled) {

_Type=ANIMATION_ENABLED;
_Data=_Enabled;
}

void entity::action::particlesystem_enabled(bool _Enabled) {

_Type=PARTICLESYSTEM_ENABLED;
_Data=_Enabled;
}

void entity::action::custom(float _Value) {

_Type=CUSTOM;
_Data=_Value;
}

//Motion
//Protected

entity::motion::motion(action_group *_Owner,bool _Enabled):
	action(_Owner,_Enabled),_Technique(LERP),_Duration(0.0f),
		_Progress(0.0f) {
	//Empty
}

entity::motion::motion(const motion &_Motion):action(_Motion) {
	copy(_Motion);
}

entity::motion::motion(action_group *_Owner,const motion &_Motion):
	action(_Owner,_Motion) {
	copy(_Motion);
}

entity::motion::~motion() {
	//Empty
}

void entity::motion::copy(const motion &_Motion) {

_Technique=_Motion._Technique;
_Duration=_Motion._Duration;
_Progress=_Motion._Progress;

_Data2=_Motion._Data2;
}

//Public

entity::motion &entity::motion::operator=(const motion &_Motion) {

	//Worth doing
	if (this!=&_Motion) {
	
	action::operator=(_Motion); //Invoke base operator=
	copy(_Motion);
	}

return *this;
}

void entity::motion::technique(const TECHNIQUE &_Technique) {
	this->_Technique=_Technique;
}

entity::motion::TECHNIQUE entity::motion::technique() const {
	return _Technique;
}

void entity::motion::duration(float _Duration) {
	this->_Duration=_Duration;
}

float entity::motion::duration() const {
	return _Duration;
}

float entity::motion::progress() const {
	return _Progress;
}

const any_type &entity::motion::data2() const {
	return _Data2;
}

//Type/data

void entity::motion::translate(const vector2 &_To) {

_Type=TRANSLATE;
_Data=vector2::ZERO;
_Data2=_To;
}

void entity::motion::rotate(float _To) {

_Type=ROTATE;
_Data=0.0f;
_Data2=_To;
}

void entity::motion::scale(const vector2 &_To) {

_Type=SCALE;
_Data=vector2::ZERO;
_Data2=_To;
}

void entity::motion::position(const vector2 &_From,const vector2 &_To) {

_Type=POSITION;
_Data=_From;
_Data2=_To;
}

void entity::motion::rotation(float _From,float _To) {

_Type=ROTATION;
_Data=_From;
_Data2=_To;
}

void entity::motion::scaling(const vector2 &_From,const vector2 &_To) {

_Type=SCALING;
_Data=_From;
_Data2=_To;
}

void entity::motion::color_channel(const color &_From,const color &_To) {

_Type=COLOR;
_Data=_From;
_Data2=_To;
}

void entity::motion::opacity(float _From,float _To) {

_Type=OPACITY;
_Data=_From;
_Data2=_To;
}

void entity::motion::visible(bool _From,bool _To) {

_Type=VISIBLE;
_Data=_From;
_Data2=_To;
}

void entity::motion::sound_enabled(bool _From,bool _To) {

_Type=SOUND_ENABLED;
_Data=_From;
_Data2=_To;
}

void entity::motion::sound_volume(float _From,float _To) {

_Type=SOUND_VOLUME;
_Data=_From;
_Data2=_To;
}

void entity::motion::light_enabled(bool _From,bool _To) {

_Type=LIGHT_ENABLED;
_Data=_From;
_Data2=_To;
}

void entity::motion::light_brightness(float _From,float _To) {

_Type=LIGHT_BRIGHTNESS;
_Data=_From;
_Data2=_To;
}

void entity::motion::animation_enabled(bool _From,bool _To) {

_Type=ANIMATION_ENABLED;
_Data=_From;
_Data2=_To;
}

void entity::motion::particlesystem_enabled(bool _From,bool _To) {

_Type=PARTICLESYSTEM_ENABLED;
_Data=_From;
_Data2=_To;
}

void entity::motion::custom(float _From,float _To) {

_Type=CUSTOM;
_Data=_From;
_Data2=_To;
}

//Action group
//Private

entity::action_group::action_group(const std::string &_Name,
	bool _Enabled):_Name(_Name),_Time(0.0f),_Start(0.0f),
		_Enabled(_Enabled),_Automatic(true),_Loop(0),
			_Current_loop(0),_Reverse(false) {
	//Empty
}

entity::action_group::action_group(const action_group &_Action_group):
	_Name(_Action_group._Name) {
	copy(_Action_group);
}

entity::action_group::action_group(const std::string &_Name,
	const action_group &_Action_group):_Name(_Name) {
	copy(_Action_group);
}

entity::action_group::~action_group() {
	clear_actions();
}

void entity::action_group::copy(const action_group &_Action_group) {

_Time=_Action_group._Time;
_Start=_Action_group._Start;
_Enabled=_Action_group._Enabled;
_Automatic=_Action_group._Automatic;

_Loop=_Action_group._Loop;
_Current_loop=_Action_group._Current_loop;
_Reverse=_Action_group._Reverse;

	if (!_Action_group._Myactions.empty()) {
	
	action *_Action;
	
		for (_Actions::const_iterator _Iter=_Action_group._Myactions.begin(),
			_End=_Action_group._Myactions.end();_Iter!=_End;++_Iter) {
		
		_Action=(*_Iter); //Increase speed
		
		motion *_Motion=dynamic_cast<motion*>(_Action);
		
			if (_Motion) create_motion(*_Motion);
			else create_action(*_Action);
		}
	}
}

//Public

entity::action_group &entity::action_group::operator=(const action_group &_Action_group) {

	//Worth doing
	if (this!=&_Action_group) {
	
	clear_actions();
	copy(_Action_group);
	}

return *this;
}

const std::string &entity::action_group::name() const {
	return _Name;
}

void entity::action_group::start(float _Start) {
	this->_Start=_Start;
}

float entity::action_group::start() const {
	return _Start;
}

void entity::action_group::enabled(bool _Enabled) {
	this->_Enabled=_Enabled;
}

bool entity::action_group::enabled() const {
	return _Enabled;
}

void entity::action_group::automatic(bool _Automatic) {

	if (_Automatic) _Reverse=false;

this->_Automatic=_Automatic;
}

bool entity::action_group::automatic() const {
	return _Automatic;
}

void entity::action_group::loop(unsigned int _Loop) {

	//Update current loop
	if (this->_Loop==_Current_loop || _Current_loop>_Loop)
		_Current_loop=_Loop;

this->_Loop=_Loop;
}

unsigned int entity::action_group::loop() const {
	return _Loop;
}

void entity::action_group::reset_loop() {
	_Current_loop=_Loop;
}

void entity::action_group::reverse() {

	if (!_Automatic) _Reverse=!_Reverse;
}

//Actions/motions

entity::action *entity::action_group::create_action(bool _Enabled) {

action *_Myaction=new action(this,_Enabled);
_Myactions.push_back(_Myaction);
return _Myaction;
}

entity::action *entity::action_group::create_action(const action &_Action) {

action *_Myaction=new action(this,_Action);
_Myactions.push_back(_Myaction);
return _Myaction;
}

entity::motion *entity::action_group::create_motion(bool _Enabled) {

motion *_Mymotion=new motion(this,_Enabled);
_Myactions.push_back(_Mymotion);
return _Mymotion;
}

entity::motion *entity::action_group::create_motion(const motion &_Motion) {

motion *_Mymotion=new motion(this,_Motion);
_Myactions.push_back(_Mymotion);
return _Mymotion;
}

void entity::action_group::remove_action(action *&_Action) {

	for (_Actions::iterator _Iter=_Myactions.begin(),
		_End=_Myactions.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Action) {
		
		delete _Action;
		_Action=0; //Set referenced pointer to null
		
		_Myactions.erase(_Iter);
		return;
		}
	}
}

void entity::action_group::clear_actions() {

	for (_Actions::const_iterator _Iter=_Myactions.begin(),
		_End=_Myactions.end();_Iter!=_End;++_Iter)
			delete (*_Iter);

_Myactions.clear();
}

entity::action_group::actions_iter entity::action_group::actions_begin() const {
	return _Myactions.begin();
}

entity::action_group::actions_iter entity::action_group::actions_end() const {
	return _Myactions.end();
}

//Entity
//Protected

entity::entity(entity_manager *_Owner,render_node *_Parent_node,
	const interval &_Draw_order,bool _Visible):
		static_entity(_Owner,_Parent_node,_Draw_order,_Visible),
			_Time(0.0f) {
	//Empty
}

entity::entity(const entity &_Entity):static_entity(_Entity) {
	copy(_Entity);
}

entity::entity(entity_manager *_Owner,render_node *_Parent_node,
	const interval &_Draw_order,const entity &_Entity):
		static_entity(_Owner,_Parent_node,_Draw_order,_Entity) {
	copy(_Entity);
}

entity::~entity() {
	clear_actiongroups();
}

void entity::copy(const entity &_Entity) {

_Time=_Entity._Time;

	if (!_Entity._Myaction_groups.empty()) {
	
	action_group *_Action_group;
	
		for (_Action_groups::const_iterator _Iter=_Entity._Myaction_groups.begin(),
			_End=_Entity._Myaction_groups.end();_Iter!=_End;++_Iter) {
		
		_Action_group=(*_Iter); //Increase speed
		create_actiongroup(_Action_group->_Name,*_Action_group);
		}
	}
}

//Custom type callbacks

void entity::action_triggered(action *_Action,float _Data) {
	//Empty - override if needed
}

void entity::motion_triggered(motion *_Motion,float _Data) {
	//Empty - override if needed
}

//Action/motion callbacks

void entity::action_executed(action *_Action) {
	//Empty - override if needed
}

void entity::motion_updated(motion *_Motion) {
	//Empty - override if needed
}

void entity::motion_completed(motion *_Motion) {
	//Empty - override if needed
}

void entity::actiongroup_completed(action_group *_Action_group) {
	//Empty - override if needed
}

//Action/motion

void entity::execute_action(action *_Action) {

	//No action data
	if (_Action->_Data.empty()) return;

object *_Object=0;

	if (_Action->_Scope!=action::TREE) {
	
	_Object=object_pointer(_Action->_Object_name);
	
		//Object is required for BRANCH and LOCAL scope
		if (!_Object) return;
	}
	
	switch (_Action->_Type) {
	
		case action::TRANSLATE: {
		
		vector2 _Unit=_Action->_Data.data<vector2>();
		
			//Reverse unit
			if (_Action->_Owner->_Reverse) _Unit=-_Unit;
			
			if (_Action->_Scope==action::TREE)
				_Root_node->translate(_Unit);
			else if (_Action->_Scope==action::BRANCH)
				_Object->node()->translate(_Unit);
			else if (_Action->_Scope==action::LOCAL)
				_Object->node()->translate(_Unit);
		
		break;
		}
		
		case action::ROTATE: {
		
		float _Angle=_Action->_Data.data<float>();
		
			//Reverse angle
			if (_Action->_Owner->_Reverse) _Angle=-_Angle;
			
			if (_Action->_Scope==action::TREE)
				_Root_node->rotate(_Angle);
			else if (_Action->_Scope==action::BRANCH)
				_Object->node()->rotate(_Angle);
			else if (_Action->_Scope==action::LOCAL)
				_Object->node()->rotate(_Angle);
		
		break;
		}
		
		case action::SCALE: {
		
		vector2 _Scale=_Action->_Data.data<vector2>();
		
			//Reverse scale
			if (_Action->_Owner->_Reverse) _Scale=-_Scale;
			
			if (_Action->_Scope==action::TREE)
				_Root_node->scale(_Scale);
			else if (_Action->_Scope==action::BRANCH)
				_Object->node()->scale(_Scale);
			else if (_Action->_Scope==action::LOCAL)
				_Object->node()->scale(_Scale);
		
		break;
		}
		
		case action::POSITION: {
		
			if (_Action->_Scope==action::TREE)
				_Root_node->position(_Action->_Data.data<vector2>());
			else if (_Action->_Scope==action::BRANCH)
				_Object->node()->position(_Action->_Data.data<vector2>());
			else if (_Action->_Scope==action::LOCAL)
				_Object->node()->position(_Action->_Data.data<vector2>());
		
		break;
		}
		
		case action::ROTATION: {
		
			if (_Action->_Scope==action::TREE)
				_Root_node->rotation(_Action->_Data.data<float>());
			else if (_Action->_Scope==action::BRANCH)
				_Object->node()->rotation(_Action->_Data.data<float>());
			else if (_Action->_Scope==action::LOCAL)
				_Object->node()->rotation(_Action->_Data.data<float>());
		
		break;
		}
		
		case action::SCALING: {
		
			if (_Action->_Scope==action::TREE)
				_Root_node->scaling(_Action->_Data.data<vector2>());
			else if (_Action->_Scope==action::BRANCH)
				_Object->node()->scaling(_Action->_Data.data<vector2>());
			else if (_Action->_Scope==action::LOCAL)
				_Object->node()->scaling(_Action->_Data.data<vector2>());
		
		break;
		}
		
		case action::COLOR: {
		
			if (_Action->_Scope==action::TREE)
				color_channel(0,_Action->_Data.data<color>());
			else if (_Action->_Scope==action::BRANCH)
				color_channel(_Object,_Action->_Data.data<color>());
			else if (_Action->_Scope==action::LOCAL)
				_Object->color_channel(_Action->_Data.data<color>(),false);
		
		break;
		}
		
		case action::OPACITY: {
		
			if (_Action->_Scope==action::TREE)
				opacity(0,_Action->_Data.data<float>());
			else if (_Action->_Scope==action::BRANCH)
				opacity(_Object,_Action->_Data.data<float>());
			else if (_Action->_Scope==action::LOCAL)
				_Object->opacity(_Action->_Data.data<float>(),false);
		
		break;
		}
		
		case action::VISIBLE: {
		
			if (_Action->_Scope==action::TREE)
				visible(0,_Action->_Data.data<bool>());
			else if (_Action->_Scope==action::BRANCH)
				visible(_Object,_Action->_Data.data<bool>(),true);
			else if (_Action->_Scope==action::LOCAL)
				_Object->visible(_Action->_Data.data<bool>(),true);
		
		break;
		}
		
		case action::SOUND_ENABLED: {
		
			//Cannot reverse sound, skip
			if (_Action->_Owner->_Reverse) break;
			
			if (_Action->_Scope==action::TREE)
				sound_enabled(0,_Action->_Data.data<bool>());
			else if (_Action->_Scope==action::BRANCH)
				sound_enabled(_Object,_Action->_Data.data<bool>());
			else if (_Action->_Scope==action::LOCAL) {
			
				if (_Action->_Data.data<bool>())
					_Object->play(false);
				else _Object->stop(false);
			}
		
		break;
		}
		
		case action::SOUND_VOLUME: {
		
			if (_Action->_Scope==action::TREE)
				sound_volume(0,_Action->_Data.data<float>());
			else if (_Action->_Scope==action::BRANCH)
				sound_volume(_Object,_Action->_Data.data<float>());
			else if (_Action->_Scope==action::LOCAL)
				_Object->volume(_Action->_Data.data<float>(),false);
		
		break;
		}
		
		case action::LIGHT_ENABLED: {
		
			if (_Action->_Scope==action::TREE)
				light_enabled(0,_Action->_Data.data<bool>());
			else if (_Action->_Scope==action::BRANCH)
				light_enabled(_Object,_Action->_Data.data<bool>());
			else if (_Action->_Scope==action::LOCAL) {
			
			light *_Light=dynamic_cast<light*>(_Object);
			
				if (_Light) _Light->enabled(_Action->_Data.data<bool>());
			}
		
		break;
		}
		
		case action::LIGHT_BRIGHTNESS: {
		
			if (_Action->_Scope==action::TREE)
				light_brightness(0,_Action->_Data.data<float>());
			else if (_Action->_Scope==action::BRANCH)
				light_brightness(_Object,_Action->_Data.data<float>());
			else if (_Action->_Scope==action::LOCAL) {
			
			light *_Light=dynamic_cast<light*>(_Object);
			
				if (_Light) _Light->brightness(_Action->_Data.data<float>());
			}
		
		break;
		}

		case action::ANIMATION_ENABLED: {

		objects _Objects;

			if (_Action->_Scope==action::TREE)
				object_pointers(_Objects);
			else if (_Action->_Scope==action::BRANCH)
				object_pointers(_Object,_Objects);
			else if (_Action->_Scope==action::LOCAL)
				_Objects.push_back(_Object);
			
			for (objects::iterator _Iter=_Objects.begin(),
				_End=_Objects.end();_Iter!=_End;++_Iter) {
			
			object *_Obj=(*_Iter); //Increase speed

				for (object::components_iter _Citer=_Obj->components_begin(),
					_Cend=_Obj->components_end();_Citer!=_Cend;++_Citer) {
				
				animation *_Animation=dynamic_cast<animation*>((*_Citer)->data());

					if (_Animation) {

						if (_Action->_Data.data<bool>()) _Animation->start();
						else _Animation->stop();
					}
				}
			}
		
		break;
		}

		case action::PARTICLESYSTEM_ENABLED: {
		
		objects _Objects;

			if (_Action->_Scope==action::TREE)
				object_pointers(_Objects);
			else if (_Action->_Scope==action::BRANCH)
				object_pointers(_Object,_Objects);
			else if (_Action->_Scope==action::LOCAL)
				_Objects.push_back(_Object);
			
			for (objects::iterator _Iter=_Objects.begin(),
				_End=_Objects.end();_Iter!=_End;++_Iter) {
			
			object *_Obj=(*_Iter); //Increase speed

				for (object::components_iter _Citer=_Obj->components_begin(),
					_Cend=_Obj->components_end();_Citer!=_Cend;++_Citer) {
				
				particle_system *_Particle_system=dynamic_cast<particle_system*>((*_Citer)->data());

					if (_Particle_system) {

						if (_Action->_Data.data<bool>()) _Particle_system->start();
						else _Particle_system->stop();
					}
				}
			}
		
		break;
		}
		
		//Custom
		default:
		action_triggered(_Action,_Action->_Data.data<float>());
		break;
	}

action_executed(_Action); //Action executed
}

void entity::update_motion(motion *_Motion,float _Time) {

	//No motion data
	if (_Motion->_Data.empty() || _Motion->_Data2.empty()) return;

object *_Object=0;

	if (_Motion->_Scope!=action::TREE) {
	
	_Object=object_pointer(_Motion->_Object_name);
	
		//Object is required for PARENT and LOCAL scope
		if (!_Object) return;
	}

any_type _Result;

	switch (_Motion->_Type) {
	
		//bool (ignore technique)
		case action::VISIBLE:
		case action::SOUND_ENABLED:
		case action::LIGHT_ENABLED:
		case action::ANIMATION_ENABLED:
		case action::PARTICLESYSTEM_ENABLED:
		_Result=(_Motion->_Progress<=0.5f?_Motion->_Data.
			data<bool>():_Motion->_Data2.data<bool>());
		break;
		
		//color (ignore technique)
		case action::COLOR: {
		
			//Mix explicitly defined colors only (ZERO denotes base color)
			if (_Motion->_Data.data<color>()!=color::ZERO &&
				_Motion->_Data2.data<color>()!=color::ZERO)
					_Result=_Motion->_Data.data<color>().mix(
						_Motion->_Data2.data<color>(),_Motion->_Progress);
		
		break;
		}
		
		//vector2 (ignore technique)
		case action::TRANSLATE:
		case action::SCALE:
		_Result=(_Motion->_Data2.data<vector2>()*_Time)/
			_Motion->_Duration;
		break;
		
		//vector2
		case action::POSITION:
		case action::SCALING: {
		
			if (_Motion->_Technique==motion::LERP)
				_Result=_Motion->_Data.data<vector2>().lerp(
					_Motion->_Data2.data<vector2>(),_Motion->_Progress);
			else if (_Motion->_Technique==motion::SLERP)
				_Result=_Motion->_Data.data<vector2>().slerp(
					_Motion->_Data2.data<vector2>(),_Motion->_Progress);
		
		break;
		}
		
		//float (ignore technique)
		case action::ROTATE:
		_Result=(_Motion->_Data2.data<float>()*_Time)/
			_Motion->_Duration;
		break;
		
		//float
		default: {
		
			if (_Motion->_Technique==motion::LERP)
				_Result=math::lerp(_Motion->_Data.data<float>(),
					_Motion->_Data2.data<float>(),_Motion->_Progress);
			else if (_Motion->_Technique==motion::SLERP)
				_Result=math::slerp(_Motion->_Data.data<float>(),
					_Motion->_Data2.data<float>(),0.0f,_Motion->_Progress);
		
		break;
		}
	};
	
	switch (_Motion->_Type) {
	
		case action::TRANSLATE: {
		
			if (_Motion->_Scope==action::TREE)
				_Root_node->translate(_Result.data<vector2>());
			else if (_Motion->_Scope==action::BRANCH)
				_Object->node()->translate(_Result.data<vector2>());
			else if (_Motion->_Scope==action::LOCAL)
				_Object->node()->translate(_Result.data<vector2>());
		
		break;
		}
		
		case action::ROTATE: {
		
			if (_Motion->_Scope==action::TREE)
				_Root_node->rotate(_Result.data<float>());
			else if (_Motion->_Scope==action::BRANCH)
				_Object->node()->rotate(_Result.data<float>());
			else if (_Motion->_Scope==action::LOCAL)
				_Object->node()->rotate(_Result.data<float>());
		
		break;
		}
		
		case action::SCALE: {
		
			if (_Motion->_Scope==action::TREE)
				_Root_node->scale(_Result.data<vector2>());
			else if (_Motion->_Scope==action::BRANCH)
				_Object->node()->scale(_Result.data<vector2>());
			else if (_Motion->_Scope==action::LOCAL)
				_Object->node()->scale(_Result.data<vector2>());
		
		break;
		}
		
		case action::POSITION: {
		
			if (_Motion->_Scope==action::TREE)
				_Root_node->position(_Result.data<vector2>());
			else if (_Motion->_Scope==action::BRANCH)
				_Object->node()->position(_Result.data<vector2>());
			else if (_Motion->_Scope==action::LOCAL)
				_Object->node()->position(_Result.data<vector2>());
		
		break;
		}
		
		case action::ROTATION: {
		
			if (_Motion->_Scope==action::TREE)
				_Root_node->rotation(_Result.data<float>());
			else if (_Motion->_Scope==action::BRANCH)
				_Object->node()->rotation(_Result.data<float>());
			else if (_Motion->_Scope==action::LOCAL)
				_Object->node()->rotation(_Result.data<float>());
		
		break;
		}
		
		case action::SCALING: {
		
			if (_Motion->_Scope==action::TREE)
				_Root_node->scaling(_Result.data<vector2>());
			else if (_Motion->_Scope==action::BRANCH)
				_Object->node()->scaling(_Result.data<vector2>());
			else if (_Motion->_Scope==action::LOCAL)
				_Object->node()->scaling(_Result.data<vector2>());
		
		break;
		}
		
		case action::COLOR: {
		
			//Mix base color
			if (_Result.empty()) {
			
			bool _From_base=(_Motion->_Data.data<color>()==color::ZERO);
			bool _To_base=(_Motion->_Data2.data<color>()==color::ZERO);
			
				if (_Motion->_Scope==action::TREE) {
				
					if (_From_base && _To_base)
						color_channel(0, color::ZERO);
					else {
					
					object *_Myobject;
					component *_Component;
					
						//Change color channel for all objects
						for (_Branches::const_iterator _Iter=_Mybranches.begin(),
							_End=_Mybranches.end();_Iter!=_End;++_Iter) {
						
						_Myobject=(*_Iter)->_Object; //Increase speed
						
							//Change color channel for all components
							for (object::components_iter _Citer=_Myobject->components_begin(),
								_Cend=_Myobject->components_end();_Citer!=_Cend;++_Citer) {
							
							_Component=(*_Citer); //Increase speed
							const color &_Base=_Component->color_channel(true);
							
								if (_From_base)
									_Component->color_channel(_Base.mix(_Motion->_Data2.
										data<color>(),_Motion->_Progress),false);
								else _Component->color_channel(_Motion->_Data.
									data<color>().mix(_Base,_Motion->_Progress),false);
							}
						}
					}
				}
				else if (_Motion->_Scope==action::BRANCH) {
				
					if (_From_base && _To_base)
						color_channel(_Object,color::ZERO);
					else {
					
					objects _Objects;
					traverse(_Object,_Objects);
					
					object *_Myobject;
					component *_Component;
					
						//Change color channel for this object and its child objects
						for (objects::const_iterator _Iter=_Objects.begin(),
							_End=_Objects.end();_Iter!=_End;++_Iter) {
						
						_Myobject=(*_Iter); //Increase speed
						
							//Change color channel for all components
							for (object::components_iter _Citer=_Myobject->components_begin(),
								_Cend=_Myobject->components_end();_Citer!=_Cend;++_Citer) {
							
							_Component=(*_Citer); //Increase speed
							const color &_Base=_Component->color_channel(true);
							
								if (_From_base)
									_Component->color_channel(_Base.mix(_Motion->_Data2.
										data<color>(),_Motion->_Progress),false);
								else _Component->color_channel(_Motion->_Data.
									data<color>().mix(_Base,_Motion->_Progress),false);
							}
						}
					}
				}
				else if (_Motion->_Scope==action::LOCAL) {
				
					if (_From_base && _To_base)
						_Object->color_channel(color::ZERO,false);
					else {
					
					component *_Component;
					
						//Change color channel for all components
						for (object::components_iter _Iter=_Object->components_begin(),
							_End=_Object->components_end();_Iter!=_End;++_Iter) {
						
						_Component=(*_Iter); //Increase speed
						const color &_Base=_Component->color_channel(true);
						
							if (_From_base)
								_Component->color_channel(_Base.mix(_Motion->_Data2.
									data<color>(),_Motion->_Progress),false);
							else _Component->color_channel(_Motion->_Data.
								data<color>().mix(_Base,_Motion->_Progress),false);
						}
					}
				}
			}
			//Explicit
			else {
			
				if (_Motion->_Scope==action::TREE)
					color_channel(0,_Result.data<color>());
				else if (_Motion->_Scope==action::BRANCH)
					color_channel(_Object,_Result.data<color>());
				else if (_Motion->_Scope==action::LOCAL)
					_Object->color_channel(_Result.data<color>(),false);
			}
		
		break;
		}
		
		case action::OPACITY: {
		
			if (_Motion->_Scope==action::TREE)
				opacity(0,_Result.data<float>());
			else if (_Motion->_Scope==action::BRANCH)
				opacity(_Object,_Result.data<float>());
			else if (_Motion->_Scope==action::LOCAL)
				_Object->opacity(_Result.data<float>(),false);
		
		break;
		}
		
		case action::VISIBLE: {
		
			if (_Motion->_Scope==action::TREE)
				visible(0,_Result.data<bool>());
			else if (_Motion->_Scope==action::BRANCH)
				visible(_Object,_Result.data<bool>(),true);
			else if (_Motion->_Scope==action::LOCAL)
				_Object->visible(_Result.data<bool>(),true);
		
		break;
		}
		
		case action::SOUND_ENABLED: {
		
			//Cannot reverse sound, skip
			if (_Motion->_Owner->_Reverse) break;
			
			if (_Motion->_Scope==action::TREE)
				sound_enabled(0,_Result.data<bool>());
			else if (_Motion->_Scope==action::BRANCH)
				sound_enabled(_Object,_Result.data<bool>());
			else if (_Motion->_Scope==action::LOCAL) {
			
				if (_Result.data<bool>())
					_Object->play(false);
				else _Object->stop(false);
			}
		
		break;
		}
		
		case action::SOUND_VOLUME: {
		
			if (_Motion->_Scope==action::TREE)
				sound_volume(0,_Result.data<float>());
			else if (_Motion->_Scope==action::BRANCH)
				sound_volume(_Object,_Result.data<float>());
			else if (_Motion->_Scope==action::LOCAL)
				_Object->volume(_Result.data<float>(),false);
		
		break;
		}
		
		case action::LIGHT_ENABLED: {
		
			if (_Motion->_Scope==action::TREE)
				light_enabled(0,_Result.data<bool>());
			else if (_Motion->_Scope==action::BRANCH)
				light_enabled(_Object,_Result.data<bool>());
			else if (_Motion->_Scope==action::LOCAL) {
			
			light *_Light=dynamic_cast<light*>(_Object);
			
				if (_Light) _Light->enabled(_Result.data<bool>());
			}
		
		break;
		}
		
		case action::LIGHT_BRIGHTNESS: {
		
			if (_Motion->_Scope==action::TREE)
				light_brightness(0,_Result.data<float>());
			else if (_Motion->_Scope==action::BRANCH)
				light_brightness(_Object,_Result.data<float>());
			else if (_Motion->_Scope==action::LOCAL) {
			
			light *_Light=dynamic_cast<light*>(_Object);
			
				if (_Light) _Light->brightness(_Result.data<float>());
			}
		
		break;
		}

		case action::ANIMATION_ENABLED: {

		objects _Objects;

			if (_Motion->_Scope==action::TREE)
				object_pointers(_Objects);
			else if (_Motion->_Scope==action::BRANCH)
				object_pointers(_Object,_Objects);
			else if (_Motion->_Scope==action::LOCAL)
				_Objects.push_back(_Object);
			
			for (objects::iterator _Iter=_Objects.begin(),
				_End=_Objects.end();_Iter!=_End;++_Iter) {
			
			object *_Obj=(*_Iter); //Increase speed

				for (object::components_iter _Citer=_Obj->components_begin(),
					_Cend=_Obj->components_end();_Citer!=_Cend;++_Citer) {
				
				animation *_Animation=dynamic_cast<animation*>((*_Citer)->data());

					if (_Animation) {

						if (_Result.data<bool>()) _Animation->start();
						else _Animation->stop();
					}
				}
			}
		
		break;
		}

		case action::PARTICLESYSTEM_ENABLED: {
		
		objects _Objects;

			if (_Motion->_Scope==action::TREE)
				object_pointers(_Objects);
			else if (_Motion->_Scope==action::BRANCH)
				object_pointers(_Object,_Objects);
			else if (_Motion->_Scope==action::LOCAL)
				_Objects.push_back(_Object);
			
			for (objects::iterator _Iter=_Objects.begin(),
				_End=_Objects.end();_Iter!=_End;++_Iter) {
			
			object *_Obj=(*_Iter); //Increase speed

				for (object::components_iter _Citer=_Obj->components_begin(),
					_Cend=_Obj->components_end();_Citer!=_Cend;++_Citer) {
				
				particle_system *_Particle_system=dynamic_cast<particle_system*>((*_Citer)->data());

					if (_Particle_system) {

						if (_Result.data<bool>()) _Particle_system->start();
						else _Particle_system->stop();
					}
				}
			}
		
		break;
		}
		
		//Custom
		default:
		motion_triggered(_Motion,_Result.data<float>());
		break;
	}

motion_updated(_Motion); //Motion updated

	//Motion completed
	if (_Motion->_Progress==1.0f)
		motion_completed(_Motion);
}

float entity::update_actiongroup(action_group *_Action_group,float _Time) {

	//Subtract time
	if (_Action_group->_Reverse) _Time=-_Time;

float _Stop=0.0f;
action *_Action;

	for (action_group::_Actions::const_iterator _Iter=_Action_group->
		_Myactions.begin(),_End=_Action_group->_Myactions.end();
			_Iter!=_End;++_Iter) {
	
	_Action=(*_Iter); //Increase speed
	
	float _Start=_Action_group->_Start+_Action->_Start;
	motion *_Motion=dynamic_cast<motion*>(_Action);
	
		if (_Motion) {
		
			//Update stop point
			if (_Start+_Motion->_Duration>_Stop)
				_Stop=_Start+_Motion->_Duration;
		}
		else {
		
			//Update stop point
			if (_Start>_Stop) _Stop=_Start;
		}
		
		//Nothing more to do
		if (!_Action_group->_Enabled || !_Action->_Enabled) continue;
		
		if (_Motion) {
		
			//Update motion
			if (_Action_group->_Time+_Time>=_Start &&
				_Action_group->_Time+_Time<=_Start+_Motion->_Duration) {
			
			_Motion->_Progress=(_Action_group->_Time+_Time-_Start)/
				_Motion->_Duration;
			update_motion(_Motion,_Time);
			}
			else {
			
				if (_Action_group->_Reverse) {
				
					//Not fully reversed
					if (_Action_group->_Time>=_Start &&
						_Action_group->_Time+_Time<=_Start) {
					
					_Motion->_Progress=0.0f;
					update_motion(_Motion,_Start-_Action_group->_Time);
					}
				}
				//Not fully completed
				else if (_Action_group->_Time<=_Start+_Motion->_Duration &&
					_Action_group->_Time+_Time>=_Start+_Motion->_Duration) {
				
				_Motion->_Progress=1.0f;
				update_motion(_Motion,_Start+_Motion->_Duration-
					_Action_group->_Time);
				}
			}
		}
		else {
		
			if (_Action_group->_Reverse) {
			
				//Execute action (reversed)
				if (_Action_group->_Time>=_Start &&
					_Action_group->_Time+_Time<=_Start)
						execute_action(_Action);
			}
			//Execute action
			else if (_Action_group->_Time<=_Start &&
				_Action_group->_Time+_Time>=_Start)
					execute_action(_Action);
		}
	}
	
	//Update time
	if (_Action_group->_Enabled) {
	
	_Action_group->_Time+=_Time;
	
		if (_Action_group->_Reverse) {
		
			if (_Action_group->_Time<=0.0f) {
			
			_Action_group->_Time=0.0f;
			_Action_group->_Reverse=false;
			_Action_group->_Enabled=false;
			}
		}
		else {
		
			//Stop action group when stop point is reached
			if (_Action_group->_Time>=_Stop)
				stop_actiongroup(_Action_group);
			
			//Sync time
			if (_Action_group->_Automatic)
				_Action_group->_Time=this->_Time;
		}
	}

return _Stop;
}

void entity::stop_actiongroup(action_group *_Action_group) {

_Action_group->_Time=0.0f; //Reset time

	//Finite loop
	if (_Action_group->_Current_loop>0) {
	
	--_Action_group->_Current_loop;
	
		//Action group is finish
		if (_Action_group->_Current_loop==0) {
		
		_Action_group->reset_loop(); //Reset loop
		_Action_group->_Enabled=false; //Disable action group
		}
	}

actiongroup_completed(_Action_group);
}

void entity::update_actiongroups(float _Time) {

float _Stop=0.0f;
action_group *_Action_group;

	for (_Action_groups::const_iterator _Iter=_Myaction_groups.begin(),
		_End=_Myaction_groups.end();_Iter!=_End;++_Iter) {
	
	_Action_group=(*_Iter); //Increase speed
	
		if (_Action_group->_Enabled || _Action_group->_Automatic) {
		
		float _Group_stop=update_actiongroup(_Action_group,_Time);
		
			if (_Action_group->_Automatic && _Group_stop>_Stop)
				_Stop=_Group_stop;
		}
	}
	
	//Update and reset time
	if ((this->_Time+=_Time)>=_Stop) this->_Time=0.0f;
}

//Public

entity &entity::operator=(const entity &_Entity) {

	//Worth doing
	if (this!=&_Entity) {
	
	clear_actiongroups();
	
	static_entity::operator=(_Entity); //Invoke base operator=
	copy(_Entity);
	}

return *this;
}

void entity::update(float _Time) {

	//Update action groups
	if (!_Myaction_groups.empty()) update_actiongroups(_Time);
	//Reset time
	else this->_Time=0.0f;

static_entity::update(_Time); //Use base functionality
}

void entity::append(const entity &_Entity) {

static_entity::append(_Entity);

	if (!_Entity._Myaction_groups.empty()) {
	
	action_group *_Action_group;
	
		for (_Action_groups::const_iterator _Iter=_Entity._Myaction_groups.begin(),
			_End=_Entity._Myaction_groups.end();_Iter!=_End;++_Iter) {
		
		_Action_group=(*_Iter); //Increase speed
		create_actiongroup(_Action_group->_Name,*_Action_group);
		}
	}
}

//Action groups

entity::action_group *entity::create_actiongroup(const std::string &_Name,
	bool _Enabled) {

	//Action group already exist
	if (actiongroup_pointer(_Name)) return 0;

action_group *_Myaction_group=new action_group(_Name,_Enabled);
_Myaction_groups.push_back(_Myaction_group);
return _Myaction_group;
}

entity::action_group *entity::create_actiongroup(const std::string &_Name,
	const action_group &_Action_group) {

	//Action group already exist
	if (actiongroup_pointer(_Name)) return 0;

action_group *_Myaction_group=new action_group(_Name,_Action_group);
_Myaction_groups.push_back(_Myaction_group);
return _Myaction_group;
}

void entity::remove_actiongroup(action_group *&_Action_group) {

	for (_Action_groups::iterator _Iter=_Myaction_groups.begin(),
		_End=_Myaction_groups.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Action_group) {
		
		delete _Action_group;
		_Action_group=0; //Set referenced pointer to null
		
		_Myaction_groups.erase(_Iter);
		return;
		}
	}
}

void entity::clear_actiongroups() {

	for (_Action_groups::const_iterator _Iter=_Myaction_groups.begin(),
		_End=_Myaction_groups.end();_Iter!=_End;++_Iter)
			delete (*_Iter);

_Myaction_groups.clear();
}

entity::actiongroups_iter entity::actiongroups_begin() const {
	return _Myaction_groups.begin();
}

entity::actiongroups_iter entity::actiongroups_end() const {
	return _Myaction_groups.end();
}

entity::action_group *entity::actiongroup_pointer(const std::string &_Name) const {

	for (_Action_groups::const_iterator _Iter=_Myaction_groups.begin(),
		_End=_Myaction_groups.end();_Iter!=_End;++_Iter) {
	
		//Action group exist, return pointer
		if ((*_Iter)->_Name==_Name) return (*_Iter);
	}

return 0;
}