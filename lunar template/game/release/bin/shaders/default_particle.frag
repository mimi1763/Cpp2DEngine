/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2009

Author: Jan Ivar Goli
Area: shading
File: default_particle.frag
---------------------------------------------------
*/

uniform bool useTexture;
uniform sampler2D texture;

void main(void) {

	if (useTexture) gl_FragColor=texture2D(texture,gl_TexCoord[0].st)*gl_Color;
	else gl_FragColor=gl_Color;
}