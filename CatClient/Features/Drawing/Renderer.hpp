#pragma once

#define DEBUGINDICATORS 1

namespace Render {
	inline void doRenderer(ImDrawList* pDraw) {
		/* Watermark */
		pDraw->AddText(ImVec2(10, 10), Utils::RGBCol(ImColor(255, 255, 255, 255)), "CatClient", NULL, 14.0f);
		//pDraw->AddText(ImVec2(10, ImGui::GetIO().DisplaySize.y - 25), Utils::RGBCol(ImColor(255, 255, 255, 255)), ("FPS: " + std::to_string(floorf(1.f / ImGui::GetIO().DeltaTime))).c_str(), NULL, 14.0f);
#if DEBUGINDICATORS
		pDraw->AddText(ImVec2(10, 30), Utils::RGBCol(ImColor(255, 255, 255, 255)), ("menuOpened => " + std::to_string(Menu::menuOpened)).c_str(), NULL, 6.0f);
#endif
	}
}