#pragma once

namespace Indicator {
	inline void Render() {
		int FramesPerSecond = floorf(1.f / ImGui::GetIO().DeltaTime);
		ImColor pColorIndicator = ImColor(130.f, 120.f, 160.f);
		ImColor pColorGUIIndicator = ImColor(25.f, 25.f, 25.f);

		/* Initialize the Drawer */
		CustomDrawList::InitDrawer(5.f);

		float yPos = ImGui::GetIO().DisplaySize.y - 60.f; /* This is only accurate in fullscreen TODO: fix it so it automatically aligns with the window size */

		/* FPS */
		if (Variables::ShowFPS) {
			CustomDrawList::RectText(ImVec2(2.f, yPos), "FPS: " + std::to_string(FramesPerSecond), pColorIndicator, pColorGUIIndicator); yPos -= 20.f;
		}
		if (Variables::ShowCPS) {
			CustomDrawList::RectText(ImVec2(2.f, yPos), "CPS: " + std::to_string(Input::validClicks[0]) + " | " + std::to_string(Input::validClicks[1]), pColorIndicator, pColorGUIIndicator); yPos -= 20.f;
		}
		if (Variables::ShowKeystrokes) {
			ImColor pColorKeystrokeGUI = ImColor(20.f, 20.f, 20.f, 200.f);
			ImColor pColorKeystrokeActive = ImColor(50.f, 50.f, 50.f, 170.f);
			float keystrokeXPosition = ImGui::GetIO().DisplaySize.x - 78.f;
			float keystrokeYPosition = ImGui::GetIO().DisplaySize.y - 160.f;
			CustomDrawList::drawKeystroke(ImVec2(keystrokeXPosition, keystrokeYPosition), "W", Input::IsKeyDown('W'), false, pColorIndicator, pColorKeystrokeActive, pColorKeystrokeGUI); keystrokeYPosition += 40.f; //no distance is 36.f
			CustomDrawList::drawKeystroke(ImVec2(keystrokeXPosition, keystrokeYPosition), "S", Input::IsKeyDown('S'), false, pColorIndicator, pColorKeystrokeActive, pColorKeystrokeGUI); keystrokeXPosition -= 39.f; //no distance is 35.f
			CustomDrawList::drawKeystroke(ImVec2(keystrokeXPosition, keystrokeYPosition + 40.f), "Space", Input::IsKeyDown(VK_SPACE), true, pColorIndicator, pColorKeystrokeActive, pColorKeystrokeGUI);
			CustomDrawList::drawKeystroke(ImVec2(keystrokeXPosition, keystrokeYPosition), "A", Input::IsKeyDown('A'), false, pColorIndicator, pColorKeystrokeActive, pColorKeystrokeGUI); keystrokeXPosition += 78.f; //no distance is 74.f
			CustomDrawList::drawKeystroke(ImVec2(keystrokeXPosition, keystrokeYPosition), "D", Input::IsKeyDown('D'), false, pColorIndicator, pColorKeystrokeActive, pColorKeystrokeGUI);
		}

		/* Debug Indicators */
		if (Variables::debugIndicators) {
			CustomDrawList::RectText(ImVec2(2.f, yPos), "menuOpened => " + std::to_string(Menu::menuOpened), pColorIndicator, pColorGUIIndicator); yPos -= 20.f;
			CustomDrawList::RectText(ImVec2(2.f, yPos), "Down => " + std::to_string(Input::IsKeyDown(VK_INSERT)), pColorIndicator, pColorGUIIndicator); yPos -= 20.f;
			CustomDrawList::RectText(ImVec2(2.f, yPos), "Released => " + std::to_string(Input::IsKeyReleased(VK_INSERT)), pColorIndicator, pColorGUIIndicator); yPos -= 20.f; //this doesnt turn into one because of being too fast i guess?
			CustomDrawList::RectText(ImVec2(2.f, yPos), "Toggled => " + std::to_string(Input::IsKeyToggled(VK_INSERT)), pColorIndicator, pColorGUIIndicator); yPos -= 20.f;
		}
	}
}