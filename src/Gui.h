#pragma once
#include "stdafx.h"

class Gui
{
	bool showUI = true;
	int renderIndex = (int)RenderType::D3D9;
	int activeRenderIndex = (int)RenderType::D3D9;
	int* hideTimeout = nullptr;
	bool showList = false;
	bool showRestartNotify = false;
	bool showErrorSaveNotify = false;

public:
	void toggleUI(bool v) { showUI = v; }

	void init();
	void updateActualActiveRender();
	void draw();
	void deinit();
	void drawSeletionOption(const char* txt, int* v, RenderType v_button);
};

