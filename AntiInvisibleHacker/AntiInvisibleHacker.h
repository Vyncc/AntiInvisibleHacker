#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

#include "RLSDK_STUFF.h"

class AntiInvisibleHacker: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow/*, public BakkesMod::Plugin::PluginWindow*/
{
	bool EnablePlugin = true;
	bool EnablePluginInReplays = true;
	bool EnableNotifications = true;
	bool EnableCanvas = false;

	bool IsHacker(PriWrapper pri);
	bool HasNoCar(CarWrapper car);
	void RevealInvisibleHacker(PriWrapper pri);
	void RevealInvisibleHackers();

	void Render(CanvasWrapper canvas);

	//Utils
	void ShowBakkesmodNotification(std::string title, std::string text, float timeout = 3.5f, std::string texture = "default", uint8_t toastType = 0, float width = 290.f, float height = 60.f);


	void RenderLink(std::string link);
	void RenderUnderLine(ImColor col_);
	std::wstring s2ws(const std::string& s);

	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();

	// Inherited via PluginSettingsWindow
	void RenderSettings() override;
	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;

	// Inherited via PluginWindow
	/*

	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menuTitle_ = "AntiInvisibleHacker";

	virtual void Render() override;
	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	virtual void SetImGuiContext(uintptr_t ctx) override;
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;
	
	*/
};

