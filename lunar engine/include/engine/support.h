/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: support.h
---------------------------------------------------
*/

#ifndef _SUPPORT_
#define _SUPPORT_

namespace lunar {

//Forward declarations
class core;

class support {

	friend core; //Grant core exclusive access to initialize this

public:

	enum TYPE { NONE, OPENGL, ARB };

private:

	static TYPE _Blend_separate;
	static TYPE _Fbo;
	static TYPE _Multi_texture;
	static TYPE _Point_sprite;
	static TYPE _Shader;
	static TYPE _Vbo;
	
	static int _Texture_units;
	
	//Private constructor
	support();
	
	//Private destructor
	~support();

public:

	static TYPE blend_separate();
	static TYPE fbo();
	static TYPE multi_texture();
	static TYPE point_sprite();
	static TYPE shader();
	static TYPE vbo();
	
	static int texture_units();
};

} //namespace lunar

#endif