#include "pch.h"
#include "AntiInvisibleHacker.h"

#include "IMGUI/imgui_internal.h"

std::string AntiInvisibleHacker::GetPluginName()
{
	return "AntiInvisibleHacker";
}

void AntiInvisibleHacker::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Render the plugin settings here
// This will show up in bakkesmod when the plugin is loaded at
//  f2 -> plugins -> AntiInvisibleHacker
void AntiInvisibleHacker::RenderSettings()
{
	ImGui::Checkbox("Enable Plugin", &EnablePlugin);

	if (!EnablePlugin)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}
	ImGui::Checkbox("Enable Plugin In Replays", &EnablePluginInReplays);
	if (!EnablePlugin)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

	ImGui::Checkbox("Enable Notifications", &EnableNotifications);
	ImGui::Checkbox("Enable Canvas", &EnableCanvas);

	ImGui::NewLine();

	if (ImGui::Button("Reveal Invisible Hackers"))
	{
		gameWrapper->Execute([&](GameWrapper* gw) {
			cvarManager->executeCommand("antiinvisiblehacker_reveal_invisible_hackers");
			});
	}

}

/*
// Do ImGui rendering here
void AntiInvisibleHacker::Render()
{
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string AntiInvisibleHacker::GetMenuName()
{
	return "AntiInvisibleHacker";
}

// Title to give the menu
std::string AntiInvisibleHacker::GetMenuTitle()
{
	return menuTitle_;
}

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void AntiInvisibleHacker::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool AntiInvisibleHacker::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool AntiInvisibleHacker::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void AntiInvisibleHacker::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void AntiInvisibleHacker::OnClose()
{
	isWindowOpen_ = false;
}
*/
