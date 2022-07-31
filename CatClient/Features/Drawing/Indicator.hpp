#pragma once

namespace Indicator {
	inline void Render() {
		int FramesPerSecond = floorf(1.f / ImGui::GetIO().DeltaTime);
		ImColor pColorIndicator = ImColor(130.f, 120.f, 160.f);

		/* Initialize the Drawer */
		CustomDrawList::InitDrawer(5.f);

		float yPos = ImGui::GetIO().DisplaySize.y - 60.f; /* This is only accurate in fullscreen TODO: fix it so it automatically aligns with the window size */

		/* FPS */
		if (Variables::ShowFPS) {
			CustomDrawList::RectText(ImVec2(2.f, yPos), "FPS: " + std::to_string(FramesPerSecond), pColorIndicator, ImColor(25.f, 25.f, 25.f)); yPos -= 20.f;
		}

		/* Debug Indicators */
		if (Variables::debugIndicators) {
			CustomDrawList::RectText(ImVec2(2.f, yPos), "ShowFPS => " + std::to_string(Variables::ShowFPS), pColorIndicator, ImColor(25.f, 25.f, 25.f)); yPos -= 20.f;
			CustomDrawList::RectText(ImVec2(2.f, yPos), "menuOpened => " + std::to_string(Menu::menuOpened), pColorIndicator, ImColor(25.f, 25.f, 25.f)); yPos -= 20.f;
			CustomDrawList::RectText(ImVec2(2.f, yPos), "Down => " + std::to_string(Input::IsKeyDown(VK_INSERT)), pColorIndicator, ImColor(25.f, 25.f, 25.f)); yPos -= 20.f;
			CustomDrawList::RectText(ImVec2(2.f, yPos), "Released => " + std::to_string(Input::IsKeyReleased(VK_INSERT)), pColorIndicator, ImColor(25.f, 25.f, 25.f)); yPos -= 20.f; //this doesnt turn into one because of being too fast i guess?
			CustomDrawList::RectText(ImVec2(2.f, yPos), "Toggled => " + std::to_string(Input::IsKeyToggled(VK_INSERT)), pColorIndicator, ImColor(25.f, 25.f, 25.f)); yPos -= 20.f;
		}
	}
}