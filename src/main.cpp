#include "C:/dev/ExamplePlugin-CommonLibSSE/build/simpleini-master/SimpleIni.h"

const SKSE::MessagingInterface* g_messaging2 = nullptr;

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface * a_skse, SKSE::PluginInfo * a_info)
{
#ifndef NDEBUG
    auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
    auto path = logger::log_directory();
    if (!path) {
        return false;
    }

    *path /= "loki_NoFollowerAttackCollision.log"sv;
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

    auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
    log->set_level(spdlog::level::trace);
#else
    log->set_level(spdlog::level::info);
    log->flush_on(spdlog::level::info);
#endif

    spdlog::set_default_logger(std::move(log));
    spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

    logger::info("loki_NoFollowerAttackCollision v1.0.0");

    a_info->infoVersion = SKSE::PluginInfo::kVersion;
    a_info->name = "loki_NoFollowerAttackCollision";
    a_info->version = 1;

    if (a_skse->IsEditor()) {
        logger::critical("Loaded in editor, marking as incompatible"sv);
        return false;
    }

    const auto ver = a_skse->RuntimeVersion();
    if (ver < SKSE::RUNTIME_1_5_39) {
        logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
        return false;
    }

    return true;
}

static void SKSEMessageHandler(SKSE::MessagingInterface::Message* message) {

    switch (message->type) {
    case SKSE::MessagingInterface::kNewGame:
    case SKSE::MessagingInterface::kPostLoadGame: {
        break;
    }
    case SKSE::MessagingInterface::kPostLoad: {
        break;
    }
    default:
        break;
    }

}

class Loki_NoFollowerAttackCollision {

public:
    Loki_NoFollowerAttackCollision() {
    }
    ~Loki_NoFollowerAttackCollision() {
    }
    static Loki_NoFollowerAttackCollision* GetSingleton() {
        static Loki_NoFollowerAttackCollision* singleton = new Loki_NoFollowerAttackCollision();
        return singleton;
    }
    static void InstallUnkHook() {

        REL::Relocation<std::uintptr_t> UnkHook{ REL::ID(37650) }; //+38B

        auto& trampoline = SKSE::GetTrampoline();
        _CharacterUnk_628C20 = trampoline.write_call<5>(UnkHook.address() + 0x38B, CharacterUnk_628C20);

        logger::info("Unk Hook injected");

    }
    static void InstallDoCombatSpellApplyHook() {
        //char __fastcall Actor::DoCombatSpellApply_1406282E0(Actor *a1, SpellItem *a2, TESObjectREFR *target)
        REL::Relocation<std::uintptr_t> CombatSpellApply { 0x140 };

        auto& trampoline = SKSE::GetTrampoline();
        _DoCombatSpellApply = trampoline.write_call<5>(CombatSpellApply.address(), DoCombatSpellApply);

        logger::info("DoCombatSpellApply hook injected");
    }

private:
    // RCX = Aggressor, RDX = Victim, R8 = ???, R9 = ???, XMM0 = ???
    static void CharacterUnk_628C20(RE::Character* a_char, RE::Actor* a_actor, std::int64_t a3, char a4, float a5) {
        if (!a_actor || !a_char) { return _CharacterUnk_628C20(a_char, a_actor, a3, a4, a5); }

        if ((a_char->IsPlayerRef() || a_char->IsPlayerTeammate()) && a_actor->IsPlayerTeammate()) { return; }

        return _CharacterUnk_628C20(a_char, a_actor, a3, a4, a5);
    }
    static bool DoCombatSpellApply(RE::Actor* a_actor, RE::SpellItem* a_spell, RE::TESObjectREFR* a_target) {
        if (!a_actor || !a_spell || !a_target) { return _DoCombatSpellApply(a_actor, a_spell, a_target); };

        RE::Actor* a_victim = skyrim_cast<RE::Actor*>(a_target);
        if ((a_actor->IsPlayerRef() || a_actor->IsPlayerTeammate()) && a_victim->IsPlayerTeammate()) { return false; };

        return _DoCombatSpellApply(a_actor, a_spell, a_target);
    }
    static inline REL::Relocation<decltype(DoCombatSpellApply)> _DoCombatSpellApply;
    static inline REL::Relocation<decltype(CharacterUnk_628C20)> _CharacterUnk_628C20;

};

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface * a_skse)
{
    logger::info("NoFollowerAttackCollision loaded");
    SKSE::Init(a_skse);
    SKSE::AllocTrampoline(64);

    Loki_NoFollowerAttackCollision::InstallUnkHook();

    return true;
}