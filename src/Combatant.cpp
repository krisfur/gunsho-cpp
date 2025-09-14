#include "Combatant.h"

#include <utility>

Combatant::Combatant(std::string name, const int health, const int dice_pool, const std::vector<Ability>& abilities)
    : name(std::move(name)), health(health), max_health(health), dice_pool_count(dice_pool), abilities(abilities) {}
