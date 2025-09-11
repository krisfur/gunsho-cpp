#ifndef ENEMY_H
#define ENEMY_H

#include "Combatant.h" // For Combatant
#include "assets.h"

class Enemy : public Combatant {
public:
    Enemy(std::string name, int health, int dice_pool, const std::vector<Ability>& abilities);
    virtual ~Enemy() = default;

    // Virtual method to allow different enemy types to define their behavior
    virtual void perform_turn_action(Combatant& player, std::string& feedback_message) = 0;
    virtual void draw(const GameAssets& assets, float scale, float x_pos, float y_pos, float sprite_width, float sprite_height) = 0;
};

class StartEnemy : public Enemy {
public:
    StartEnemy();
    void perform_turn_action(Combatant& player, std::string& feedback_message) override;
    void draw(const GameAssets& assets, float scale, float x_pos, float y_pos, float sprite_width, float sprite_height) override;
};

class BossEnemy : public Enemy {
public:
    BossEnemy();
    void perform_turn_action(Combatant& player, std::string& feedback_message) override;
    void draw(const GameAssets& assets, float scale, float x_pos, float y_pos, float sprite_width, float sprite_height) override;
};

#endif // ENEMY_H
