#include "stdafx.h"
#include "Main.h"

#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

static const char* currentRenderFn = "./addons/SelectRender/active_render.bin";

static const wchar_t* render_gw2hook_d3d9_dll = L"./addons/gw2hook/d3d9.dll";
static const wchar_t* render_d912pxy_d3d9_dll = L"./addons/d912pxy/dll/release/d3d9.dll";
static const wchar_t* render_dxvk_d3d9_dll = L"./addons/dxvk/d3d9.dll";
static const wchar_t* render_dxvk_async_d3d9_dll = L"./addons/dxvk-async/d3d9.dll";

static const wchar_t* render_reshade_d3d9_dll = L"./addons/reshade/d3d9.dll";
static const wchar_t* render_reshade_dxgi_dll = L"./addons/reshade/dxgi.dll";
static const wchar_t* render_reshade_d3d12_dll = L"./addons/reshade/d3d12.dll";

static const wchar_t* render_gw2enhanced_dxgi_dll = L"./addons/gw2enhanced/dxgi.dll";
static const wchar_t* render_gw2enhanced_d3d12_dll = L"./addons/gw2enhanced/d3d12.dll";

wchar_t* GetD3D9CustomLib()
{
	wchar_t* ret = gAddon().main.loadRender();
	gAddon().api->log_text(LL_INFO, (wchar_t*)L"SelectRender", ret);
	return ret;
}

void ExternalShowMenu()
{
	gAddon().main.stopAutoHide();
	gAddon().gui->toggleUI(true);
}

void Main::init()
{
	FILE* f = nullptr;
	fopen_s(&f, currentRenderFn, "r");
	if (f)
	{
		fread(&activeRenderer, sizeof(activeRenderer), 1, f);
		if ((int)activeRenderer >= (int)RenderType::COUNT)
		{
			gAddon().api->log_text(LL_ERR, (wchar_t*)L"SelectRender", (wchar_t*)L"Garbadge in active_render.bin file, using stock d3d9");
			activeRenderer = RenderType::D3D9;
		}
		fclose(f);
	}

	auto nha = gAddon().api->hash_name((wchar_t*)L"D3D_wrapper_custom_d3d9_lib_query");
	if (gAddon().api->register_function(&GetD3D9CustomLib, nha) != GW2AL_OK)
		gAddon().api->log_text(LL_ERR, (wchar_t*)L"SelectRender", (wchar_t*)L"someone already defined custom d3d lib query");

	nha = gAddon().api->hash_name((wchar_t*)L"SelectRender_ExternalShowMenu");
	if (gAddon().api->register_function(&ExternalShowMenu, nha) != GW2AL_OK)
		gAddon().api->log_text(LL_ERR, (wchar_t*)L"SelectRender", (wchar_t*)L"external show menu proc is not registered");

	GetSystemDirectory(sysD3D9, 4096);
	lstrcatW(sysD3D9, L"\\d3d9.dll");
}

void Main::update()
{
	if (!needAutoHide)
		return;
	int cuTime = GetTickCount();
	int dlt = max(cuTime - lastUpd, 0);
	if (!lastUpd)
		dlt = 0;
	lastUpd = cuTime;
	hideTimeout -= dlt;
	if (hideTimeout < 0)
	{
		gAddon().gui->toggleUI(false);
		gAddon().api->log_text(LL_INFO, (wchar_t*)L"SelectRender", (wchar_t*)L"UI hidden due to timeout");
		hideTimeout = 0;
	}
}

bool Main::setRenderer(RenderType v)
{
	bool ret = false;
	FILE* f = nullptr;
	fopen_s(&f, currentRenderFn, "w");

	if (f)
	{
		int typeddt = (int)v;
		if (fwrite(&typeddt, sizeof(typeddt), 1, f) != 1)
			gAddon().api->log_text(LL_ERR, (wchar_t*)L"SelectRender", (wchar_t*)L"Can't write active render file, stock d3d9 will still be used on restart");
		else
			ret = true;
		fclose(f);
	} else 
		gAddon().api->log_text(LL_ERR, (wchar_t*)L"SelectRender", (wchar_t*)L"Can't create active render file, stock d3d9 will still be used on restart");
	
	return ret;
}

wchar_t* Main::loadRender()
{
	gAddon().api->log_text(LL_INFO, (wchar_t*)L"SelectRender", (wchar_t*)L"Loading d3d9");
	switch (activeRenderer)
	{
	case RenderType::D3D9:
	default:
		return sysD3D9;
	case RenderType::D3D9_X_RESHADE:
		return (wchar_t*)render_reshade_d3d9_dll;
	case RenderType::D3D9_X_GW2HOOK:
		return (wchar_t*)render_gw2hook_d3d9_dll;
	case RenderType::D912PXY:
		return (wchar_t*)render_d912pxy_d3d9_dll;
	case RenderType::D912PXY_X_RESHADE:
		extraDllsLoaded &= 
			(LoadLibraryW(render_reshade_dxgi_dll) != NULL) ||
			(LoadLibraryW(render_reshade_d3d12_dll) != NULL);
		return (wchar_t*)render_d912pxy_d3d9_dll;
	case RenderType::D912PXY_X_GW2ENHANCED:
		extraDllsLoaded &=
			(LoadLibraryW(render_gw2enhanced_dxgi_dll) != NULL) ||
			(LoadLibraryW(render_gw2enhanced_d3d12_dll) != NULL);;
		return (wchar_t*)render_d912pxy_d3d9_dll;
	case RenderType::DXVK:
		return (wchar_t*)render_dxvk_d3d9_dll;
	case RenderType::DXVK_X_RESHADE:
		//TODO: see how to load reshade with dxvk from code (or maybe it is not needed at all)
		return (wchar_t*)render_dxvk_d3d9_dll;
	case RenderType::DXVK_ASYNC:
		_putenv_s("DXVK_ASYNC", "1");
		return (wchar_t*)render_dxvk_async_d3d9_dll;
	case RenderType::DXVK_ASYNC_X_RESHADE:
		_putenv_s("DXVK_ASYNC", "1");
		return (wchar_t*)render_dxvk_async_d3d9_dll;
	}
}

void Main::checkAvailabilityUncached()
{
	renderOptionAvailability[(int)RenderType::D3D9] = true;
	renderOptionAvailability[(int)RenderType::D3D9_X_RESHADE] = fileExists(render_reshade_d3d9_dll);
	renderOptionAvailability[(int)RenderType::D3D9_X_GW2HOOK] = fileExists(render_gw2hook_d3d9_dll);
	renderOptionAvailability[(int)RenderType::D912PXY] = fileExists(render_d912pxy_d3d9_dll);
	renderOptionAvailability[(int)RenderType::D912PXY_X_RESHADE] = fileExists(render_d912pxy_d3d9_dll)
		&& (fileExists(render_reshade_dxgi_dll) || fileExists(render_reshade_d3d12_dll));
	renderOptionAvailability[(int)RenderType::D912PXY_X_GW2ENHANCED] = fileExists(render_d912pxy_d3d9_dll)
		&& (fileExists(render_gw2enhanced_dxgi_dll) || fileExists(render_gw2enhanced_d3d12_dll));
	renderOptionAvailability[(int)RenderType::DXVK] = fileExists(render_dxvk_d3d9_dll);
	renderOptionAvailability[(int)RenderType::DXVK_X_RESHADE] = fileExists(render_dxvk_d3d9_dll);
	renderOptionAvailability[(int)RenderType::DXVK_ASYNC] = fileExists(render_dxvk_async_d3d9_dll);
	renderOptionAvailability[(int)RenderType::DXVK_ASYNC_X_RESHADE] = fileExists(render_dxvk_async_d3d9_dll);
}

bool Main::checkAvailability(RenderType index)
{
	if (!renderAvailabilityCached)
		checkAvailabilityUncached();

	if (index >= RenderType::COUNT)
		return false;

	return renderOptionAvailability[(int)index];
}

bool Main::fileExists(const wchar_t* fn)
{
	FILE* f = nullptr;
	_wfopen_s(&f, fn, L"rb");
	if (!f)
		return false;
	
	fclose(f);
	return true;
}
