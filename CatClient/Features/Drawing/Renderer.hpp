#pragma once

#define DEBUGINDICATORS 0
namespace CustomDrawList { /* Made for an Easier Way to Draw and for the Later Menu if i ever am gonna be Able to get Input */
	inline float fontSize = 1.f;
	inline ImDrawList* pDrawList = nullptr;
	inline ImFont* pFont = Menu::drawFont;
	inline void InitDrawer(float customFontSize = 1.f, ImFont* customFont = ImGui::GetFont()) {
		pDrawList = ImGui::GetBackgroundDrawList();
		fontSize = customFontSize;
		return;
	}

	inline void drawText(ImVec2 pTextPos, std::string pText, ImColor pColor = ImColor(255.f, 255.f, 255.f)) {
		pDrawList->AddText(pFont, fontSize, pTextPos, ImColor(pColor.Value.x / 255.f, pColor.Value.y / 255.f, pColor.Value.z / 255.f, pColor.Value.w), pText.c_str());
	}

	inline void drawLine(ImVec2 vecFrom, ImVec2 vecTo, ImColor pColor = ImColor(255.f, 255.f, 255.f), float fThickness = 1.f) {
		pDrawList->AddLine(vecFrom, vecTo, ImColor(pColor.Value.x / 255.f, pColor.Value.y / 255.f, pColor.Value.z / 255.f, pColor.Value.w), fThickness);
	}

	inline void drawRect(ImVec2 vecFrom, ImVec2 vecTo, ImColor pColor = ImColor(255.f, 255.f, 255.f), float rounding = 0.0f, ImDrawFlags flags = 0, float fThickness = 1.f) {
		pDrawList->AddRect(vecFrom, vecTo, ImColor(pColor.Value.x / 255.f, pColor.Value.y / 255.f, pColor.Value.z / 255.f, pColor.Value.w), rounding, flags, fThickness);
	}

	inline void drawFilledRect(ImVec2 vecFrom, ImVec2 vecTo, ImColor pColor = ImColor(255.f, 255.f, 255.f), float rounding = 0.0f, ImDrawFlags flags = 0) {
		pDrawList->AddRectFilled(vecFrom, vecTo, ImColor(pColor.Value.x / 255.f, pColor.Value.y / 255.f, pColor.Value.z / 255.f, pColor.Value.w), rounding, flags);
	}

	inline ImVec2 calcTextSize(std::string pText) {
		return pFont->CalcTextSizeA(fontSize, 1000.f, -1.f, pText.c_str());
	};
}
namespace Render {
	inline void doRenderer() {
		int FramesPerSecond = floorf(1.f / ImGui::GetIO().DeltaTime);
		float yPos = 0.f;

		/* Initialize the Drawer */
		CustomDrawList::InitDrawer(5.f);

		/* Watermark */
		CustomDrawList::drawText(ImVec2(5.f, yPos), "CatClient"); yPos += 20.f;

		/* FPS */
		CustomDrawList::drawText(ImVec2(5.f, yPos), "FPS: " + std::to_string(FramesPerSecond)); yPos += 20.f;
	}
}