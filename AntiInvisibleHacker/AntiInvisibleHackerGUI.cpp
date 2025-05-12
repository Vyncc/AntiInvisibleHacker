#include "pch.h"
#include "AntiInvisibleHacker.h"

#include "IMGUI/imgui_internal.h"

std::string AntiInvisibleHacker::GetPluginName()
{
	return "AntiInvisibleHacker v1.1";
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
	if (AreGlobalsValid())
	{
		if (ImGui::Checkbox("Enable Plugin", &EnablePlugin))
		{
			if (!EnablePlugin)
			{
				EnablePluginInReplays = false;
			}
		}

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
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("It will display a box on the cheater, in case the car doesn't spawn");
			ImGui::EndTooltip();
		}

		ImGui::NewLine();

		if (ImGui::Button("Reveal Invisible Hackers"))
		{
			gameWrapper->Execute([&](GameWrapper* gw) {
				cvarManager->executeCommand("antiinvisiblehacker_reveal_invisible_hackers");
				});
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Click here if somehow the plugin doesn't automatically reveal the cheater");
			ImGui::Text("It should not happen, but just in case");
			ImGui::EndTooltip();
		}

		ImGui::NewLine();
		ImGui::Separator();

		ImGui::Text("Important note :");
		ImGui::Text("This plugin will only work for the Rocket League version v2.51.\nSo I recommend you to remove the plugin from your plugins folder once Psyonix releases a new Rocket League update, as it might crash your game due to the changes\nPsyonix will make to the game.");
	}
	else
	{
		ImGui::Text("This plugin has been made to only work for the Rocket League version v2.51 and won't work for the current version you are playing on.");
		ImGui::Text("I recommend you to remove the plugin from your plugins folder.");
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
