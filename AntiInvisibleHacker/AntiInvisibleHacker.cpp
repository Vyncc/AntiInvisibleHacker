#include "pch.h"
#include "AntiInvisibleHacker.h"

#include "RenderingTools/RenderingTools.h"


BAKKESMOD_PLUGIN(AntiInvisibleHacker, "AntiInvisibleHacker v1.1", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
std::shared_ptr<GameWrapper> _globalGameWrapper;

TArray<class UObject*>* GObjects;
TArray<struct FNameEntry*>* GNames;

void AntiInvisibleHacker::onLoad()
{
	_globalCvarManager = cvarManager;
	_globalGameWrapper = gameWrapper;

	InitGlobals();

	if (AreGlobalsValid())
	{
		cvarManager->registerNotifier("antiinvisiblehacker_reveal_invisible_hackers", [&](std::vector<std::string> args) {
			RevealInvisibleHackers();
			}, "", 0);

		gameWrapper->HookEventWithCallerPost<CarWrapper>("Function TAGame.Car_TA.PostBeginPlay", [this](CarWrapper caller, void* params, std::string eventname) {
			if (!EnablePlugin) return;
			if (gameWrapper->IsInReplay() && !EnablePluginInReplays) return;

			gameWrapper->SetTimeout([&, caller](GameWrapper* gw) mutable {
				RevealInvisibleHacker(caller.GetPRI());
				}, 0.05f);
			});

		gameWrapper->HookEventWithCaller<ServerWrapper>("Function GameEvent_Soccar_TA.Countdown.BeginState", [this](ServerWrapper caller, void* params, std::string eventname) {
			if (!EnableNotifications) return;

			if (caller.GetSecondsRemaining() == 300)
			{
				LOG("Checking for cheaters");

				for (PriWrapper pri : caller.GetPRIs())
				{
					if (!pri) continue;

					if (IsHacker(pri))
					{
						ShowBakkesmodNotification("Cheater detected !", std::string(pri.GetPlayerName().ToString() + " has a good gaming chair!"));
					}
				}
			}
			});

		gameWrapper->RegisterDrawable([this](CanvasWrapper canvas) {
			Render(canvas);
			});
	}
}

bool AntiInvisibleHacker::IsHacker(PriWrapper pri)
{
	uintptr_t loadoutsAddress = pri.memory_address + OFFSET_PRI_LOADOUTS;
	FClientLoadoutDatas* loadoutDatas = reinterpret_cast<FClientLoadoutDatas*>(loadoutsAddress);

	//LOG("Blue loadout ({})", loadoutDatas->Loadouts[0].Products.Num());
	//LOG("Orange loadout ({})", loadoutDatas->Loadouts[1].Products.Num());

	//Check if player is exploiting invisible car hack
	return (loadoutDatas->Loadouts[0].Products.Num() == 0 || loadoutDatas->Loadouts[1].Products.Num() == 0);
}

bool AntiInvisibleHacker::HasNoCar(CarWrapper car)
{
	return (!car.GetbLoadoutSet() && !car.GetBoostComponent() && !car.GetDodgeComponent() &&
		!car.GetAirControlComponent() && !car.GetJumpComponent() && !car.GetDoubleJumpComponent());
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

	if (IsHacker(pri))
	{
		LOG("Hacker detected");

		//check if hacker doesn't have car
		if (HasNoCar(priCar))
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

		if (IsHacker(pri))
		{
			//check if hacker doesn't have car
			if (HasNoCar(priCar))
			{
				uintptr_t locationAddress = priCar.memory_address + OFFSET_ACTOR_LOCATION;
				uintptr_t rotationAddress = priCar.memory_address + OFFSET_ACTOR_ROTATION;
				Vector priCarLocation = *reinterpret_cast<Vector*>(locationAddress);
				Rotator priCarRotation = *reinterpret_cast<Rotator*>(rotationAddress);

				priCarLocation.Z += 10.f;

				canvas.SetColor(255, 255, 255, 255);

				CameraWrapper camera = gameWrapper->GetCamera();
				if (!camera) continue;

				if (RT::Frustum(canvas, camera).IsInFrustum(priCarLocation))
				{
					RT::Cube(priCarLocation, RotatorToQuat(priCarRotation), 75.f, 2.f).Draw(canvas);
				}
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