#pragma once

namespace Loki {

    class NoFollowerAttackCollision {

    public:
        NoFollowerAttackCollision();
        ~NoFollowerAttackCollision();
        static NoFollowerAttackCollision* GetSingleton();
        static void InstallMeleeHook();
        static void InstallSweepHook();

    private:
        // RCX = Aggressor, RDX = Victim, R8 = ???, R9 = ???, XMM0 = ???
        static void CharacterUnk_628C20(RE::Character* a_char, RE::Actor* a_actor, std::int64_t a3, char a4, float a5);
        static inline REL::Relocation<decltype(CharacterUnk_628C20)> _CharacterUnk_628C20;

    };

}