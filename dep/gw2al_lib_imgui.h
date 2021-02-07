#pragma once

#include <gw2al_api.h>
#include <imgui.h>

#define LIB_IMGUI_DEP_ENTRY { L"lib_imgui", L" ", 1, 0, 1, 0 }

template<typename UserHandler>
class lib_imgui
{
	const wchar_t* getContextExportName = L"lib_imgui_get_context";
	const wchar_t* guiDrawEventName = L"lib_imgui_draw_event";

	gw2al_hashed_name getContextExportNameHash = 0;
	gw2al_hashed_name guiDrawEventNameHash = 0;
	gw2al_event_id guiDrawEventId = 0;

	static lib_imgui<UserHandler>& instance();

	typedef void* (*procType)();
	void* (*getImguiContextProc)();
	static void drawEventHandler(void* imguiContext)
	{
		ImGui::SetCurrentContext((ImGuiContext*)imguiContext);
		instance().handler.draw();
	}

	gw2al_core_vtable* gAPI = nullptr;
	gw2al_hashed_name subscriber = 0;

	UserHandler handler;

	void initInternal(gw2al_core_vtable* in_api, gw2al_hashed_name addonNameHash, unsigned int priorityOverride)
	{
		gAPI = in_api;

		getContextExportNameHash = gAPI->hash_name((wchar_t*)getContextExportName);
		guiDrawEventNameHash = gAPI->hash_name((wchar_t*)guiDrawEventName);
		guiDrawEventId = gAPI->query_event(guiDrawEventNameHash);

		getImguiContextProc = (procType)gAPI->query_function(getContextExportNameHash);
		subscriber = addonNameHash;
		gAPI->watch_event(guiDrawEventId, addonNameHash, &drawEventHandler, priorityOverride);

		ImGui::SetCurrentContext((ImGuiContext*)getImguiContextProc());
		handler.init();
	}

public:
	void init(gw2al_core_vtable* in_api, const wchar_t* addonName, unsigned int priorityOverride = 0)
	{
		initInternal(in_api, in_api->hash_name((wchar_t*)addonName), priorityOverride);
	}

	void init(gw2al_core_vtable* in_api, gw2al_hashed_name addonNameHash, unsigned int priorityOverride = 0)
	{
		initInternal(in_api, addonNameHash, priorityOverride);
	}

	void deinit()
	{
		gAPI->unwatch_event(guiDrawEventId, subscriber);
		handler.deinit();
	}

	UserHandler* operator->()
	{
		return &handler;
	}

	ImGuiContext* getImguiContext()
	{
		return (ImGuiContext*)getImguiContextProc();
	}
};

/*
class UserInterface
{
	UserInterface() = default;

	void init()
	{
	  //called after lib_imgui init with setted up imgui context
	}

	void draw()
	{
	  //called every frame to draw interface if not hidden
	}

	void deinit()
	{
	  //called at deinit
	}
}

//somewhere in addon instance

lib_imgui<UserInterface> ui;

template<>
inline lib_imgui<Gui>& lib_imgui<Gui>::instance()
{
	return ui;
}

*/