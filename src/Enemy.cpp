#include "Enemy.h"
#include "Combatant.h"
#include <algorithm>

Enemy::Enemy(std::string name, int health, int dice_pool, const std::vector<Ability>& abilities)
    : Combatant(name, health, dice_pool, abilities) {}

StartEnemy::StartEnemy()
    : Enemy("Goblin", 50, 0, {}) {}

void StartEnemy::perform_turn_action(Combatant& player, std::string& feedback_message) {
    feedback_message = "Goblin attacks for 15 damage!";
    player.health = std::max(0, player.health - 15);
}

BossEnemy::BossEnemy()
    : Enemy("Dragon", 100, 0, {}) {}

void BossEnemy::perform_turn_action(Combatant& player, std::string& feedback_message) {
    feedback_message = "Dragon breathes fire for 20 damage!";
    player.health = std::max(0, player.health - 20);
}
