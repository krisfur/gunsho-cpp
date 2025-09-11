#pragma once

#include <string>
#include <vector>

enum class AbilityEffectType {
    DAMAGE
};

struct AbilityEffect {
    AbilityEffectType type;
    int base_amount;
    int base_healing;
};

struct Ability {
    std::string name;
    std::string description;
    int cost_red;
    int cost_blue;
    AbilityEffect effect;
};

std::vector<Ability> get_default_player_abilities();
std::vector<Ability> get_enemy_abilities();
