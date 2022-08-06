#pragma once

namespace Variables {
	inline bool ShowFPS = false;
	inline bool ShowCPS = false;
	inline bool debugIndicators = false;
	inline bool Zoom = false;
	inline bool Fullbright = false; // not implemented
	inline bool ShowKeystrokes = false;
}

namespace CustomDrawList { /* Made for an Easier Way to Draw */
	inline float fontSize = 1.f;
	inline ImDrawList* pDrawList = nullptr;
	inline ImFont* pFont = nullptr;
	inline void InitDrawer(float customFontSize = 1.f, ImFont* customFont = ImGui::GetFont()) {
		pDrawList = ImGui::GetBackgroundDrawList();
		pFont = customFont;
		fontSize = customFontSize;
		return;
	}

	inline void drawText(ImVec2 pTextPos, std::string pText, ImColor pColor = ImColor(255.f, 255.f, 255.f)) {
		pDrawList->AddText(pFont, fontSize, pTextPos, ImColor(pColor.Value.x / 255.f, pColor.Value.y / 255.f, pColor.Value.z / 255.f, pColor.Value.w > 1.f ? pColor.Value.w / 255.f : pColor.Value.w), pText.c_str());
	}

	inline void drawLine(ImVec2 vecFrom, ImVec2 vecTo, ImColor pColor = ImColor(255.f, 255.f, 255.f), float fThickness = 1.f) {
		pDrawList->AddLine(vecFrom, vecTo, ImColor(pColor.Value.x / 255.f, pColor.Value.y / 255.f, pColor.Value.z / 255.f, pColor.Value.w > 1.f ? pColor.Value.w / 255.f : pColor.Value.w), fThickness);
	}

	inline void drawRect(ImVec2 vecFrom, ImVec2 vecTo, ImColor pColor = ImColor(255.f, 255.f, 255.f), float rounding = 0.0f, ImDrawFlags flags = 0, float fThickness = 1.f) {
		pDrawList->AddRect(vecFrom, vecTo, ImColor(pColor.Value.x / 255.f, pColor.Value.y / 255.f, pColor.Value.z / 255.f, pColor.Value.w > 1.f ? pColor.Value.w / 255.f : pColor.Value.w), rounding, flags, fThickness);
	}

	inline void drawFilledRect(ImVec2 vecFrom, ImVec2 vecTo, ImColor pColor = ImColor(255.f, 255.f, 255.f), float rounding = 0.0f, ImDrawFlags flags = 0) {
		pDrawList->AddRectFilled(vecFrom, vecTo, ImColor(pColor.Value.x / 255.f, pColor.Value.y / 255.f, pColor.Value.z / 255.f, pColor.Value.w > 1.f ? pColor.Value.w / 255.f : pColor.Value.w), rounding, flags);
	}

	inline ImVec2 calcTextSize(std::string pText) {
		return pFont->CalcTextSizeA(fontSize, 1000.f, -1.f, pText.c_str());
	}

	/* Menu Stuff */
	inline void InitMenuDrawer(float customFontSize = 1.f, ImFont* customFont = ImGui::GetFont()) {
		pFont = customFont;
		fontSize = customFontSize;
		return;
	}

	inline void SelectableText(ImVec2 pTextPos, std::string pText, int currentSelected, int neededSelected, bool IsActive, ImColor pColorOff, ImColor pColorOn, ImColor pColorActive, bool& variable) {
		if (IsActive && currentSelected == neededSelected) {
			variable = !variable;
		}

		drawText(pTextPos, pText, variable ? pColorActive : currentSelected != neededSelected ? pColorOff : pColorOn);
	}

	inline void SelectableBox(ImVec2 vecPos, std::string pText, int currentSelected, int neededSelected, bool IsActive, ImColor pColorGUI, ImColor pColorOff, ImColor pColorOn, ImColor pColorActive, bool& variable) {
		if (IsActive && currentSelected == neededSelected) {
			variable = !variable;
		}

		drawRect(ImVec2(vecPos.x - 3.f, vecPos.y), ImVec2(vecPos.x + 96.f, vecPos.y + 21.f), pColorGUI, 0.0f, 0, 1.6f);
		drawRect(ImVec2(vecPos.x - 2.8f, vecPos.y + 0.8f), ImVec2(vecPos.x + 96.8f, vecPos.y + 21.8f), ImColor(100.f, 100.f, 100.f, 25.f), 0.0f, 0, 1.6f); /* Shadow */
		drawFilledRect(ImVec2(vecPos.x - 2.f, vecPos.y + 1.f), ImVec2(vecPos.x + 95.f, vecPos.y + 20.f), ImColor(pColorGUI.Value.x, pColorGUI.Value.y, pColorGUI.Value.z, 175.f), 0.0f, 0);
		drawText(vecPos, pText, variable ? pColorActive : currentSelected != neededSelected ? pColorOff : pColorOn);
	}

	inline void RectText(ImVec2 vecPos, std::string pText, ImColor pColorText, ImColor pColorGUI) {
		drawRect(ImVec2(vecPos.x - 3.f, vecPos.y), ImVec2(vecPos.x + 96.f, vecPos.y + 21.f), pColorGUI, 0.0f, 0, 1.6f);
		drawRect(ImVec2(vecPos.x - 2.8f, vecPos.y + 0.8f), ImVec2(vecPos.x + 96.8f, vecPos.y + 21.8f), ImColor(100.f, 100.f, 100.f, 25.f), 0.0f, 0, 1.6f); /* Shadow */
		drawFilledRect(ImVec2(vecPos.x - 2.f, vecPos.y + 1.f), ImVec2(vecPos.x + 95.f, vecPos.y + 20.f), ImColor(pColorGUI.Value.x, pColorGUI.Value.y, pColorGUI.Value.z, 175.f), 0.0f, 0);
		drawText(vecPos, pText, pColorText);
	}

	inline void drawKeystroke(ImVec2 vecPos, std::string pText, bool IsActive, bool IsSpace, ImColor pColorText, ImColor pColorActive, ImColor pColorGUI) {
		if (!IsSpace) {
			drawRect(ImVec2(vecPos.x - 3.f, vecPos.y - 4.f), ImVec2(vecPos.x + 32.f, vecPos.y + 32.f), pColorGUI, 0.0f, 0, 1.6f);
			drawRect(ImVec2(vecPos.x - 2.8f, vecPos.y - 3.2f), ImVec2(vecPos.x + 32.8f, vecPos.y + 32.8f), ImColor(100.f, 100.f, 100.f, 25.f), 0.0f, 0, 1.6f); /* Shadow */
			drawFilledRect(ImVec2(vecPos.x - 3.f, vecPos.y - 4.f), ImVec2(vecPos.x + 32.f, vecPos.y + 32.f), IsActive ? ImColor(pColorActive.Value.x, pColorActive.Value.y, pColorActive.Value.z, pColorActive.Value.w) : ImColor(pColorGUI.Value.x, pColorGUI.Value.y, pColorGUI.Value.z, 175.f), 0.0f, 0);
			drawText(ImVec2((vecPos.x + 11.f) - calcTextSize(pText).x, (vecPos.y + 7.f) - calcTextSize(pText).y), pText, pColorText);
		}
		else {
			drawRect(ImVec2(vecPos.x - 3.f, vecPos.y - 4.f), ImVec2(vecPos.x + 110.f, vecPos.y + 32.f), pColorGUI, 0.0f, 0, 1.6f);
			drawRect(ImVec2(vecPos.x - 2.8f, vecPos.y - 3.2f), ImVec2(vecPos.x + 110.8f, vecPos.y + 32.8f), ImColor(100.f, 100.f, 100.f, 25.f), 0.0f, 0, 1.6f); /* Shadow */
			drawFilledRect(ImVec2(vecPos.x - 3.f, vecPos.y - 4.f), ImVec2(vecPos.x + 110.f, vecPos.y + 32.f), IsActive ? ImColor(pColorActive.Value.x, pColorActive.Value.y, pColorActive.Value.z, pColorActive.Value.w) : ImColor(pColorGUI.Value.x, pColorGUI.Value.y, pColorGUI.Value.z, 175.f), 0.0f, 0);
			drawText(ImVec2((vecPos.x + 42.f) - calcTextSize(pText).x, (vecPos.y + 7.f) - calcTextSize(pText).y), pText, pColorText);
		}
	}
}