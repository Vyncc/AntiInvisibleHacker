#include "pch.h"
#include "AntiInvisibleHacker.h"

#include "IMGUI/imgui_internal.h"
#include <shellapi.h> //for ShellExecute

std::string AntiInvisibleHacker::GetPluginName()
{
	return "AntiInvisibleHacker v1.2";
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
		RenderUnderLine(ImColor(255, 255, 252, 170));
		ImGui::Text("This plugin will only work for the Rocket League version v2.52.\nSo I recommend you to remove the plugin from your plugins folder once Psyonix releases a new Rocket League update, as it might crash your game due to the changes\nPsyonix will make to the game.");
	}
	else
	{
		ImGui::Text("This plugin has been made to only work for the Rocket League version v2.52 and won't work for the current version you are playing on.");
		ImGui::Text("I recommend you to remove the plugin from your plugins folder.");
	}

	ImGui::NewLine();
	ImGui::NewLine();

	ImGui::Text("Plugin made by");
	ImGui::SameLine();
	RenderLink("https://x.com/Vync5");
}

void AntiInvisibleHacker::RenderLink(std::string link)
{
	std::wstring w_LINK = s2ws(link);
	LPCWSTR L_LINK = w_LINK.c_str();

	ImGui::TextColored(ImColor(3, 94, 252, 255), link.c_str());
	RenderUnderLine(ImColor(3, 94, 252, 255));
	if (ImGui::IsItemHovered())
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		if (ImGui::IsMouseClicked(0))
		{
			ShellExecute(0, 0, L_LINK, 0, 0, SW_SHOW); //open link in web browser
		}
		RenderUnderLine(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
	}
}

//https://gist.github.com/dougbinks/ef0962ef6ebe2cadae76c4e9f0586c69
void AntiInvisibleHacker::RenderUnderLine(ImColor col_)
{
	ImVec2 min = ImGui::GetItemRectMin();
	ImVec2 max = ImGui::GetItemRectMax();
	min.y = max.y;
	ImGui::GetWindowDrawList()->AddLine(min, max, col_, 1.0f);
}

std::wstring AntiInvisibleHacker::s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
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
