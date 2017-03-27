/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: entity.h
---------------------------------------------------
*/

#ifndef _ENTITY_
#define _ENTITY_

//Dependencies (entity)
#include "static_entity.h"
#include "any_type.h"

class entity:public static_entity {

	friend entity_manager;

public:

	//Forward declarations (entity)
	struct action_group;
	
	struct action {
	
		friend entity;
		friend action_group;
	
	public:
	
		enum TYPE { TRANSLATE, ROTATE, SCALE,					//Relative transformations
					POSITION, ROTATION, SCALING,				//Absolute transformations
					COLOR, OPACITY, VISIBLE,					//Visuals
					SOUND_ENABLED, SOUND_VOLUME,				//Sound specific
					LIGHT_ENABLED, LIGHT_BRIGHTNESS,			//Light specific
					ANIMATION_ENABLED, PARTICLESYSTEM_ENABLED,	//Component specific
					CUSTOM };									//User callback
		
		enum SCOPE { TREE,		//All objects in entity
					 BRANCH,	//An object and its child objects
					 LOCAL };	//One specific object
	
	protected:
	
		action_group *_Owner;
		TYPE _Type;
		SCOPE _Scope;
		float _Start;
		bool _Enabled;
		
		std::string _Object_name;
		any_type _Data;
		
		//Protected constructor
		explicit action(action_group *_Owner,bool _Enabled=true);
		
		//Protected copy constructor
		action(const action &_Action);
		
		//Protected alternative copy constructor
		action(action_group *_Owner,const action &_Action);
		
		//Protected virtual destructor
		virtual ~action();
		
		void copy(const action &_Action);
	
	public:
	
		action &operator=(const action &_Action);

		action_group *owner() const;
		TYPE type() const;
		
		void scope(const SCOPE &_Scope);
		void scope(const SCOPE &_Scope,const std::string &_Object_name);
		SCOPE scope() const;
		
		void start(float _Start);
		float start() const;
		
		void enabled(bool _Enabled);
		bool enabled() const;

		const any_type &data() const;
		
		//Type/data
		
		virtual void translate(const vector2 &_Translate);
		virtual void rotate(float _Rotate);
		virtual void scale(const vector2 &_Scale);
		
		void position(const vector2 &_Position);
		void rotation(float _Rotation);
		void scaling(const vector2 &_Scaling);
		
		void color_channel(const color &_Color);
		void opacity(float _Opacity);
		void visible(bool _Visible);
		
		void sound_enabled(bool _Enabled);
		void sound_volume(float _Volume);
		
		void light_enabled(bool _Enabled);
		void light_brightness(float _Brightness);

		void animation_enabled(bool _Enabled);
		void particlesystem_enabled(bool _Enabled);
		
		void custom(float _Value);
	};
	
	struct motion:public action {
	
		friend entity;
		friend action_group;
	
	public:
	
		enum TECHNIQUE { LERP, SLERP };
	
	protected:
	
		TECHNIQUE _Technique;
		float _Duration;
		float _Progress;
		
		any_type _Data2;
		
		//Protected constructor
		explicit motion(action_group *_Owner,bool _Enabled=true);
		
		//Protected copy constructor
		motion(const motion &_Motion);
		
		//Protected alternative copy constructor
		motion(action_group *_Owner,const motion &_Motion);
		
		//Protected virtual destructor
		virtual ~motion();
		
		void copy(const motion &_Motion);
	
	public:
	
		motion &operator=(const motion &_Motion);
		
		void technique(const TECHNIQUE &_Technique);
		TECHNIQUE technique() const;
		
		void duration(float _Duration);
		float duration() const;
		float progress() const;

		const any_type &data2() const;
		
		//Type/data
		
		virtual void translate(const vector2 &_To);
		virtual void rotate(float _To);
		virtual void scale(const vector2 &_To);
		
		void position(const vector2 &_From,const vector2 &_To);
		void rotation(float _From,float _To);
		void scaling(const vector2 &_From,const vector2 &_To);
		
		void color_channel(const color &_From,const color &_To);
		void opacity(float _From,float _To);
		void visible(bool _From,bool _To);
		
		void sound_enabled(bool _From,bool _To);
		void sound_volume(float _From,float _To);
		
		void light_enabled(bool _From,bool _To);
		void light_brightness(float _From,float _To);

		void animation_enabled(bool _From,bool _To);
		void particlesystem_enabled(bool _From,bool _To);
		
		void custom(float _From,float _To);
	};
	
	struct action_group {
	
		friend entity;
	
	private:
	
		typedef std::list<action*> _Actions;
		
		std::string _Name;
		float _Time;
		float _Start;
		bool _Enabled;
		bool _Automatic;
		
		unsigned int _Loop;
		unsigned int _Current_loop;
		bool _Reverse;
		
		_Actions _Myactions;
		
		//Private constructor
		explicit action_group(const std::string &_Name,bool _Enabled=true);
		
		//Private copy constructor
		action_group(const action_group &_Action_group);
		
		//Private alternative copy constructor
		action_group(const std::string &_Name,const action_group &_Action_group);
		
		//Private destructor
		~action_group();
		
		void copy(const action_group &_Action_group);
	
	public:
	
		typedef _Actions::const_iterator actions_iter; //Keep data safe
		
		action_group &operator=(const action_group &_Action_group);
		
		const std::string &name() const;
		
		void start(float _Start);
		float start() const;
		
		void enabled(bool _Enabled);
		bool enabled() const;
		
		void automatic(bool _Automatic);
		bool automatic() const;
		
		void loop(unsigned int _Loop);
		unsigned int loop() const;
		
		void reset_loop();
		void reverse();
		
		//Actions/motions
		
		action *create_action(bool _Enabled=true);
		action *create_action(const action &_Action);
		
		motion *create_motion(bool _Enabled=true);
		motion *create_motion(const motion &_Motion);
		
		void remove_action(action *&_Action);
		void clear_actions();
		
		actions_iter actions_begin() const;
		actions_iter actions_end() const;
	};

protected:

	typedef std::list<action_group*> _Action_groups;
	
	float _Time;
	_Action_groups _Myaction_groups;
	
	//Protected constructor
	entity(entity_manager *_Owner,render_node *_Parent_node,
		const interval &_Draw_order,bool _Visible=true);
	
	//Protected copy constructor
	entity(const entity &_Entity);
	
	//Protected alternative copy constructor
	entity(entity_manager *_Owner,render_node *_Parent_node,
		const interval &_Draw_order,const entity &_Entity);
	
	//Protected virtual destructor
	virtual ~entity();
	
	void copy(const entity &_Entity);
	
	//Custom type callbacks
	
	virtual void action_triggered(action *_Action,float _Data);
	virtual void motion_triggered(motion *_Motion,float _Data);
	
	//Action/motion callbacks
	
	virtual void action_executed(action *_Action);
	
	virtual void motion_updated(motion *_Motion);
	virtual void motion_completed(motion *_Motion);
	
	virtual void actiongroup_completed(action_group *_Action_group);
	
	//Action/motion
	
	void execute_action(action *_Action);
	void update_motion(motion *_Motion,float _Time);
	
	float update_actiongroup(action_group *_Action_group,float _Time);
	void stop_actiongroup(action_group *_Action_group);
	
	void update_actiongroups(float _Time);

public:

	typedef _Action_groups::const_iterator actiongroups_iter; //Keep data safe
	
	entity &operator=(const entity &_Entity);
	
	virtual void update(float _Time);

	void append(const entity &_Entity);
	
	//Action groups
	
	action_group *create_actiongroup(const std::string &_Name,
		bool _Enabled=true);
	action_group *create_actiongroup(const std::string &_Name,
		const action_group &_Action_group);
	
	void remove_actiongroup(action_group *&_Action_group);
	void clear_actiongroups();
	
	actiongroups_iter actiongroups_begin() const;
	actiongroups_iter actiongroups_end() const;
	
	action_group *actiongroup_pointer(const std::string &_Name) const;
};

#endif