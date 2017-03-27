/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: script_parser.cpp
---------------------------------------------------
*/

#include "script_parser.h"

//Dependencies
#include <map>
#include <stack>
#include <algorithm>
#include <utility>

#include "file_manager.h"

namespace lunar {

//Public

//Result
//Private

void script_parser::result::copy(const result &_Result) {

_Type=_Result._Type;
_Merge=_Result._Merge;
_Script_name=_Result._Script_name;

_Myobjects=_Result._Myobjects;
}

//Result
//Public

script_parser::result::result(const TYPE &_Type,bool _Merge):
	_Type(_Type),_Merge(_Merge) {
	//Empty
}

script_parser::result::result(const result &_Result) {
	copy(_Result);
}

script_parser::result::~result() {
	//Empty
}

script_parser::result &script_parser::result::operator=(
	const result &_Result) {

	//Worth doing
	if (this!=&_Result) copy(_Result);

return *this;
}

script_parser::result::TYPE script_parser::result::type() const {
	return _Type;
}

bool script_parser::result::merge() const {
	return _Merge;
}

const std::string &script_parser::result::script_name() const {
	return _Script_name;
}

script_class::object::objects_iterator script_parser::result::
	objects_begin() const {
	return _Myobjects.begin();
}

script_class::object::objects_iterator script_parser::result::
	objects_end() const {
	return _Myobjects.end();
}

//Parser
//Private

void script_parser::copy(const script_parser &_Script_parser) {
	_Script_container=_Script_parser._Script_container;
}

bool script_parser::parse(std::string _Script_data,result &_Result) const {

//----------------------------
//Process and replace includes
//----------------------------

//To trace circular include references
typedef std::vector<std::pair<std::string,size_t>> _Includes;

const std::string _Include_tag("#include ");
_Includes _Current_includes;

	//Check all include statements
	for (size_t _Off=0;(_Off=_Script_data.find(
		_Include_tag,_Off))!=std::string::npos;) {
	
		//Done tracing include reference(s)
		while (!_Current_includes.empty() &&
			_Current_includes.back().second<=_Off)
				_Current_includes.pop_back();
	
	size_t _Off2=_Off+_Include_tag.size();
	
		//Check if include statement is valid
		//Shortest include path possible "x" or <x>
		if (_Off2+2<_Script_data.size() &&
			(_Script_data[_Off2]=='"' ||
				_Script_data[_Off2]=='<')) {
		
		size_t _Off3=_Script_data.find(
			(_Script_data[_Off2]=='<'?'>':'"'),_Off2+1);
		++_Off2;
		
			//Found end of include
			if (_Off3!=std::string::npos) {
			
				//Found include name
				if (_Off2<_Off3) {
				
				std::string _Include_name=_Script_data.
					substr(_Off2,_Off3-_Off2);
				
				//Self circular?
				bool _Circular=(_Include_name==_Result._Script_name);
				
					if (!_Circular) {
					
						//Check for circular include reference(s)
						for (_Includes::const_iterator _Iter=_Current_includes.begin(),
							_End=_Current_includes.end();_Iter!=_End;++_Iter) {
						
							//Found a circular reference
							if ((*_Iter).first==_Include_name) {
							
							_Circular=true;
							break;
							}
						}
					}
					
					if (!_Circular) {
					
					std::string _Data;
					
					//Check if include name is a file
					file_data *_File_data=static_cast<file_data*>(
						file_container::instance()->data(_Include_name));
					
						//Found file resource, of try find file directly
						if (_File_data || file_manager::load_binary(
							_Include_name,_Data)) {
						
							//Load include data from data file
							if (_File_data)
								_Script_container->load_bytes(_File_data,_Data);
							
							//Load include data from file (filename)
							//Replace include statement with file data
							if (!_Data.empty() || (_File_data && file_manager::
								load_binary(_File_data->filename(),_Data))) {
							
							size_t _Size=_Off3-_Off+1;
							_Script_data.replace(_Off,_Size,_Data);
							
								//Update parent include range
								for (_Includes::iterator _Iter=_Current_includes.begin(),
									_End=_Current_includes.end();_Iter!=_End;++_Iter) {
								
									//Increase range
									if (_Data.size()>=_Size)
										(*_Iter).second+=_Data.size()-_Size;
									//Decrease range
									else (*_Iter).second-=_Size-_Data.size();
								}
							
							//Trace include reference
							_Current_includes.push_back(std::pair<std::string,size_t>(
								_Include_name,_Off+_Data.size()));
							continue;
							}
						}
					}
				}
			
			_Off=_Off3+1;
			continue;
			}
		}
	
	_Off=_Off2;
	}

//--------------------
//Tokenize script data
//--------------------

bool _Literal=false;

	//Change special characters (outside string literals) to unit separators
	for (size_t i=0;i<_Script_data.size();++i) {
	
		if (_Script_data[i]<=' ') {
		
			if (!_Literal) _Script_data[i]=' '-1; 
		}
		else if (_Script_data[i]=='\\') {
		
			if (_Script_data[i+1]=='\\' || _Script_data[i+1]=='"') {
			
			_Script_data.erase(i,1);
			continue;
			}
		
		_Script_data.erase(i--,1);
		}
		else if (_Script_data[i]=='"') _Literal=!_Literal;
	}

string_utility::tokens _Tokens;
string_utility::tokenize(_Script_data,"\x1F",_Tokens);

//---------------------------------------------------
//Build objects and extract property data from tokens
//---------------------------------------------------

typedef std::stack<std::pair<script_class*,script_class::object*>> _Objects;

typedef std::list<script_class::object> _Template;
typedef std::map<std::string,std::pair<std::string,_Template>> _Templates;

typedef std::list<std::pair<script_class*,script_class::_Classes>> _Compiled_classes;
typedef std::list<std::pair<script_class*,script_class::_Scopes>> _Compiled_scopes;
typedef std::list<std::pair<script_class*,script_class::_Properties>> _Compiled_properties;

_Objects _Myobjects;
_Templates _Mytemplates;

_Compiled_classes _Mycompiled_classes;
_Compiled_scopes _Mycompiled_scopes;
_Compiled_properties _Mycompiled_properties;

script_class *_Current_class=_Global_class;
script_class::object *_Current_object=0;
unsigned int _Comment_block=0;

script_scope *_Scope;
script_class *_Class;
script_property *_Property;

	for (size_t i=0;i<_Tokens.size();++i) {
	
		//Comment begin (bracket is not bounded to a class or property)
		if (_Tokens[i]=="{" || _Tokens[i]=="/*") ++_Comment_block;
		else if (_Tokens[i]=="}" || _Tokens[i]=="*/") {
		
			//Comment end
			if (_Comment_block>0) --_Comment_block;
			
			//Check if object is out of scope
			else if (!_Myobjects.empty()) {
			
				//Look for default template
				if (!_Current_object->_Templated && !_Mytemplates.empty()) {
				
				_Templates::iterator _Find=_Mytemplates.find(
					_Current_object->_Class_name);
				
					//Check if a default template definition exists
					if (_Find!=_Mytemplates.end()) {
					
					_Current_object->append_front((*_Find).second.second);
					_Current_object->_Templated=true; //Template inserted
					}
				}
			
			_Myobjects.pop();
			
				if (_Myobjects.empty()) {
				
				_Current_class=_Global_class;
				_Current_object=0;
				}
				else {
				
				_Current_class=_Myobjects.top().first;
				_Current_object=_Myobjects.top().second;
				}
			}
		
		continue;
		}
		
		//Skip token inside comment block
		if (_Comment_block>0) continue;
	
	size_t _Objects_size=_Myobjects.size();
	
		//Check for templates (global scope only)
		if (_Myobjects.empty() && _Tokens[i].size()>1 &&
			_Tokens[i][0]=='.') {
		
		std::string _Template_class;
		std::string _Template_name;
		std::string _Template_inherit;
		
		size_t _Name_index=_Tokens[i].find_last_of('.');
		size_t _Inherit_index=_Tokens[i].find_last_of(':');
		
			//Found :
			if (_Inherit_index!=std::string::npos) {
			
				//#a.b:inherit
				if (_Name_index!=std::string::npos &&
					_Inherit_index>_Name_index+1 &&
						_Inherit_index+1<_Tokens[i].size()) {
				
				_Template_class=_Tokens[i].substr(1,_Name_index-1);
				_Template_name=_Tokens[i].substr(1,_Inherit_index-1);
				_Template_inherit=_Tokens[i].substr(_Inherit_index+1);
				}
			}
			//Found .
			else if (_Name_index!=std::string::npos) {
			
				//#a.b
				if (_Name_index+1<_Tokens[i].size()) {
				
				_Template_class=_Tokens[i].substr(1,_Name_index-1);
				_Template_name=_Tokens[i].substr(1);
				}
			}
			//Default template
			else {
			
			_Template_class=_Tokens[i].substr(1);
			_Template_name=_Template_class;
			}
			
			//Found template class
			if (!_Template_class.empty()) {
			
				//Check if template class exists (ignore scopes)
				for (_Classes::const_iterator _Iter=_Myclasses.begin(),
					_End=_Myclasses.end();_Iter!=_End;++_Iter) {
				
				_Class=(*_Iter); //Increase speed
				
					//Check if class name equals template class
					if (_Class->_Name==_Template_class) {
					
						//Requires consecutive token
						if (i+1<_Tokens.size() && _Tokens[i+1]=="{")
							++i; //Skip token
						else break;
					
					_Template &_Template_objects=(_Mytemplates[_Template_name]=
						std::pair<std::string,_Template>(_Template_class,
							_Template())).second;
					
						//Inherit from another template in the same class
						if (!_Template_inherit.empty()) {

						script_class::_Classes *_All_classes=0;
						
							//Look for compiled classes for this class
							for (_Compiled_classes::iterator _Iter=_Mycompiled_classes.begin(),
								_End=_Mycompiled_classes.end();_Iter!=_End;++_Iter) {

								if ((*_Iter).first==_Current_class) {

								_All_classes=&(*_Iter).second;
								break;
								}
							}

							//Compile classes
							if (!_All_classes) {

							_Mycompiled_classes.push_back(std::pair<script_class*,script_class::
								_Classes>(_Current_class,script_class::_Classes()));
							_All_classes=&_Mycompiled_classes.back().second;

							compile_classes(_Current_class,*_All_classes);
							}

							for (_Classes::iterator _Iter=_All_classes->begin(),
								_End=_All_classes->end();_Iter!=_End;++_Iter) {
			
							const std::string &_Class_name = (*_Iter)->_Name; //Increase

							_Templates::iterator _Find=_Mytemplates.find(
								_Template_inherit=="default"?_Class_name:
									_Class_name+"."+_Template_inherit);
						
								//Check if template definition exists
								if (_Find!=_Mytemplates.end()) {
							
								const _Template &_Mytemplate=(*_Find).second.second;
							
									for (_Template::const_iterator _Oiter=_Mytemplate.begin(),
										_Oend=_Mytemplate.end();_Oiter!=_Oend;++_Oiter)
											_Template_objects.push_back(*_Oiter);
								}
							}
						}
					
					script_class::object _Object;
					_Object._Class_name=_Class->_Name;
					_Object._Name=_Template_name;
					_Object._Templated=true;
					_Template_objects.push_back(_Object);
					
					//Add template to stack
					_Myobjects.push(std::pair<script_class*,script_class::object*>(
						_Current_class=_Class,_Current_object=&_Template_objects.back()));
					break;
					}
				}
			}
		}
		
		//Template added, continue to next token
		if (_Objects_size<_Myobjects.size()) continue;

	script_class::_Scopes *_All_scopes=0;
			
		//Look for compiled scopes for this class
		for (_Compiled_scopes::iterator _Iter=_Mycompiled_scopes.begin(),
			_End=_Mycompiled_scopes.end();_Iter!=_End;++_Iter) {

			if ((*_Iter).first==_Current_class) {

			_All_scopes=&(*_Iter).second;
			break;
			}
		}

		//Compile scopes
		if (!_All_scopes) {

		_Mycompiled_scopes.push_back(std::pair<script_class*,script_class::
			_Scopes>(_Current_class,script_class::_Scopes()));
		_All_scopes=&_Mycompiled_scopes.back().second;

		compile_scopes(_Current_class,*_All_scopes);
		}
		
		for (script_class::_Scopes::const_iterator _Iter=_All_scopes->begin(),
			_End=_All_scopes->end();_Iter!=_End;++_Iter) {
		
		_Scope=(*_Iter); //Increase speed
		
			for (_Classes::const_iterator _Citer=_Scope->
				_Myregistered_classes.begin(),_Cend=_Scope->
					_Myregistered_classes.end();_Citer!=_Cend;++_Citer) {
			
			_Class=(*_Citer); //Increase speed
			
				//Check if class name equals token
				if (_Class->_Name==_Tokens[i]) {
				
				bool _Create_object=false;
				std::string _Name;
				
					//Requires consecutive token
					if (i+1<_Tokens.size() && _Tokens[i+1]=="{") {
					
					_Create_object=true;
					++i; //Skip token
					}
					//Requires consecutive tokens
					else if (i+2<_Tokens.size() && _Tokens[i+2]=="{") {
					
						//Check if name is inside string literals
						if (_Tokens[i+1].size()>1 && _Tokens[i+1][0]=='"' &&
							_Tokens[i+1][_Tokens[i+1].size()-1]=='"')
								_Name=_Tokens[i+1].substr(1,_Tokens[i+1].size()-2);
						
						//Use token directly as name
						else _Name=_Tokens[i+1];
					
					_Create_object=true;
					i+=2; //Skip tokens
					}
					
					//Create an object of this class
					if (_Create_object) {
					
					script_class::object _Object;
					_Object._Class_name=_Class->_Name;
					_Object._Name=_Name;
					
					_Object._Tag=_Class->_Tag;
					_Object._Depth=(unsigned int)_Myobjects.size();
					
						if (_Object._Depth>0)
							_Object._Parent=_Myobjects.top().first->_Name;
						
						if (_Current_object) {
						
						_Current_object->_Myobjects.push_back(_Object);
						_Current_object=&_Current_object->_Myobjects.back();
						}
						else {
						
						_Result._Myobjects.push_back(_Object);
						_Current_object=&_Result._Myobjects.back();
						}
					
					//Add object to stack
					_Myobjects.push(std::pair<script_class*,script_class::object*>(
						_Current_class=_Class,_Current_object));
					}
				
				break;
				}
			}
			
			//New object added
			if (_Objects_size<_Myobjects.size()) break;
		}
		
		//Don't check for templates or properties, continue to next token
		if (!_Current_object || _Objects_size<_Myobjects.size()) continue;
		
		//Look for templates
		if (_Tokens[i].size()>1 && _Tokens[i][0]=='.') {
		
		std::string _Template_name=_Tokens[i].substr(1);

		script_class::_Classes *_All_classes=0;
		
			//Look for compiled classes for this class
			for (_Compiled_classes::iterator _Iter=_Mycompiled_classes.begin(),
				_End=_Mycompiled_classes.end();_Iter!=_End;++_Iter) {

				if ((*_Iter).first==_Current_class) {

				_All_classes=&(*_Iter).second;
				break;
				}
			}

			//Compile classes
			if (!_All_classes) {

			_Mycompiled_classes.push_back(std::pair<script_class*,script_class::
				_Classes>(_Current_class,script_class::_Classes()));
			_All_classes=&_Mycompiled_classes.back().second;

			compile_classes(_Current_class,*_All_classes);
			}

			for (_Classes::iterator _Iter=_All_classes->begin(),
				_End=_All_classes->end();_Iter!=_End;++_Iter) {
			
			const std::string &_Class_name = (*_Iter)->_Name; //Increase

			_Templates::iterator _Find=_Mytemplates.find(_Template_name=="default"?
				_Class_name:_Class_name+"."+_Template_name);

				//Check if a template definition exists
				if (_Find!=_Mytemplates.end()) {
				
				_Current_object->append((*_Find).second.second);
				_Current_object->_Templated=true; //Template inserted
				continue; //Continue to next token
				}
			}
		}
	
	script_class::_Properties *_All_properties=0;
	
		//Look for compiled properties for this class
		for (_Compiled_properties::iterator _Iter=_Mycompiled_properties.begin(),
			_End=_Mycompiled_properties.end();_Iter!=_End;++_Iter) {
		
			if ((*_Iter).first==_Current_class) {
			
			_All_properties=&(*_Iter).second;
			break;
			}
		}
		
		//Compile properties
		if (!_All_properties) {
		
		_Mycompiled_properties.push_back(std::pair<script_class*,script_class::
			_Properties>(_Current_class,script_class::_Properties()));
		_All_properties=&_Mycompiled_properties.back().second;
		
		compile_properties(_Current_class,*_All_properties);
		}
		
		for (script_class::_Properties::const_iterator _Iter=_All_properties->begin(),
			_End=_All_properties->end();_Iter!=_End;++_Iter) {
		
		_Property=(*_Iter);
		
			//Check if property name equals token
			if (_Property->_Name==_Tokens[i]) {
			
			script_property::data::_Values _Myvalues;
			
				for (size_t j=i+1;j<_Tokens.size() && (_Myvalues.size()<_Property->
					_Myparameters.size() || _Property->_Variable_parameters);++j) {
				
				size_t _Parameter=(_Myvalues.size()<_Property->_Myparameters.size()?
					_Myvalues.size():_Property->_Myparameters.size()-1);
				bool _Custom_value=false;
				
					//Check for custom values associated with this parameter
					if (!_Property->_Myparameters[_Parameter]._Custom_values.empty()) {
					
					string_utility::tokens _Custom_values;
					string_utility::tokenize(_Property->_Myparameters[_Parameter].
						_Custom_values,",",_Custom_values);
					
						for (size_t k=0;k<_Custom_values.size();++k) {
						
							if (string_utility::compare(_Tokens[j],_Custom_values[k])) {
							
							_Custom_value=true;
							break;
							}
						}
					}
					
					//Check for regular values
					if (!_Custom_value) {
					
						try {
						
							switch (_Property->_Myparameters[_Parameter]._Data_type) {
							
								case script_property::DT_CUSTOM:
								throw std::bad_cast(); //Requires explicit custom value
								
								case script_property::DT_BOOL:
								lexical_cast<bool>(_Tokens[j],true);
								break;
								
								case script_property::DT_INT16:
								lexical_cast<short>(_Tokens[j],true);
								break;
								
								case script_property::DT_UINT16:
								lexical_cast<unsigned short>(_Tokens[j],true);
								break;
								
								case script_property::DT_INT32:
								lexical_cast<int>(_Tokens[j],true);
								break;
								
								case script_property::DT_UINT32:
								lexical_cast<unsigned int>(_Tokens[j],true);
								break;
								
								case script_property::DT_INT64:
								lexical_cast<long long>(_Tokens[j],true);
								break;
								
								case script_property::DT_UINT64:
								lexical_cast<unsigned long long>(_Tokens[j],true);
								break;
								
								case script_property::DT_FLOAT:
								lexical_cast<float>(_Tokens[j],true);
								break;
								
								case script_property::DT_DOUBLE:
								lexical_cast<double>(_Tokens[j],true);
								break;
								
								case script_property::DT_STRING:
								
									//String type requires literals
									if (_Tokens[j].size()<2 || _Tokens[j][0]!='"' ||
										_Tokens[j][_Tokens[j].size()-1]!='"')
											throw std::bad_cast();
								
								break;
								
								case script_property::DT_COLOR:
								lexical_cast<color>(_Tokens[j],true);
								break;
								
								case script_property::DT_VECTOR2:
								lexical_cast<vector2>(_Tokens[j],true);
								break;
							};
						}
						catch (std::bad_cast) {
							break; //Invalid data type for parameter
						}
					}
					
					//Token has correct data type for parameter
					if (!_Custom_value && _Property->_Myparameters[_Parameter].
						_Data_type==script_property::DT_STRING)
							_Myvalues.push_back(_Tokens[j].substr(
								1,_Tokens[j].size()-2));
					else _Myvalues.push_back(_Tokens[j]);
				}
				
				//Create data from this property
				if (_Myvalues.size()>=_Property->_Required_parameters) {
				
				script_property::data _Data;
				_Data._Property_name=_Property->_Name;
				_Data._Myvalues=_Myvalues;
				_Current_object->_Mydata.push_back(_Data);
				
				i+=_Myvalues.size(); //Skip tokens
				break;
				}
			
			//Don't break
			//Inherited properties can have equal names
			}
		}
	}
	
	//Merge objects from same class with equal names
	if (_Result._Merge) merge_objects(_Result._Myobjects);

//Remove objects with no child objects or properties
cleanup_objects(_Result._Myobjects);

	//Convert to a linear result
	if (_Result._Type==result::LINEAR && !_Result._Myobjects.empty()) {
	
	script_class::object::_Objects _Linear_objects;
	linearize(_Result._Myobjects,_Linear_objects);
	_Result._Myobjects=_Linear_objects; //Use linear result
	}

return !_Result._Myobjects.empty();
}

void script_parser::compile_classes(script_class *_Script_class,
	script_class::_Classes &_Myclasses) {

	//Can't inherit from the same class multiple times
	for (_Classes::const_iterator _Iter=_Myclasses.begin(),
		_End=_Myclasses.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Script_class) return;
	}

_Myclasses.push_back(_Script_class);
	
	//Compile inherited classes
	for (script_class::_Classes::const_iterator _Iter=_Script_class->
		_Myregistered_inheritances.begin(),_End=_Script_class->
			_Myregistered_inheritances.end();_Iter!=_End;++_Iter)
				compile_classes(*_Iter,_Myclasses);
}

void script_parser::compile_scopes(script_class *_Script_class,
	script_class::_Scopes &_Myscopes) {

_Classes _Inherited_classes;
compile_scopes(_Script_class,_Myscopes,_Inherited_classes);
}

void script_parser::compile_scopes(script_class *_Script_class,
	script_class::_Scopes &_Myscopes,_Classes &_Inherited_classes) {

	//Can't inherit from the same class multiple times
	for (_Classes::const_iterator _Iter=_Inherited_classes.begin(),
		_End=_Inherited_classes.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Script_class) return;
	}

_Inherited_classes.push_back(_Script_class);

	//Compile scopes
	for (script_class::_Scopes::const_iterator _Iter=_Script_class->
		_Myregistered_scopes.begin(),_End=_Script_class->_Myregistered_scopes.end();
			_Iter!=_End;++_Iter) {

	bool found=false;

		for (script_class::_Scopes::const_iterator _Siter=_Myscopes.begin(),
			_Send=_Myscopes.end();_Siter!=_Send;++_Siter) {
		
			if ((*_Siter)==(*_Iter)) {
			
			found = true;
			break;
			}
		}

		if (!found) _Myscopes.push_back(*_Iter);
	}
	
	//Compile inherited scopes
	for (script_class::_Classes::const_iterator _Iter=_Script_class->
		_Myregistered_inheritances.begin(),_End=_Script_class->
			_Myregistered_inheritances.end();_Iter!=_End;++_Iter)
				compile_scopes(*_Iter,_Myscopes,
					_Inherited_classes);
}

void script_parser::compile_properties(script_class *_Script_class,
	script_class::_Properties &_Myproperties) {

_Classes _Inherited_classes;
compile_properties(_Script_class,_Myproperties,_Inherited_classes);
}

void script_parser::compile_properties(script_class *_Script_class,
	script_class::_Properties &_Myproperties,_Classes &_Inherited_classes) {

	//Can't inherit from the same class multiple times
	for (_Classes::const_iterator _Iter=_Inherited_classes.begin(),
		_End=_Inherited_classes.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Script_class) return;
	}

_Inherited_classes.push_back(_Script_class);

	//Compile properties
	for (script_class::_Properties::const_iterator _Iter=_Script_class->
		_Myproperties.begin(),_End=_Script_class->_Myproperties.end();
			_Iter!=_End;++_Iter) _Myproperties.push_back(*_Iter);
	
	//Compile inherited properties
	for (script_class::_Classes::const_iterator _Iter=_Script_class->
		_Myregistered_inheritances.begin(),_End=_Script_class->
			_Myregistered_inheritances.end();_Iter!=_End;++_Iter)
				compile_properties(*_Iter,_Myproperties,
					_Inherited_classes);
}

void script_parser::merge_objects(script_class::object::_Objects &_Myobjects) {

	for (script_class::object::_Objects::iterator _Iter=_Myobjects.begin(),
		_End=_Myobjects.end();_Iter!=_End;++_Iter) {
	
	script_class::object &_Object=(*_Iter); //Increase speed
	
		//Not allowed to merge objects without a name
		if (!_Object._Name.empty()) {
		
		script_class::object::_Objects::iterator _Oiter=_Iter;
		
			for (++_Oiter;_Oiter!=_End;) {
			
			script_class::object &_Object2=(*_Oiter); //Increase speed
			
				//Found two objects from same class with equal names
				if (_Object._Class_name==_Object2._Class_name &&
					_Object._Name==_Object2._Name) {
				
				_Object.append(_Object2);
				_Oiter=_Myobjects.erase(_Oiter);
				}
				else ++_Oiter;
			}
		}
		
		if (!_Object._Myobjects.empty())
			merge_objects(_Object._Myobjects);
	}
}

void script_parser::cleanup_objects(script_class::object::_Objects &_Myobjects) {

	for (script_class::object::_Objects::iterator _Iter=_Myobjects.begin(),
		_End=_Myobjects.end();_Iter!=_End;) {
	
	script_class::object &_Object=(*_Iter); //Increase speed
	
		if (!_Object._Myobjects.empty())
			cleanup_objects(_Object._Myobjects);
		
		if (_Object._Myobjects.empty() && _Object._Mydata.empty())
			_Iter=_Myobjects.erase(_Iter);
		else ++_Iter;
	}
}

void script_parser::linearize(const script_class::object::_Objects &_Source,
	script_class::object::_Objects &_Destination) {

	for (script_class::object::_Objects::const_iterator _Iter=
		_Source.begin(),_End=_Source.end();_Iter!=_End;++_Iter) {
	
	const script_class::object &_Object=(*_Iter); //Increase speed
	
	_Destination.push_back(script_class::object());
	_Destination.back().copy_linear(_Object); //Make linear copy
	
		if (!_Object._Myobjects.empty()) linearize(
			_Object._Myobjects,_Destination);
	}
}

//Public

script_parser::script_parser():_Script_container(
	file_container::instance()) {
	_Global_class=new script_class(this,"");
}

script_parser::script_parser(const script_parser &_Script_parser) {

_Global_class=new script_class(this,""); //Not copyable
copy(_Script_parser);
}

script_parser::~script_parser() {

clear_classes();
clear_scopes();

delete _Global_class;
}

script_parser &script_parser::operator=(const script_parser &_Script_parser) {

	//Worth doing
	if (this!=&_Script_parser) copy(_Script_parser);

return *this;
}

bool script_parser::parse_file(const std::string &_Filename,
	result &_Result) const {

std::string _Data;

	if (!file_manager::load_binary(_Filename,_Data)) return false;

_Result._Script_name=_Filename;
return parse(_Data,_Result);
}

bool script_parser::parse_script(const std::string &_Script_name,
	result &_Result) const {

file_data *_Script_data=static_cast<file_data*>(
	_Script_container->data(_Script_name));

	if (!_Script_data) return false;

std::string _Data;

//Load script data from data file
_Script_container->load_bytes(_Script_data,_Data);

	//Load script data from file (filename)
	if (_Data.empty() && !file_manager::load_binary(
		_Script_data->filename(),_Data)) return false;

_Result._Script_name=_Script_data->name();
return parse(_Data,_Result);
}

bool script_parser::parse_data(const std::string &_Script_data,
	result &_Result) const {
	return parse_data(_Script_data,"",_Result);
}

bool script_parser::parse_data(const std::string &_Script_data,
	const std::string &_Script_name,result &_Result) const {

_Result._Script_name=_Script_name;
return parse(_Script_data,_Result);
}

void script_parser::script_source(file_container *_Script_container) {
	this->_Script_container=_Script_container;
}

file_container *script_parser::script_source() const {
	return _Script_container;
}

//Classes

script_class *script_parser::create_class(const std::string &_Name) {

	//Class already exist
	if (class_pointer(_Name)) return 0;

script_class *_Myclass=new script_class(this,_Name);
_Myclasses.push_back(_Myclass);
return _Myclass;
}

script_class *script_parser::create_class(const std::string &_Name,
	const script_class &_Script_class) {

	//Class already exist
	if (class_pointer(_Name)) return 0;

script_class *_Myclass=new script_class(this,_Name,_Script_class);
_Myclasses.push_back(_Myclass);
return _Myclass;
}

void script_parser::remove_class(script_class *&_Script_class) {

	for (_Classes::iterator _Iter=_Myclasses.begin(),
		_End=_Myclasses.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Script_class) {
		
			//Unregister class from all scopes
			for (_Scopes::const_iterator _Siter=_Myscopes.begin(),
				_Send=_Myscopes.end();_Siter!=_Send;++_Siter)
					(*_Siter)->unregister_class(_Script_class);
			
			//Unregister class from all inheritances
			for (_Classes::const_iterator _Citer=_Myclasses.begin(),
				_Cend=_Myclasses.end();_Citer!=_Cend;++_Citer)
					(*_Citer)->unregister_inheritance(_Script_class);
		
		delete _Script_class;
		_Script_class=0; //Set referenced pointer to null
		
		_Myclasses.erase(_Iter);
		return;
		}
	}
}

void script_parser::clear_classes() {

	//Unregister all scope classes
	for (_Scopes::const_iterator _Iter=_Myscopes.begin(),
		_End=_Myscopes.end();_Iter!=_End;++_Iter)
			(*_Iter)->unregister_classes();
	
	//Unregister all inheritances
	for (_Classes::const_iterator _Iter=_Myclasses.begin(),
		_End=_Myclasses.end();_Iter!=_End;++_Iter)
			(*_Iter)->unregister_inheritances();
	
	for (_Classes::const_iterator _Iter=_Myclasses.begin(),
		_End=_Myclasses.end();_Iter!=_End;++_Iter)
			delete (*_Iter);

_Myclasses.clear();
}

script_parser::classes_iterator script_parser::classes_begin() const {
	return _Myclasses.begin();
}

script_parser::classes_iterator script_parser::classes_end() const {
	return _Myclasses.end();
}

script_class *script_parser::class_pointer(const std::string &_Name) const {

	for (_Classes::const_iterator _Iter=_Myclasses.begin(),
		_End=_Myclasses.end();_Iter!=_End;++_Iter) {
	
		//Class name exist, return pointer
		if ((*_Iter)->_Name==_Name) return (*_Iter);
	}

return 0;
}

//Scopes

script_scope *script_parser::create_scope(const std::string &_Name) {

	//Scope already exist
	if (scope_pointer(_Name)) return 0;

script_scope *_Myscope=new script_scope(this,_Name);
_Myscopes.push_back(_Myscope);
return _Myscope;
}

script_scope *script_parser::create_scope(const std::string &_Name,
	const script_scope &_Script_scope) {

	//Scope already exist
	if (scope_pointer(_Name)) return 0;

script_scope *_Myscope=new script_scope(this,_Name,_Script_scope);
_Myscopes.push_back(_Myscope);
return _Myscope;
}

void script_parser::remove_scope(script_scope *&_Script_scope) {

	for (_Scopes::iterator _Iter=_Myscopes.begin(),
		_End=_Myscopes.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Script_scope) {
		
			//Unregister scope from all classes
			for (_Classes::const_iterator _Citer=_Myclasses.begin(),
				_Cend=_Myclasses.end();_Citer!=_Cend;++_Citer)
					(*_Citer)->unregister_scope(_Script_scope);
		
		_Global_class->unregister_scope(_Script_scope); //Unregister scope globally
		
		delete _Script_scope;
		_Script_scope=0; //Set referenced pointer to null
		
		_Myscopes.erase(_Iter);
		return;
		}
	}
}

void script_parser::clear_scopes() {

	//Unregister all class scopes
	for (_Classes::const_iterator _Iter=_Myclasses.begin(),
		_End=_Myclasses.end();_Iter!=_End;++_Iter)
			(*_Iter)->unregister_scopes();

_Global_class->unregister_scopes(); //Unregister all global scopes

	for (_Scopes::const_iterator _Iter=_Myscopes.begin(),
		_End=_Myscopes.end();_Iter!=_End;++_Iter)
			delete (*_Iter);

_Myscopes.clear();
}

script_parser::scopes_iterator script_parser::scopes_begin() const {
	return _Myscopes.begin();
}

script_parser::scopes_iterator script_parser::scopes_end() const {
	return _Myscopes.end();
}

script_scope *script_parser::scope_pointer(const std::string &_Name) const {

	for (_Scopes::const_iterator _Iter=_Myscopes.begin(),
		_End=_Myscopes.end();_Iter!=_End;++_Iter) {
	
		//Scope name exist, return pointer
		if ((*_Iter)->_Name==_Name) return (*_Iter);
	}

return 0;
}

//Registration

void script_parser::register_scope(script_scope *_Script_scope) {
	_Global_class->register_scope(_Script_scope);
}

void script_parser::unregister_scope(script_scope *_Script_scope) {
	_Global_class->unregister_scope(_Script_scope);
}

void script_parser::unregister_scopes() {
	_Global_class->unregister_scopes();
}

} //namespace lunar