#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "abilities.h"
#include "assets.h"
#include "Enemy.h"
#include "Combatant.h"
#include <vector>
#include <string>
#include <memory>

enum class DiceType {
    ATTACK,
    DEFENSE
};

enum class TurnState {
    PLAYER_TURN,
    ENEMY_TURN,
    GAME_OVER,
    LEVEL_COMPLETE
};

class RolledDie {
public:
    DiceType type;
    int value;
    Rectangle source_rect;
};

class Game {
public:
    Game(int screenWidth, int screenHeight, const std::string& title);
    ~Game();
    void run();

private:
    void init_game();
    void update();
    void draw();
    void roll_dice();
    void apply_ability_effect(const Ability& ability);
    void draw_text_with_shadow(const std::string& text, float x, float y, float font_size, Color color, Font font);
    void load_enemy(int level);

    const int baseScreenWidth = 1920;
    const int baseScreenHeight = 1080;
    float scale;

    GameAssets assets;

    Combatant player;
    std::unique_ptr<Enemy> current_enemy;
    TurnState turn;
    std::vector<RolledDie> current_dice;
    bool needs_dice_roll;
    std::string feedback_message;
    float transition_timer;
    std::string game_over_message;
    int current_level;
};

#endif // GAME_H
