#pragma once

namespace Menu {
	inline bool menuOpened = false;
	inline void doMenu() {
		ImDrawList* pDraw = ImGui::GetForegroundDrawList();
		if (!pDraw)
			return;

		float yPos = 0.f;
		bool wasActivated = false;
		static bool didOnce = false;
		const int maxSelectedTabs = 1;
		static int currentlySelectedTab = 0;
		
		if (Input::IsKeyDown(VK_DOWN) && !didOnce) { currentlySelectedTab += 1; didOnce = true; wasActivated = false; }
		if (Input::IsKeyDown(VK_UP) && !didOnce) { currentlySelectedTab -= 1; didOnce = true; wasActivated = false; }
		if (Input::IsKeyDown(VK_RIGHT) && !didOnce) { wasActivated = true; didOnce = true; }

		currentlySelectedTab = std::clamp(currentlySelectedTab, 0, maxSelectedTabs);

		CustomDrawList::InitMenuDrawer(4.f);

		/* Colors */
		ImColor selectableGUIClr = ImColor(25.f, 25.f, 25.f);
		ImColor selectableOffClr = ImColor(135.f, 135.f, 135.f);
		ImColor selectableOnClr = ImColor(180.f, 180.f, 180.f);
		ImColor selectableActiveClr = ImColor(100.f, 105.f, 210.f);

		/* Watermark */
		CustomDrawList::RectText(ImVec2(2.f, yPos), "CatClient", ImColor(110.f, 100.f, 130.f), selectableGUIClr); yPos += 20.f;
		CustomDrawList::SelectableBox(ImVec2(2.f, yPos), "FPS", currentlySelectedTab, 0, wasActivated, selectableGUIClr, selectableOffClr, selectableOnClr, selectableActiveClr, Variables::ShowFPS); yPos += 20.f;
		//CustomDrawList::SelectableBox(ImVec2(2.f, yPos), "CPS", currentlySelectedTab, 1, wasActivated, selectableGUIClr, selectableOffClr, selectableOnClr, selectableActiveClr, Variables::ShowCPS); yPos += 20.f;
		//CustomDrawList::SelectableBox(ImVec2(5.f, yPos), "Debug", currentlySelectedTab, 1, wasActivated, selectableGUIClr, selectableOffClr, selectableOnClr, selectableActiveClr, Variables::debugIndicators); yPos += 20.f;

		if (didOnce && !(Input::IsKeyDown(VK_DOWN) || Input::IsKeyDown(VK_UP) || Input::IsKeyDown(VK_RIGHT))) {
			didOnce = false;
		}
	}
}