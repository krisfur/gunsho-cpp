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

void StartEnemy::draw(const GameAssets& assets, float scale, float x_pos, float y_pos, float sprite_width, float sprite_height) {
    Texture2D enemy_texture = (health <= max_health / 2) ? assets.goblin_hurt : assets.goblin;
    DrawTexturePro(enemy_texture, {0, 0, (float)enemy_texture.width, (float)enemy_texture.height}, {x_pos, y_pos, sprite_width, sprite_height}, {0, 0}, 0, WHITE);
}

float StartEnemy::get_sprite_scale() {
    return 0.55f; // size for goblin
}

BossEnemy::BossEnemy()
    : Enemy("Dragon", 100, 0, {}) {}

void BossEnemy::perform_turn_action(Combatant& player, std::string& feedback_message) {
    feedback_message = "Dragon breathes fire for 20 damage!";
    player.health = std::max(0, player.health - 20);
}

void BossEnemy::draw(const GameAssets& assets, float scale, float x_pos, float y_pos, float sprite_width, float sprite_height) {
    Texture2D enemy_texture = (health <= max_health / 2) ? assets.dragon_hurt : assets.dragon;
    DrawTexturePro(enemy_texture, {0, 0, (float)enemy_texture.width, (float)enemy_texture.height}, {x_pos, y_pos, sprite_width, sprite_height}, {0, 0}, 0, WHITE);
}

float BossEnemy::get_sprite_scale() {
    return 1.35f; // size for dragon
}
