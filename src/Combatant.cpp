#include "Combatant.h"

Combatant::Combatant(std::string name, int health, int dice_pool, const std::vector<Ability>& abilities)
    : name(name), health(health), max_health(health), dice_pool_count(dice_pool), abilities(abilities) {}
