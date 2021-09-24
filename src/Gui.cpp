#include "stdafx.h"

void Gui::init()
{
	hideTimeout = gAddon().main.getHideTimeoutPtr();
	updateActualActiveRender();
}

void Gui::updateActualActiveRender()
{
	activeRenderIndex = (int)gAddon().main.getActiveRenderer();
	renderIndex = activeRenderIndex;
}

void Gui::draw()
{
	if (!showUI)
		return;

	//should be separated from gui draw, but who cares
	gAddon().main.update();

	ImGui::Begin("SelectRender");

	ImGui::Text("Current:");
	ImGui::SameLine();

	switch (activeRenderIndex)
	{
	case (int)RenderType::D3D9:
		ImGui::Text("stock direct3d 9 renderer");
		break;
	case (int)RenderType::D3D9_X_RESHADE:
		ImGui::Text("stock direct3d 9 renderer with ReShade");
		break;
	case (int)RenderType::D3D9_X_GW2HOOK:
		ImGui::Text("stock direct3d 9 renderer with GW2Hook");
		break;
	case (int)RenderType::D912PXY:
		ImGui::Text("direct3d 12 renderer via d912pxy proxy");
		break;
	case (int)RenderType::D912PXY_X_RESHADE:
		ImGui::Text("direct3d 12 renderer via d912pxy proxy with ReShade");
		break;
	case (int)RenderType::D912PXY_X_GW2ENHANCED:
		ImGui::Text("direct3d 12 renderer via d912pxy proxy with Gw2Enhanced");
		break;
	case (int)RenderType::DXVK:
		ImGui::Text("vulkan renderer via DXVK");
		break;
	case (int)RenderType::DXVK_X_RESHADE:
		ImGui::Text("vulkan renderer via DXVK with ReShade");
		break;
	case (int)RenderType::DXVK_ASYNC:
		ImGui::Text("vulkan renderer via DXVK (async)");
		break;
	case (int)RenderType::DXVK_ASYNC_X_RESHADE:
		ImGui::Text("vulkan renderer via DXVK (async) with ReShade");
		break;
	case (int)RenderType::DXVK11:
		ImGui::Text("vulkan renderer via DXVK D3D11");
		break;
	case (int)RenderType::DXVK11_X_RESHADE:
		ImGui::Text("vulkan renderer via DXVK D3D11 with ReShade");
		break;
	case (int)RenderType::DXVK11_ASYNC:
		ImGui::Text("vulkan renderer via DXVK D3D11 (async) with ReShade");
		break;
	case (int)RenderType::DXVK11_ASYNC_X_RESHADE:
		ImGui::Text("vulkan renderer via DXVK D3D11 (async) with ReShade");
		break;
	case (int)RenderType::D3D11:
		ImGui::Text("stock direct3d 11 renderer");
		break;
	case (int)RenderType::D3D11_X_RESHADE:
		ImGui::Text("stock direct3d 11 renderer with ReShade");
		break;
	default:
		ImGui::Text("current render selection is broken...");
		break;
	}

	if (!gAddon().main.isExtraDllsLoaded())
	{
		ImGui::Text("Some extra dlls are not loaded!");
		ImGui::Text("Check integrity of selected renderer installation");
	}

	if (showList)
	{
		ImGui::NewLine();
		drawSeletionOption("D3D9", &renderIndex, RenderType::D3D9);
		drawSeletionOption("D3D9 + ReShade", &renderIndex, RenderType::D3D9_X_RESHADE);
		drawSeletionOption("D3D9 + GW2Hook", &renderIndex, RenderType::D3D9_X_GW2HOOK);
		drawSeletionOption("d912pxy", &renderIndex, RenderType::D912PXY);
		drawSeletionOption("d912pxy + ReShade", &renderIndex, RenderType::D912PXY_X_RESHADE);
		drawSeletionOption("d912pxy + Gw2Enhanced", &renderIndex, RenderType::D912PXY_X_GW2ENHANCED);
		drawSeletionOption("DXVK", &renderIndex, RenderType::DXVK);
		drawSeletionOption("DXVK + ReShade", &renderIndex, RenderType::DXVK_X_RESHADE);
		drawSeletionOption("DXVK (async)", &renderIndex, RenderType::DXVK_ASYNC);
		drawSeletionOption("DXVK (async) + ReShade", &renderIndex, RenderType::DXVK_ASYNC_X_RESHADE);
		drawSeletionOption("DXVK (for D3D11)", &renderIndex, RenderType::DXVK11);
		drawSeletionOption("DXVK + ReShade (for D3D11)", &renderIndex, RenderType::DXVK11_X_RESHADE);
		drawSeletionOption("DXVK (async, for D3D11)", &renderIndex, RenderType::DXVK11_ASYNC);
		drawSeletionOption("DXVK + ReShade (async, for D3D11)", &renderIndex, RenderType::DXVK11_ASYNC_X_RESHADE);
		drawSeletionOption("D3D11", &renderIndex, RenderType::D3D11);
		drawSeletionOption("D3D11 + ReShade", &renderIndex, RenderType::D3D11_X_RESHADE);
		ImGui::NewLine();
		if (ImGui::Button("Apply"))
		{
			showRestartNotify = gAddon().main.setRenderer((RenderType)renderIndex);
			showErrorSaveNotify = !showRestartNotify;
		}

		if (showRestartNotify)
			ImGui::Text("Changes will be applied after game restart");

		if (showErrorSaveNotify)
			ImGui::Text("Failed to write binary data to addons/SelectRender directory, changes are not applied");
	}
	else
	{
		if (ImGui::Button("Change"))
		{
			gAddon().main.stopAutoHide();
			showList = true;
		}
	}
	
	if (*hideTimeout)
	{
		ImGui::NewLine();
		ImGui::Text("Will auto hide in %u seconds", (*hideTimeout)/1000);
	}
	if (ImGui::Button("Hide"))
		toggleUI(false);
	ImGui::End();
}

void Gui::deinit()
{
	//CHANGE ME
}

void Gui::drawSeletionOption(const char* txt, int* v, RenderType v_button)
{
	if (gAddon().main.checkAvailability(v_button))
		ImGui::RadioButton(txt, v, (int)v_button);
	else
		ImGui::Text("   %s (N/A)", txt);
}
