/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: shaderprogram_manager.cpp
---------------------------------------------------
*/

#include "shaderprogram_manager.h"

//Dependencies
#include "system.h"

#include "support.h"
#include "shader_manager.h"
#include "file_manager.h"

#include "texture_manager.h"
#include "animation_manager.h"

#include "script_parser.h"
#include "string_utility.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
shaderprogram_manager *shaderprogram_manager::_Instance=0;

//Protected

void shaderprogram_manager::copy(const shaderprogram_manager &_Shaderprogram_manager) {

_Script_container=_Shaderprogram_manager._Script_container;
_Shader_manager=_Shaderprogram_manager._Shader_manager;
_Texture_manager=_Shaderprogram_manager._Texture_manager;
_Animation_manager=_Shaderprogram_manager._Animation_manager;
}

void shaderprogram_manager::removed(manageable *_Manageable) {

shaderprogram *_Shaderprogram=static_cast<shaderprogram*>(_Manageable);

	//Invalid GL program, skip
	if (_Shaderprogram->_Glprogram==0) return;
	
	//When gl program is deleted, shaders are automatically detached
	if (support::shader()==support::OPENGL) glDeleteProgram(
		_Shaderprogram->_Glprogram);
	else if (support::shader()==support::ARB) glDeleteObjectARB(
		_Shaderprogram->_Glprogram);

_Shaderprogram->_Glprogram=0;
}

//GL program/script specific

void shaderprogram_manager::init_particlescript(script_parser *_Script_parser) {

script_property *_Property;

//Shaderprogram class
script_class *_Shaderprogram_class=_Script_parser->create_class("shaderprogram");

_Property=_Shaderprogram_class->create_property("shader");
_Property->add_parameter(script_property::DT_STRING);

//Shadervariable class
script_class *_Shadervariable_class=_Script_parser->create_class("shadervariable");

_Property=_Shadervariable_class->create_property("components");
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Shadervariable_class->create_property("auto_update");
_Property->add_parameter(script_property::DT_BOOL);

//Attribute class
script_class *_Attribute_class=_Script_parser->create_class("attribute");

_Property=_Attribute_class->create_property("stride");
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Attribute_class->create_property("normalized");
_Property->add_parameter(script_property::DT_BOOL);

//Uniform class
script_class *_Uniform_class=_Script_parser->create_class("uniform");

_Property=_Uniform_class->create_property("size");
_Property->add_parameter(script_property::DT_UINT32);

//Sampler class
script_class *_Sampler_class=_Script_parser->create_class("sampler");

_Property=_Sampler_class->create_property("texture");
_Property->add_parameter(script_property::DT_STRING);

_Property=_Sampler_class->create_property("animation");
_Property->add_parameter(script_property::DT_STRING);

//Scopes
script_scope *_Shaderprogram_scope=_Script_parser->create_scope("shaderprogram");
_Shaderprogram_scope->register_class(_Shaderprogram_class);

script_scope *_Variable_scope=_Script_parser->create_scope("variable");
_Variable_scope->register_class(_Attribute_class);
_Variable_scope->register_class(_Uniform_class);
_Variable_scope->register_class(_Sampler_class);

_Script_parser->register_scope(_Shaderprogram_scope);
_Shaderprogram_class->register_scope(_Variable_scope);

//Inheritance
_Attribute_class->register_inheritance(_Shadervariable_class);
_Uniform_class->register_inheritance(_Shadervariable_class);
_Sampler_class->register_inheritance(_Uniform_class);
}

bool shaderprogram_manager::init_shaderprogram(shaderprogram *_Shaderprogram,
	file_data *_Script_data) {

std::string _Data;

//Load script data from data file
_Script_container->load_bytes(_Script_data,_Data);

	//Load script data from file (filename)
	if (_Data.empty() && !file_manager::load_binary(
		_Script_data->filename(),_Data)) return false;
	
	//Init script parser first time
	if (!_Script_parser) {
	
	_Script_parser=new script_parser();
	init_particlescript(_Script_parser);
	}

script_parser::result _Result(script_parser::result::LINEAR);

	if (!_Script_parser->parse_data(_Data,_Script_data->name(),_Result))
		return false;

typedef std::vector<shader_data*> _Shader_data;
_Shader_data _Myshader_data;

shadervariable *_Shadervariable=0; //Current variable

	for (script_class::object::objects_iterator _Iter=_Result.objects_begin(),
		_End=_Result.objects_end();_Iter!=_End;++_Iter) {
	
	const script_class::object &_Object=(*_Iter); //Increase speed
	
		//Variables
		if (_Object!="shaderprogram") {
		
			//No shader data found, return
			if (_Myshader_data.empty()) return false;
			
			//Invalid variable name, skip
			if (_Object.name().empty()) continue;
		
		_Shadervariable=0;
		
			if (_Object=="attribute") {
			
				//Check for an existing attribute with this name
				for (shaderprogram::_Attributes::const_iterator _Iter=_Shaderprogram->
					_Myattributes.begin(),_End=_Shaderprogram->_Myattributes.end();
						_Iter!=_End;++_Iter) {
				
					if ((*_Iter)->name()==_Object.name()) {
					
					_Shadervariable=(*_Iter);
					break;
					}
				}
				
				if (!_Shadervariable) _Shadervariable=_Shaderprogram->
					create_attribute(_Object.name());
			}
			else if (_Object=="uniform") {
			
				//Check for an existing uniform with this name
				for (shaderprogram::_Uniforms::const_iterator _Iter=_Shaderprogram->
					_Myuniforms.begin(),_End=_Shaderprogram->_Myuniforms.end();
						_Iter!=_End;++_Iter) {
				
					if ((*_Iter)->name()==_Object.name()) {
					
					_Shadervariable=(*_Iter);
					break;
					}
				}
				
				if (!_Shadervariable) _Shadervariable=_Shaderprogram->
					create_uniform(_Object.name());
			}
			else if (_Object=="sampler") {
			
				//Check for an existing sampler with this name
				for (shaderprogram::_Samplers::const_iterator _Iter=_Shaderprogram->
					_Mysamplers.begin(),_End=_Shaderprogram->_Mysamplers.end();
						_Iter!=_End;++_Iter) {
				
					if ((*_Iter)->name()==_Object.name()) {
					
					_Shadervariable=(*_Iter);
					break;
					}
				}
				
				if (!_Shadervariable) _Shadervariable=_Shaderprogram->
					create_sampler(_Object.name());
			}
		}
		
		//Set properties
		for (script_class::object::data_iterator _Diter=_Object.data_begin(),
			_Dend=_Object.data_end();_Diter!=_Dend;++_Diter) {
		
		const script_property::data &_Data=(*_Diter); //Increase speed
		
			//Shaderprogram properties
			if (_Object=="shaderprogram") {
			
				if (_Data=="shader") {
				
				shader_data *_Shader_data=static_cast<shader_data*>(
					_Shader_manager->shader_source()->data(_Data.value(0)));
				
					//Add shader data if found
					if (_Shader_data) _Myshader_data.push_back(_Shader_data);
				}
			}
			//Variable properties
			else {
			
				//Shared properties
				if (_Data=="components") {
				
				_Shadervariable->components(_Data.value<unsigned int>(0));
				continue;
				}
				else if (_Data=="auto_update") {
				
				_Shadervariable->auto_update(_Data.value<bool>(0));
				continue;
				}
				
				if (_Object=="attribute") {
				
				attribute *_Attribute=static_cast<attribute*>(_Shadervariable);
				
					if (_Data=="stride")
						_Attribute->stride(_Data.value<unsigned int>(0));
					else if (_Data=="normalized")
						_Attribute->normalized(_Data.value<bool>(0));
				}
				else if (_Object=="uniform" || _Object=="sampler") {
				
				uniform *_Uniform=static_cast<uniform*>(_Shadervariable);
				
					//Uniform properties
					if (_Data=="size")
						_Uniform->size(_Data.value<unsigned int>(0));
					//Sampler properties
					else if (_Object=="sampler") {
					
					sampler *_Sampler=static_cast<sampler*>(_Uniform);
					
						if (_Data=="texture" || _Data=="animation") {
						
						manager *_Manager=_Texture_manager; //Texture mode
						
							//Animation mode
							if (_Data!="texture") _Manager=_Animation_manager;
						
						texturable *_Texturable=0;
						manageable *_Manageable;
						
							//Reuse texture/animation if at least one instance already exist
							for (manager::manageables_iter _Miter=_Manager->manageables_begin(),
								_Mend=_Manager->manageables_end();_Miter!=_Mend;++_Miter) {
							
							_Manageable=(*_Miter); //Increase speed
							
								if (_Manageable->data() && _Manageable->
									data()->name()==_Data.value(0)) {
								
								_Texturable=dynamic_cast<texturable*>(_Manageable);
								break;
								}
							}
							
							if (_Manager==_Texture_manager) {
							
								//Create texture
								if (!_Texturable) _Texturable=_Texture_manager->
									create_texture(_Data.value(0));
							
							_Sampler->texture_data(static_cast<texture*>(_Texturable));
							}
							else {
							
								//Create animation
								if (!_Texturable) _Texturable=_Animation_manager->
									create_animation(_Data.value(0));
							
							_Sampler->animation_data(static_cast<animation*>(_Texturable));
							}
						}
					}
				}
			}
		}
	}

removed(_Shaderprogram); //Make sure previous GL program is deleted and shaders detached

	if (support::shader()==support::OPENGL)
		_Shaderprogram->_Glprogram=glCreateProgram();
	else if (support::shader()==support::ARB)
		_Shaderprogram->_Glprogram=glCreateProgramObjectARB();
	
	//Invalid GL program
	if (_Shaderprogram->_Glprogram==0) return false;
	
	//Shaderprogram has too many shaders, deallocate rest
	if (_Shaderprogram->_Myshaders.size()>_Myshader_data.size())
		_Shader_manager->remove(_Shaderprogram->_Myshaders[
			_Myshader_data.size()],(unsigned int)_Shaderprogram->
				_Myshaders.size()-_Myshader_data.size());
	else {
	
		//Allocate needed storage
		if (_Shaderprogram->_Myshaders.capacity()<_Myshader_data.size())
			_Shaderprogram->_Myshaders.reserve(_Myshader_data.size());
		
		//Too few shaders allocated for shaderprogram, remove all
		//Need shaders to be stored continuously in shader manager
		if (!_Shaderprogram->_Myshaders.empty() && _Shaderprogram->
			_Myshaders.size()<_Myshader_data.size()) _Shader_manager->
				remove(_Shaderprogram->_Myshaders.front(),(unsigned int)
					_Shaderprogram->_Myshaders.size());
	}
	
	//Create new shaders for shaderprogram
	if (_Shaderprogram->_Myshaders.empty()) {
	
		for (size_t i=0;i<_Myshader_data.size();++i) {
		
		shader *_Shader=_Shader_manager->create_shader(_Myshader_data[i]);
		_Shader->subscribe(_Shaderprogram,true); //Add secure listener
		_Shaderprogram->_Myshaders.push_back(_Shader);
		}
	}
	//Change shader data on already allocated shaders
	else {
	
		for (size_t i=0;i<_Myshader_data.size();++i)
			_Shaderprogram->_Myshaders[i]->data(_Myshader_data[i]);
	}
	
	//Update all shaders in program
	for (size_t i=0;i<_Shaderprogram->_Myshaders.size();++i) {
	
	_Shader_manager->update(_Shaderprogram->_Myshaders[i]);
	
		//All shaders in the program must be valid
		if (_Shaderprogram->_Myshaders[i]->status()!=manageable::OKAY)
			return false;
	}
	
	//Attach all shaders to program
	for (size_t i=0;i<_Shaderprogram->_Myshaders.size();++i) {
	
		if (support::shader()==support::OPENGL) glAttachShader(
			_Shaderprogram->_Glprogram,_Shaderprogram->_Myshaders[i]->
				glshader());
		else if (support::shader()==support::ARB) glAttachObjectARB(
			_Shaderprogram->_Glprogram,_Shaderprogram->_Myshaders[i]->
				glshader());
	}

int _Link=0;

	//Link GL program with attached shaders
	if (support::shader()==support::OPENGL) {
	
	glLinkProgram(_Shaderprogram->_Glprogram);
	glGetProgramiv(_Shaderprogram->_Glprogram,GL_LINK_STATUS,&_Link);
	}
	else if (support::shader()==support::ARB) {
	
	glLinkProgramARB(_Shaderprogram->_Glprogram);
	glGetObjectParameterivARB(_Shaderprogram->_Glprogram,
		GL_OBJECT_LINK_STATUS_ARB,&_Link);
	}
	
	//Write to shaderprogram log if linking failed
	if (_Link==0) {
	
	save_shaderprogramlog(_Shaderprogram->_Glprogram);
	return false;
	}

return true;
}

void shaderprogram_manager::save_shaderprogramlog(unsigned int _Glprogram) {

int _Length=0;

	//Retrieve log length from program object
	if (support::shader()==support::OPENGL) glGetProgramiv(
		_Glprogram,GL_INFO_LOG_LENGTH,&_Length);
	else if (support::shader()==support::ARB) glGetObjectParameterivARB(
		_Glprogram,GL_OBJECT_INFO_LOG_LENGTH_ARB,&_Length);
	
	if (_Length>0) {
	
	int _Written=0;
	char *_Log=new char[_Length];
	
		//Retrieve log from program object
		if (support::shader()==support::OPENGL) glGetProgramInfoLog(
			_Glprogram,_Length,&_Written,_Log);
		else if (support::shader()==support::ARB) glGetInfoLogARB(
			_Glprogram,_Length,&_Written,_Log);
	
	std::string _Data(_Log);
	delete[] _Log;
	
	file_manager::save("shaderprogram_log.txt",_Data,true); //Save shaderprogram log
	}
}

//Shader variable specific

bool shaderprogram_manager::init_uniform(uniform *_Uniform,
	shaderprogram *_Shaderprogram) {

_Uniform->_Location=-1;

	//Get uniform location in GL program
	if (support::shader()==support::OPENGL) _Uniform->_Location=
		glGetUniformLocation(_Shaderprogram->_Glprogram,
			_Uniform->_Name.c_str());
	else if (support::shader()==support::ARB) _Uniform->_Location=
		glGetUniformLocationARB(_Shaderprogram->_Glprogram,
			_Uniform->_Name.c_str());

return (_Uniform->_Location>=0);
}

bool shaderprogram_manager::init_attribute(attribute *_Attribute,
	shaderprogram *_Shaderprogram) {

_Attribute->_Location=-1;

	//Get attribute location in GL program
	if (support::shader()==support::OPENGL) _Attribute->_Location=
		glGetAttribLocation(_Shaderprogram->_Glprogram,
			_Attribute->_Name.c_str());
	else if (support::shader()==support::ARB) _Attribute->_Location=
		glGetAttribLocationARB(_Shaderprogram->_Glprogram,
			_Attribute->_Name.c_str());

return (_Attribute->_Location>=0);
}

bool shaderprogram_manager::init_sampler(sampler *_Sampler,
	shaderprogram *_Shaderprogram) {

	//Must be supported
	if (support::multi_texture()==support::NONE) return false;
	
	//Init uniform
	if (!init_uniform(_Sampler,_Shaderprogram)) return false;

int _Unit=0;
bool _Used=false;

sampler *_Iter_sampler;

	//Find first unused unit
	do {
	
		//Max supported samplers per shader program
		if (_Unit==support::texture_units()) return false;
	
	_Used=false;
	
		for (shaderprogram::_Samplers::const_iterator _Iter=_Shaderprogram->
			_Mysamplers.begin(),_End=_Shaderprogram->_Mysamplers.end();
				_Iter!=_End;++_Iter) {
		
		_Iter_sampler=(*_Iter); //Increase speed
		
			//Unit is in use
			if (_Iter_sampler->_Status==shadervariable::OKAY &&
				_Iter_sampler->_Unit==_Unit) {
			
			_Used=true;
			break;
			}
		}
		
		if (_Used) ++_Unit;
	} while (_Used);

_Sampler->_Unit=_Unit;
_Sampler->_Update_data=true;
return true;
}

void shaderprogram_manager::update_uniforms(shaderprogram *_Shaderprogram) const {

uniform *_Uniform;

	//Update uniform variables
	for (shaderprogram::_Uniforms::const_iterator _Iter=_Shaderprogram->
		_Myuniforms.begin(),_End=_Shaderprogram->_Myuniforms.end();
			_Iter!=_End;++_Iter) {
	
	_Uniform=(*_Iter); //Increase speed
	
		//No need for update
		if (_Uniform->_Status!=shadervariable::NEED_UPDATE &&
			!_Uniform->_Auto_update && !_Uniform->_Update_data)
				continue;
		
		//Init uniform
		if (_Uniform->_Status==shadervariable::NEED_UPDATE)
			_Uniform->_Status=(init_uniform(_Uniform,_Shaderprogram)?
				shadervariable::OKAY:shadervariable::FAILED); //Pending
		
		//Invalid
		if (_Uniform->_Status!=shadervariable::OKAY ||
			!_Uniform->_Data) continue;
		
		//Send uniform data to shaderprogram
		if (support::shader()==support::OPENGL) {
		
			if (_Uniform->_Data_type==shadervariable::SHADER_INT) {
			
				switch (_Uniform->_Components) {
				
					case 0: //Redirect
					case 1:
					glUniform1iv(_Uniform->_Location,_Uniform->_Size,
						(int*)_Uniform->_Data);
					break;
					
					case 2:
					glUniform2iv(_Uniform->_Location,_Uniform->_Size,
						(int*)_Uniform->_Data);
					break;
					
					case 3:
					glUniform3iv(_Uniform->_Location,_Uniform->_Size,
						(int*)_Uniform->_Data);
					break;
					
					case 4: //Redirect
					default: //>
					glUniform4iv(_Uniform->_Location,_Uniform->_Size,
						(int*)_Uniform->_Data);
					break;
				};
			}
			else if (_Uniform->_Data_type==shadervariable::SHADER_FLOAT) {
			
				switch (_Uniform->_Components) {
				
					case 0: //Redirect
					case 1:
					glUniform1fv(_Uniform->_Location,_Uniform->_Size,
						(float*)_Uniform->_Data);
					break;
					
					case 2:
					glUniform2fv(_Uniform->_Location,_Uniform->_Size,
						(float*)_Uniform->_Data);
					break;
					
					case 3:
					glUniform3fv(_Uniform->_Location,_Uniform->_Size,
						(float*)_Uniform->_Data);
					break;
					
					case 4: //Redirect
					default: //>
					glUniform4fv(_Uniform->_Location,_Uniform->_Size,
						(float*)_Uniform->_Data);
					break;
				};
			}
		}
		else if (support::shader()==support::ARB) {
		
			if (_Uniform->_Data_type==shadervariable::SHADER_INT) {
			
				switch (_Uniform->_Components) {
				
					case 0: //Redirect
					case 1:
					glUniform1ivARB(_Uniform->_Location,_Uniform->_Size,
						(int*)_Uniform->_Data);
					break;
					
					case 2:
					glUniform2ivARB(_Uniform->_Location,_Uniform->_Size,
						(int*)_Uniform->_Data);
					break;
					
					case 3:
					glUniform3ivARB(_Uniform->_Location,_Uniform->_Size,
						(int*)_Uniform->_Data);
					break;
					
					case 4: //Redirect
					default: //>
					glUniform4ivARB(_Uniform->_Location,_Uniform->_Size,
						(int*)_Uniform->_Data);
					break;
				};
			}
			else if (_Uniform->_Data_type==shadervariable::SHADER_FLOAT) {
			
				switch (_Uniform->_Components) {
				
					case 0: //Redirect
					case 1:
					glUniform1fvARB(_Uniform->_Location,_Uniform->_Size,
						(float*)_Uniform->_Data);
					break;
					
					case 2:
					glUniform2fvARB(_Uniform->_Location,_Uniform->_Size,
						(float*)_Uniform->_Data);
					break;
					
					case 3:
					glUniform3fvARB(_Uniform->_Location,_Uniform->_Size,
						(float*)_Uniform->_Data);
					break;
					
					case 4: //Redirect
					default: //>
					glUniform4fvARB(_Uniform->_Location,_Uniform->_Size,
						(float*)_Uniform->_Data);
					break;
				};
			}
		}
	
	_Uniform->_Update_data=false; //Data is updated
	}
}

void shaderprogram_manager::update_attributes(shaderprogram *_Shaderprogram) const {

attribute *_Attribute;

	//Update attribute variables
	for (shaderprogram::_Attributes::const_iterator _Iter=_Shaderprogram->
		_Myattributes.begin(),_End=_Shaderprogram->_Myattributes.end();
			_Iter!=_End;++_Iter) {
	
	_Attribute=(*_Iter); //Increase speed
	
		//No need for update
		if (_Attribute->_Status!=shadervariable::NEED_UPDATE &&
			!_Attribute->_Auto_update && !_Attribute->_Update_data)
				continue;
		
		//Init attribute
		if (_Attribute->_Status==shadervariable::NEED_UPDATE)
			_Attribute->_Status=(init_attribute(_Attribute,_Shaderprogram)?
				shadervariable::OKAY:shadervariable::FAILED); //Pending
		
		//Invalid
		if (_Attribute->_Status!=shadervariable::OKAY ||
			!_Attribute->_Data) continue;
		
		//Send attribute data to shaderprogram
		if (support::shader()==support::OPENGL) {
		
		glEnableVertexAttribArray(_Attribute->_Location);
		glVertexAttribPointer(_Attribute->_Location,_Attribute->_Components,
			(_Attribute->_Data_type==shadervariable::SHADER_FLOAT?
				GL_FLOAT:GL_INT),_Attribute->_Normalized,_Attribute->_Stride,
					_Attribute->_Data);
		}
		else if (support::shader()==support::ARB) {
		
		glEnableVertexAttribArrayARB(_Attribute->_Location);
		glVertexAttribPointerARB(_Attribute->_Location,_Attribute->_Components,
			(_Attribute->_Data_type==shadervariable::SHADER_FLOAT?
				GL_FLOAT:GL_INT),_Attribute->_Normalized,_Attribute->_Stride,
					_Attribute->_Data);
		}
	
	_Attribute->_Update_data=false; //Data is updated
	}
}

void shaderprogram_manager::update_samplers(shaderprogram *_Shaderprogram) const {

sampler *_Sampler;

	//Update sampler variables
	for (shaderprogram::_Samplers::const_iterator _Iter=_Shaderprogram->
		_Mysamplers.begin(),_End=_Shaderprogram->_Mysamplers.end();
			_Iter!=_End;++_Iter) {
	
	_Sampler=(*_Iter); //Increase speed
	
	//Must bind texture to sampler each time
	
		//Init attribute
		if (_Sampler->_Status==shadervariable::NEED_UPDATE)
			_Sampler->_Status=(init_sampler(_Sampler,_Shaderprogram)?
				shadervariable::OKAY:shadervariable::FAILED); //Pending
		
		//Invalid
		if (_Sampler->_Status!=shadervariable::OKAY) continue;
	
	texture *_Texture=_Sampler->texture_data();
	animation *_Animation=_Sampler->animation_data();
	manageable *_Manageable=0;
	
		if (_Texture) _Manageable=_Texture;
		else if (_Animation) _Manageable=_Animation;
		
		if (_Manageable) _Manageable->update();
		
		//Set active texture
		if (support::multi_texture()==support::OPENGL)
			glActiveTexture(GL_TEXTURE0+_Sampler->_Unit);
		else if (support::multi_texture()==support::ARB)
			glActiveTextureARB(GL_TEXTURE0_ARB+_Sampler->_Unit);
	
	unsigned int _Gltexture=0;
	
		if (_Manageable && _Manageable->status()==manageable::OKAY)
			_Gltexture=(_Texture?_Texture->gltexture():_Animation->gltexture());
		else {
		
			if (_Sampler->_Data && _Sampler->_Data_type==shadervariable::
				SHADER_INT) _Gltexture=*(int*)_Sampler->_Data;
			else {
			
			texturable *_Texturable=dynamic_cast<texturable*>(
				render_manager::instance()->active_attachable());
			
				//Texturable is already prepared and updated
				if (_Texturable) _Gltexture=_Texturable->gltexture();
			}
		}
	
	glBindTexture(GL_TEXTURE_2D,_Gltexture); //Bind GL texture to active texture unit
	
		if (_Sampler->_Auto_update || _Sampler->_Update_data) {
		
			//Send sampler unit to shaderprogram
			if (support::shader()==support::OPENGL) glUniform1iv(
				_Sampler->_Location,_Sampler->_Size,&_Sampler->_Unit);
			else if (support::shader()==support::ARB) glUniform1ivARB(
				_Sampler->_Location,_Sampler->_Size,&_Sampler->_Unit);
		
		_Sampler->_Update_data=false; //Data is updated
		}
	}
}

//Public

shaderprogram_manager::shaderprogram_manager():_Script_container(
	file_container::instance()),_Shader_manager(shader_manager::instance()),
		_Texture_manager(texture_manager::instance()),_Animation_manager(
			animation_manager::instance()),_Script_parser(0) {
	//Empty
}

shaderprogram_manager::shaderprogram_manager(shader_manager *_Shader_manager):
	_Script_container(file_container::instance()),_Shader_manager(_Shader_manager),
		_Texture_manager(texture_manager::instance()),_Animation_manager(
			animation_manager::instance()),_Script_parser(0) {
	//Empty
}

shaderprogram_manager::shaderprogram_manager(shader_manager *_Shader_manager,
	texture_manager *_Texture_manager,animation_manager *_Animation_manager):
		_Script_container(file_container::instance()),_Shader_manager(_Shader_manager),
			_Texture_manager(_Texture_manager),_Animation_manager(_Animation_manager),
				_Script_parser(0) {
	//Empty
}

shaderprogram_manager::shaderprogram_manager(
	const shaderprogram_manager& _Shaderprogram_manager):
		manager(_Shaderprogram_manager) {

_Script_parser=0; //Not copyable
copy(_Shaderprogram_manager);
}

shaderprogram_manager::~shaderprogram_manager() {

	if (_Script_parser) delete _Script_parser;

tidy(); //Call before clearing cache
clear_cache();
}

shaderprogram_manager &shaderprogram_manager::operator=(
	const shaderprogram_manager &_Shaderprogram_manager) {

	//Worth doing
	if (this!=&_Shaderprogram_manager) {
	
	manager::operator=(_Shaderprogram_manager); //Invoke base operator=
	copy(_Shaderprogram_manager);
	}

return *this;
}

shaderprogram_manager *shaderprogram_manager::instance() {
	return _Instance;
}

void shaderprogram_manager::update(manageable *_Manageable) {

	//No need for update
	if (_Manageable->status()!=manageable::NEED_UPDATE) return;

shaderprogram *_Shaderprogram=dynamic_cast<shaderprogram*>(
	_Manageable);

	//Invalid type for this manager
	if (!_Shaderprogram) return;

file_data *_Script_data=static_cast<file_data*>(
	_Manageable->data());

	//No data
	if (!_Script_data) {
	
	status(_Shaderprogram,manageable::FAILED);
	return;
	}

status(_Shaderprogram,init_shaderprogram(_Shaderprogram,_Script_data)?
	manageable::OKAY:manageable::FAILED); //Pending
}

void shaderprogram_manager::update() {
	manager::update(); //Must redirect because of name reuse
}

void shaderprogram_manager::update_shadervariables(
	shaderprogram *_Shaderprogram) {

update(_Shaderprogram);

	//Shaderprogram must be okay before updating variables
	if (_Shaderprogram->_Status!=manageable::OKAY) return;
	
	if (!_Shaderprogram->_Myuniforms.empty()) update_uniforms(_Shaderprogram);
	if (!_Shaderprogram->_Myattributes.empty()) update_attributes(_Shaderprogram);
	if (!_Shaderprogram->_Mysamplers.empty()) update_samplers(_Shaderprogram);
}

void shaderprogram_manager::script_source(file_container *_Script_container) {
	this->_Script_container=_Script_container;
}

file_container *shaderprogram_manager::script_source() const {
	return _Script_container;
}

//Shaderprogram functions

shaderprogram *shaderprogram_manager::create_shaderprogram(
	const std::string &_Script_name,bool _Secured) {
	return create_shaderprogram(static_cast<file_data*>(
		_Script_container->data(_Script_name)),_Secured);
}

shaderprogram *shaderprogram_manager::create_shaderprogram(
	const file_data &_Script_data,bool _Secured) {
	return create_shaderprogram(_Script_container->add_data(
		_Script_data),_Secured);
}

shaderprogram *shaderprogram_manager::create_shaderprogram(
	file_data *_Script_data,bool _Secured) {

shaderprogram *_Myshaderprogram=new shaderprogram(this,_Script_data);
add(_Myshaderprogram,_Secured);
return _Myshaderprogram;
}

shaderprogram *shaderprogram_manager::create_shaderprogram(
	const shaderprogram &_Shaderprogram,bool _Secured) {

shaderprogram *_Myshaderprogram=new shaderprogram(this,_Shaderprogram);
add(_Myshaderprogram,_Secured);
return _Myshaderprogram;
}

void shaderprogram_manager::clear_cache() {

manageable *_Manageable;

	//Delete GL programs and update all manageables
	for (_Manageables::const_iterator _Iter=_Mymanageables.begin(),
		_End=_Mymanageables.end();_Iter!=_End;++_Iter) {
	
	_Manageable=(*_Iter); //Increase speed
	
	removed(_Manageable);
	status(_Manageable,manageable::NEED_UPDATE);
	}
}

} //namespace lunar