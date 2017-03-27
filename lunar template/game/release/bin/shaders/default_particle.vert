/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2009

Author: Jan Ivar Goli
Area: shading
File: default_particle.vert
---------------------------------------------------
*/

attribute float pointSize;

void main(void) {

gl_PointSize=pointSize;
gl_TexCoord[0]=gl_MultiTexCoord0;

gl_FrontColor=gl_Color;
gl_Position=ftransform();
}