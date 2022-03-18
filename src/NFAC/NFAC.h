#pragma once
#include "C:/dev/simpleini-master/SimpleIni.h"

namespace Loki {

    class OnInput :
        public RE::BSTEventSink<RE::InputEvent*> {

    public:
        static OnInput* GetSingleton() {
            static OnInput singleton;
            return &singleton;
        }
        virtual RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;

    protected:
        OnInput() = default;
        OnInput(const OnInput&) = delete;
        OnInput(OnInput&&) = delete;
        virtual ~OnInput() = default;

        auto operator=(const OnInput&)->OnInput & = delete;
        auto operator=(OnInput&&)->OnInput & = delete;

    };

    class NoFollowerAttackCollision {

    public:
        enum Offset : std::uint32_t {
            kInvalid = static_cast<std::uint32_t>(-1),
            kKeyboardOffset = 0,
            kMouseOffset = 256,
            kGamepadOffset = 266
        };
		static std::uint32_t GetGamepadIndex(RE::BSWin32GamepadDevice::Key a_key)
		{
			using Key = RE::BSWin32GamepadDevice::Key;

			std::uint32_t index;
			switch (a_key)
			{
				case Key::kUp:
					index = 0;
					break;
				case Key::kDown:
					index = 1;
					break;
				case Key::kLeft:
					index = 2;
					break;
				case Key::kRight:
					index = 3;
					break;
				case Key::kStart:
					index = 4;
					break;
				case Key::kBack:
					index = 5;
					break;
				case Key::kLeftThumb:
					index = 6;
					break;
				case Key::kRightThumb:
					index = 7;
					break;
				case Key::kLeftShoulder:
					index = 8;
					break;
				case Key::kRightShoulder:
					index = 9;
					break;
				case Key::kA:
					index = 10;
					break;
				case Key::kB:
					index = 11;
					break;
				case Key::kX:
					index = 12;
					break;
				case Key::kY:
					index = 13;
					break;
				case Key::kLeftTrigger:
					index = 14;
					break;
				case Key::kRightTrigger:
					index = 15;
					break;
				default:
					index = kInvalid;
					break;
			}

			return index != kInvalid ? index + kGamepadOffset : kInvalid;
		}

        NoFollowerAttackCollision() {
			CSimpleIniA ini;
			ini.SetUnicode();
			auto filename = L"Data/SKSE/Plugins/loki_NFAC.ini";
			SI_Error rc = ini.LoadFile(filename);

			toggleKey = ini.GetLongValue("SETTINGS", "key", -1);
			protectNeutralActor = ini.GetBoolValue("SETTINGS", "bProtectNeutralActor", false);
		}
        ~NoFollowerAttackCollision() {

		}
        static NoFollowerAttackCollision* GetSingleton() {
			static NoFollowerAttackCollision* singleton = new NoFollowerAttackCollision();
			return singleton;
		}
        static void InstallMeleeHook();
        static void InstallSweepHook();
        static void InstallArrowHook();

		static void InstallInputSink();

        static inline std::uint32_t toggleKey;
		static inline bool protectNeutralActor;

    private:
        // RCX = Aggressor, RDX = Victim, R8 = ???, R9 = ???, XMM0 = ???
		static void MeleeFunction(RE::Character* a_char, RE::Actor* a_actor, std::int64_t a3, char a4, float a5);
		static void SweepFunction(RE::Character* a_char, RE::Actor* a_actor, std::int64_t a3, char a4, float a5);
		static void ArrowFunction(RE::Character* a_char, RE::Actor* a_actor, std::int64_t a3, char a4, float a5);

		static inline REL::Relocation<decltype(MeleeFunction)> _MeleeFunction;
		static inline REL::Relocation<decltype(SweepFunction)> _SweepFunction;
		static inline REL::Relocation<decltype(ArrowFunction)> _ArrowFunction;

    };

}