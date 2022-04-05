#include "NFAC.h"

static inline bool toggle = true;

void Loki::NoFollowerAttackCollision::InstallMeleeHook() {
	REL::Relocation<std::uintptr_t> MeleeHook{ REL::ID(37650) }; //+38B

	auto& trmp = SKSE::GetTrampoline();
	_MeleeFunction = trmp.write_call<5>(MeleeHook.address() + 0x38B, MeleeFunction);

	logger::info("Unk Hook injected");
}

void Loki::NoFollowerAttackCollision::InstallSweepHook() {
	REL::Relocation<std::uintptr_t> SweepHook{ REL::ID(37689) }; //+DD

	auto& trmp = SKSE::GetTrampoline();
	_SweepFunction = trmp.write_call<5>(SweepHook.address() + 0xDD, SweepFunction);

	logger::info("Unk Hook 2 injected");
}

void Loki::NoFollowerAttackCollision::InstallArrowHook() {
	REL::Relocation<std::uintptr_t> arrowHook{ REL::ID(43027) }; //+90

	auto& trmp = SKSE::GetTrampoline();
	_ArrowFunction = trmp.write_call<5>(arrowHook.address() + 0x90, ArrowFunction);

	logger::info("Arrow hook injected");
}

void Loki::NoFollowerAttackCollision::InstallInputSink() {
	auto deviceMan = RE::BSInputDeviceManager::GetSingleton();
	deviceMan->AddEventSink(OnInput::GetSingleton());
}

void Loki::NoFollowerAttackCollision::MeleeFunction(RE::Character* a_aggressor, RE::Actor* a_victim, std::int64_t a3, char a4, float a5) {

	if (!a_victim || !a_aggressor || !toggle) { return _MeleeFunction(a_aggressor, a_victim, a3, a4, a5); }

	if ((a_aggressor->IsPlayerRef() || a_aggressor->IsPlayerTeammate()) &&
		a_victim->IsPlayerTeammate() || (a_victim->IsGuard() && !a_victim->IsHostileToActor(a_aggressor))) {

		return;
	}
	if (a_victim->IsAMount() && !a_victim->IsHostileToActor(a_aggressor)) {
		return;
	}

	if (protectNeutralActor) { if (!a_victim->IsHostileToActor(a_aggressor)) { return; } }

	return _MeleeFunction(a_aggressor, a_victim, a3, a4, a5);

}

void Loki::NoFollowerAttackCollision::SweepFunction(RE::Character* a_aggressor, RE::Actor* a_victim, std::int64_t a3, char a4, float a5) {

	if (!a_victim || !a_aggressor || !toggle) { return _SweepFunction(a_aggressor, a_victim, a3, a4, a5); }

	if ((a_aggressor->IsPlayerRef() || a_aggressor->IsPlayerTeammate()) &&
		a_victim->IsPlayerTeammate() || (a_victim->IsGuard() && !a_victim->IsHostileToActor(a_aggressor))) {

		return;
	}
	if (a_victim->IsAMount() && !a_victim->IsHostileToActor(a_aggressor)) {
		return;
	}

	if (protectNeutralActor) { if (!a_victim->IsHostileToActor(a_aggressor)) { return; } }

	return _SweepFunction(a_aggressor, a_victim, a3, a4, a5);

}

void Loki::NoFollowerAttackCollision::ArrowFunction(RE::Character* a_aggressor, RE::Actor* a_victim, std::int64_t a3, char a4, float a5) {

	if (!a_victim || !a_aggressor || !toggle) { return _ArrowFunction(a_aggressor, a_victim, a3, a4, a5); }

	if ((a_aggressor->IsPlayerRef() || a_aggressor->IsPlayerTeammate()) &&
		a_victim->IsPlayerTeammate() || (a_victim->IsGuard() && !a_victim->IsHostileToActor(a_aggressor))) {

		return;
	}
	if (a_victim->IsAMount() && !a_victim->IsHostileToActor(a_aggressor)) {
		return;
	}

	if (protectNeutralActor) { if (!a_victim->IsHostileToActor(a_aggressor)) { return; } }

	return _ArrowFunction(a_aggressor, a_victim, a3, a4, a5);

}





RE::BSEventNotifyControl Loki::OnInput::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) {
	using EventType = RE::INPUT_EVENT_TYPE;
	using DeviceType = RE::INPUT_DEVICE;

	for (auto event = *a_event; event; event = event->next) {
		if (event->eventType != EventType::kButton) {
			continue;
		}

		auto button = skyrim_cast<RE::ButtonEvent*>(event);
		if (button->IsDown()) {
			auto key = button->idCode;
			switch (button->device.get()) {

				case DeviceType::kMouse: {
					key += NoFollowerAttackCollision::kMouseOffset;
					break;
				}

				case DeviceType::kKeyboard: {
					key += NoFollowerAttackCollision::kKeyboardOffset;
					break;
				}

				case DeviceType::kGamepad: {
					key = NoFollowerAttackCollision::GetGamepadIndex((RE::BSWin32GamepadDevice::Key)key);
					break;
				}

				default: continue;

			}

			auto ui = RE::UI::GetSingleton();
			auto controlmap = RE::ControlMap::GetSingleton();
			if (ui->GameIsPaused() || !controlmap->IsMovementControlsEnabled()) {
				continue;
			}

			//RE::ConsoleLog::GetSingleton()->Print("mod key -> %i", NoFollowerAttackCollision::toggleKey);
			//RE::ConsoleLog::GetSingleton()->Print("event key -> %i", key);
			if (key == NoFollowerAttackCollision::toggleKey) {
				if (!toggle) {
					toggle = true;
					RE::DebugNotification("NFAC: On");
				} else {
					toggle = false;
					RE::DebugNotification("NFAC: Off");
				}
			}
		}
	}

	return RE::BSEventNotifyControl::kContinue;
}