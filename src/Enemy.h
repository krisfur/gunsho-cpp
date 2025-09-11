#ifndef ENEMY_H
#define ENEMY_H

#include "Combatant.h" // For Combatant

class Enemy : public Combatant {
public:
    Enemy(std::string name, int health, int dice_pool, const std::vector<Ability>& abilities);
    virtual ~Enemy() = default;

    // Virtual method to allow different enemy types to define their behavior
    virtual void perform_turn_action(Combatant& player, std::string& feedback_message) = 0;
};

class StartEnemy : public Enemy {
public:
    StartEnemy();
    void perform_turn_action(Combatant& player, std::string& feedback_message) override;
};

class BossEnemy : public Enemy {
public:
    BossEnemy();
    void perform_turn_action(Combatant& player, std::string& feedback_message) override;
};

#endif // ENEMY_H
