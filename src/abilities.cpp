#include "abilities.h"

static const Ability STRONG_ATTACK = {
    "Strong Attack",
    "DMG: 10 + R",
    2,
    0,
    {AbilityEffectType::DAMAGE, 10, 0}
};

static const Ability MIXED_ATTACK = {
    "Mixed Attack",
    "DMG: 5 + R + B; HEAL: 5 * B",
    1,
    1,
    {AbilityEffectType::DAMAGE, 5, 5}
};

static const Ability BLUE_HEAL = {
    "Blue Heal",
    "HEAL: 10 * B",
    0,
    2,
    {AbilityEffectType::DAMAGE, 0, 10}
};

std::vector<Ability> get_default_player_abilities() {
    return {STRONG_ATTACK, MIXED_ATTACK, BLUE_HEAL};
}

std::vector<Ability> get_enemy_abilities() {
    return {STRONG_ATTACK};
}
