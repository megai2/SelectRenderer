#include "stdafx.h"

void Gui::init()
{
	hideTimeout = gAddon().main.getHideTimeoutPtr();
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
	default:
		ImGui::Text("current render selection is broken...");
		break;
	}

	if (showList)
	{
		ImGui::RadioButton("D3D9", &renderIndex, (int)RenderType::D3D9);
		ImGui::RadioButton("D3D9 + ReShade", &renderIndex, (int)RenderType::D3D9_X_RESHADE);
		ImGui::RadioButton("D3D9 + GW2Hook", &renderIndex, (int)RenderType::D3D9_X_GW2HOOK);
		ImGui::RadioButton("d912pxy", &renderIndex, (int)RenderType::D912PXY);
		ImGui::RadioButton("d912pxy + ReShade", &renderIndex, (int)RenderType::D912PXY_X_RESHADE);
		ImGui::RadioButton("d912pxy + Gw2Enhanced", &renderIndex, (int)RenderType::D912PXY_X_GW2ENHANCED);
		ImGui::RadioButton("DXVK", &renderIndex, (int)RenderType::DXVK);
		ImGui::RadioButton("DXVK + ReShade", &renderIndex, (int)RenderType::DXVK_X_RESHADE);
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
