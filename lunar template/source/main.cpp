/*
<Project name>

Written by <Name>
main.cpp
*/

#include <windows.h>
#include "game.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR cmdLine, int cmdShow)
{
	game myGame;
	return myGame.start();
}