#include "pch.h"
#include "AntiInvisibleHacker.h"

#include "RenderingTools/RenderingTools.h"


BAKKESMOD_PLUGIN(AntiInvisibleHacker, "AntiInvisibleHacker", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

TArray<class UObject*>* GObjects;
TArray<struct FNameEntry*>* GNames;

void AntiInvisibleHacker::onLoad()
{
	_globalCvarManager = cvarManager;

	InitGlobals();
	
	cvarManager->registerNotifier("antiinvisiblehacker_reveal_invisible_hackers", [&](std::vector<std::string> args) {
		RevealInvisibleHackers();
		}, "", 0);

	gameWrapper->HookEventWithCaller<CarWrapper>("Function TAGame.Car_TA.PostBeginPlay", [this](CarWrapper caller, void* params, std::string eventname) {
		if (!EnablePlugin) return;
		if (gameWrapper->IsInReplay() && !EnablePluginInReplays) return;

		gameWrapper->SetTimeout([&, caller](GameWrapper* gw) mutable {

			RevealInvisibleHacker(caller.GetPRI());
			}, 0.05f);

		});

	gameWrapper->HookEventWithCaller<ServerWrapper>("Function GameEvent_Soccar_TA.Countdown.BeginState", [this](ServerWrapper caller, void* params, std::string eventname) {
		if (!EnableNotifications) return;

		LOG("Checking for cheaters");

		for (PriWrapper pri : caller.GetPRIs())
		{
			if (!pri) continue;

			uintptr_t loadoutsAddress = pri.memory_address + OFFSET_PRI_LOADOUTS;
			FClientLoadoutDatas* loadoutDatas = reinterpret_cast<FClientLoadoutDatas*>(loadoutsAddress);

			//Check if player is exploiting invisible car hack
			if (loadoutDatas->Loadouts[0].Products.Num() == 0 || loadoutDatas->Loadouts[1].Products.Num() == 0)
			{
				ShowBakkesmodNotification("Cheater detected !", std::string(pri.GetPlayerName().ToString() + " has a good gaming chair!"));
			}
		}

		});

	gameWrapper->RegisterDrawable([this](CanvasWrapper canvas) {
		Render(canvas);
		});
}

void AntiInvisibleHacker::RevealInvisibleHacker(PriWrapper pri)
{
	LOG("---------------------------------------");

	if (!pri)
	{
		LOG("pri NULL !");
		return;
	}

	std::string playerName = pri.GetPlayerName().ToString();
	LOG("Checking player : {}", playerName);

	CarWrapper priCar = pri.GetCar();
	if (!priCar)
	{
		LOG("priCar NULL !");
		return;
	}

	uintptr_t loadoutsAddress = pri.memory_address + OFFSET_PRI_LOADOUTS;
	FClientLoadoutDatas* loadoutDatas = reinterpret_cast<FClientLoadoutDatas*>(loadoutsAddress);

	LOG("Blue loadout ({})", loadoutDatas->Loadouts[0].Products.Num());
	LOG("Orange loadout ({})", loadoutDatas->Loadouts[1].Products.Num());

	//Check if player is exploiting invisible car hack
	if (loadoutDatas->Loadouts[0].Products.Num() == 0 || loadoutDatas->Loadouts[1].Products.Num() == 0)
	{
		LOG("Hacker detected");
		//check if hacker doesn't have car
		if (!priCar.GetBoostComponent() && !priCar.GetDodgeComponent() && !priCar.GetAirControlComponent() && !priCar.GetJumpComponent() && !priCar.GetDoubleJumpComponent())
		{
			ExecPatch(priCar);
		}
		else
		{
			LOG("Hacker already has a car");
		}
	}
	else
	{
		LOG("Player is legit");
	}

	LOG("---------------------------------------");
}

void AntiInvisibleHacker::RevealInvisibleHackers()
{
	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (!server)
	{
		LOG("server NULL !");
		return;
	}

	for (PriWrapper pri : server.GetPRIs())
	{
		RevealInvisibleHacker(pri);
	}
}

//This will display a box on the hacker's car only if the patch to spawn the car didn't work
void AntiInvisibleHacker::Render(CanvasWrapper canvas)
{
	if (!EnableCanvas) return;

	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (!server) return;

	for (PriWrapper pri : server.GetPRIs())
	{
		if (!pri) continue;

		CarWrapper priCar = pri.GetCar();
		if (!priCar) continue;

		uintptr_t loadoutsAddress = pri.memory_address + OFFSET_PRI_LOADOUTS;
		FClientLoadoutDatas* loadoutDatas = reinterpret_cast<FClientLoadoutDatas*>(loadoutsAddress);

		//Check if player is exploiting invisible car hack
		if (loadoutDatas->Loadouts[0].Products.Num() == 0 || loadoutDatas->Loadouts[1].Products.Num() == 0)
		{
			//check if hacker doesn't have car
			if (!priCar.GetBoostComponent() && !priCar.GetDodgeComponent() && !priCar.GetAirControlComponent() && !priCar.GetJumpComponent() && !priCar.GetDoubleJumpComponent())
			{
				uintptr_t locationAddress = priCar.memory_address + OFFSET_ACTOR_LOCATION;
				uintptr_t rotationAddress = priCar.memory_address + OFFSET_ACTOR_ROTATION;
				Vector priCarLocation = *reinterpret_cast<Vector*>(locationAddress);
				Rotator priCarRotation = *reinterpret_cast<Rotator*>(rotationAddress);

				priCarLocation.Z += 10.f;

				canvas.SetColor(255, 255, 255, 255);
				RT::Cube(priCarLocation, RotatorToQuat(priCarRotation), 75.f, 2.f).Draw(canvas);
			}
		}
	}
}

void AntiInvisibleHacker::ShowBakkesmodNotification(std::string title, std::string text, float timeout, std::string texture, uint8_t toastType, float width, float height)
{
	CVarWrapper notificationCvar = cvarManager->getCvar("cl_notifications_enabled_beta");
	int valueBefore = notificationCvar.getIntValue();
	notificationCvar.setValue(1);
	gameWrapper->Toast(title, text, texture, timeout, toastType, width, height);
	notificationCvar.setValue(valueBefore); //set the value back
}

void AntiInvisibleHacker::onUnload()
{
}