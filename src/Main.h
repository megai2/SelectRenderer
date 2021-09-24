#pragma once
#include "stdafx.h"

enum class RenderType
{
	D3D9,
	D3D9_X_RESHADE,
	D3D9_X_GW2HOOK,
	D912PXY,
	D912PXY_X_RESHADE,
	D912PXY_X_GW2ENHANCED,
	DXVK,
	DXVK_X_RESHADE,
	DXVK_ASYNC,
	DXVK_ASYNC_X_RESHADE,
	DXVK11,
	DXVK11_X_RESHADE,
	DXVK11_ASYNC,
	DXVK11_ASYNC_X_RESHADE,
	D3D11,
	D3D11_X_RESHADE,
	COUNT
};

class Main
{
	int hideTimeout = 15000;
	int lastUpd = 0;
	bool needAutoHide = true;
	bool extraDllsLoaded = true;
	bool usingDX11base = false;
	RenderType activeRenderer = RenderType::D3D9;

	wchar_t sysD3D9[4096];
	wchar_t sysD3D11[4096];
	wchar_t sysDXGI[4096];

	bool renderOptionAvailability[(int)RenderType::COUNT] = { 0 };
	bool renderAvailabilityCached = false;

public:
	int* getHideTimeoutPtr() { return &hideTimeout; }
	void stopAutoHide() { needAutoHide = false; hideTimeout = 0; }
	void init();
	void update();
	bool setRenderer(RenderType v);
	RenderType getActiveRenderer() { return activeRenderer;  }
	void deinit() { }

	bool isExtraDllsLoaded() { return extraDllsLoaded; }

	wchar_t* selectRenderD3D9();
	const wchar_t* selectRenderD3D11();
	const wchar_t* selectRenderDXGI();
	void checkAvailabilityUncached();
	bool checkAvailability(RenderType index);

	bool fileExists(const wchar_t* fn);
};

