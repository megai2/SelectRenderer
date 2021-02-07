#pragma once
#include "stdafx.h"

struct addon_instance
{
	gw2al_core_vtable* api;
	Main main;
	lib_imgui<Gui> gui;
};

addon_instance& gAddon();
