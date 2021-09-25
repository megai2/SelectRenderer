#include "stdafx.h"
#include "build_version.h"

gw2al_addon_dsc gAddonDeps[] = {
	GW2AL_CORE_DEP_ENTRY,
	LIB_IMGUI_DEP_ENTRY,
	{ 0,0,0,0,0,0 }
};

gw2al_addon_dsc gAddonDsc = {
	L"SelectRender",
	L"Ingame UI based render selection for GW2",
	1,
	3,
	BUILD_VERSION_REV,
	gAddonDeps
};

addon_instance gInst;

addon_instance& gAddon()
{
	return gInst;
}

template<>
inline lib_imgui<Gui>& lib_imgui<Gui>::instance()
{
	return gInst.gui;
}


gw2al_addon_dsc* gw2addon_get_description()
{
	return &gAddonDsc;
}

gw2al_api_ret gw2addon_load(gw2al_core_vtable* core_api)
{
	gInst.api = core_api;

	gInst.main.init();
	gInst.gui.init(core_api, gAddonDsc.name);

	return GW2AL_OK;
}

gw2al_api_ret gw2addon_unload(int gameExiting)
{
	gInst.gui.deinit();
	gInst.main.deinit();

	return GW2AL_OK;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    return TRUE;
}
