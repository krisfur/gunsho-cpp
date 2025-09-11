#ifndef COMBATANT_H
#define COMBATANT_H

#include <string>
#include <vector>
#include "abilities.h"

class Combatant {
public:
    std::string name;
    int health;
    int max_health;
    int dice_pool_count;
    std::vector<Ability> abilities;

    Combatant() : health(0), max_health(0), dice_pool_count(0) {}
    Combatant(std::string name, int health, int dice_pool, const std::vector<Ability>& abilities);
};

#endif // COMBATANT_H
