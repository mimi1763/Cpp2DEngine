/*
<Project name>

Written by <Name>
game.cpp
*/

#include "game.h"

//Dependencies (engine)
#include "file_manager.h"
#include "input_manager.h"
#include "render_manager.h"
#include "resource_manager.h"
#include "texture_manager.h"
#include "render_node.h"
#include "math_utility.h"

#include "string_utility.h"
#include "any_cast.h"
#include "scene_manager.h"

//Protected

bool game::update(float time)
{
	return true;
}

bool game::post_update(float time)
{
	return true;
}

bool game::message(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void game::key_pressed(unsigned int key)
{
	//Empty
}

void game::key_released(unsigned int key)
{
	//Empty
}

void game::char_pressed(char character)
{
	//Empty
}

void game::mouse_pressed(unsigned int button, unsigned int x, unsigned int y)
{
	//Empty
}

void game::mouse_released(unsigned int button, unsigned int x, unsigned int y)
{
	//Empty
}

void game::mouse_moved(unsigned int x, unsigned int y)
{
	//Empty
}

void game::mouse_scrolled(int delta, unsigned int x, unsigned int y)
{
	//Empty
}

void game::load_settings(std::vector<unsigned int> &settings) const
{
	file_manager::string_list lines;
	
	if (!file_manager::load("settings.ini", lines))
		return;

	string_utility::tokens tokens;

	for (const auto &line : lines)
	{
		string_utility::tokenize(line, "=", tokens);
		
		if (tokens.size() > 1)
		{
			if (tokens[0] == "width")
				settings[0] = any_cast<unsigned int>(tokens[1]);
			else if (tokens[0] == "height")
				settings[1] = any_cast<unsigned int>(tokens[1]);
			else if (tokens[0] == "colordepth")
				settings[2] = any_cast<unsigned int>(tokens[1]);
			else if (tokens[0] == "fullscreen")
				settings[3] = any_cast<unsigned int>(tokens[1]);
			else if (tokens[0] == "vsync")
				settings[4] = any_cast<unsigned int>(tokens[1]);
		}

		tokens.clear();
	}
}

//Public

game::game() noexcept
{
	//Empty
}

game::~game()
{
	//Empty
}

int game::start()
{
	std::vector<unsigned int> settings{ 1024, 768, 32, 1, 0 }; //Set default values
	load_settings(settings); //Load values from ini

	render_window &renderWindow = *engine.create_renderwindow(
		"Project name", settings[0], settings[1], settings[2], (settings[3] > 0));

	renderWindow.stretch_size(1024.0f, 768.0f);
	renderWindow.min_size(800, 600);
	renderWindow.stretch(true);
	renderWindow.mouse_visible(true);

	//Initialize Lunar Engine
	if (!engine.init())
		return 1;

	engine.vertical_sync(settings[4] > 0);
	engine.subscribe_framelistener(this);
	render_window::instance()->subscribe_messagelistener(this);
	input_manager::instance()->subscribe_keylistener(this);
	input_manager::instance()->subscribe_mouselistener(this);

	resource_manager::instance()->load("bin/resources.res");


	//texture *tex = texture_manager::instance()->create_texture("castle_bg");
	
	curve *line = scene_manager::instance()->create_curve(2.0f, color(233,34,114));
	line->add_controlpoint(vector2(0.0f, 0.0f));
	line->add_controlpoint(vector2(100.0f, 0.0f));

	curve *line2 = scene_manager::instance()->create_curve(*line);

	render_node *gridnode = render_manager::instance()->root_node()->create_childnode(1);
	gridnode->position(vector2(256.0f, 384.0f));

	render_node *node = gridnode->create_childnode(1);
	node->attach(line);

	render_node *node2 = gridnode->create_childnode(1);
	node2->position(vector2(0.0f, -10.0f));
	node2->attach(line2);


	/*tex->auto_size(false);
	tex->keep_ratio(true);
	tex->ratio_mode(texturable::RATIO_KEEP_WIDTH);
	tex->size(256, 128);

	render_node *node = render_manager::instance()->root_node()->create_childnode(1);
	node->position(vector2(512.0f, 384.0f));
	node->attach(tex);
	node->rotate(math::degree(45.0f));*/

	return engine.start(); //Start rendering
}