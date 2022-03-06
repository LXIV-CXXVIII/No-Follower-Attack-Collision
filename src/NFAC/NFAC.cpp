#include "NFAC.h"

Loki::NoFollowerAttackCollision::NoFollowerAttackCollision() {

}

Loki::NoFollowerAttackCollision::~NoFollowerAttackCollision() {

}

Loki::NoFollowerAttackCollision* Loki::NoFollowerAttackCollision::GetSingleton() {
    static NoFollowerAttackCollision* singleton = new NoFollowerAttackCollision();
    return singleton;
}

void Loki::NoFollowerAttackCollision::InstallMeleeHook() {
    REL::Relocation<std::uintptr_t> UnkHook{ REL::ID(37650) }; //+38B

    auto& trampoline = SKSE::GetTrampoline();
    _CharacterUnk_628C20 = trampoline.write_call<5>(UnkHook.address() + 0x38B, CharacterUnk_628C20);

    logger::info("Unk Hook injected");
}

void Loki::NoFollowerAttackCollision::InstallSweepHook() {
    REL::Relocation<std::uintptr_t> UnkHook{ REL::ID(37689) }; //+DD

    auto& trampoline = SKSE::GetTrampoline();
    _CharacterUnk_628C20 = trampoline.write_call<5>(UnkHook.address() + 0xDD, CharacterUnk_628C20);

    logger::info("Unk Hook 2 injected");
}

void Loki::NoFollowerAttackCollision::CharacterUnk_628C20(RE::Character* a_char, RE::Actor* a_actor, std::int64_t a3, char a4, float a5) {
    if (!a_actor || !a_char) { return _CharacterUnk_628C20(a_char, a_actor, a3, a4, a5); }

    if ((a_char->IsPlayerRef() || a_char->IsPlayerTeammate()) && (a_actor->IsPlayerTeammate() || a_actor->IsGuard())) { return; }

    return _CharacterUnk_628C20(a_char, a_actor, a3, a4, a5);
}