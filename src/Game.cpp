#include "Game.h"
#include "Combatant.h"
#include <numeric>
#include <algorithm>
#include <memory>

Game::Game(int screenWidth, int screenHeight, const std::string& title)
    : scale(1.0f), turn(TurnState::PLAYER_TURN), needs_dice_roll(true), transition_timer(0.0f), current_level(1) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, title.c_str());
    SetTargetFPS(120);
    assets = LoadGameAssets();
    SetWindowIcon(assets.icon);
    init_game();
}

Game::~Game() {
    UnloadGameAssets(assets);
    CloseWindow();
}

void Game::init_game() {
    player = Combatant("GunSho", 100, 4, get_default_player_abilities());
    load_enemy(current_level);
    turn = TurnState::PLAYER_TURN;
    needs_dice_roll = true;
    feedback_message = "Your turn! Press 1-3, or Space to skip.";
    transition_timer = 0.0f;
}

void Game::load_enemy(int level) {
    if (level == 1) {
        current_enemy = std::make_unique<StartEnemy>();
    } else if (level == 2) {
        current_enemy = std::make_unique<BossEnemy>();
    }
}

void Game::run() {
    while (!WindowShouldClose()) {
        update();
        draw();
    }
}

void Game::update() {
    if (transition_timer > 0.0f) {
        transition_timer -= GetFrameTime();
        return;
    }

    switch (turn) {
        case TurnState::PLAYER_TURN: {
            if (needs_dice_roll) {
                roll_dice();
            }

            int ability_index_to_use = -1;
            if (IsKeyPressed(KEY_ONE)) ability_index_to_use = 0;
            if (IsKeyPressed(KEY_TWO)) ability_index_to_use = 1;
            if (IsKeyPressed(KEY_THREE)) ability_index_to_use = 2;

            if (IsKeyPressed(KEY_SPACE)) {
                feedback_message = "Player skipped turn.";
                current_dice.clear();
                turn = TurnState::ENEMY_TURN;
                transition_timer = 1.5f;
                return;
            }

            if (ability_index_to_use != -1 && ability_index_to_use < player.abilities.size()) {
                const auto& ability = player.abilities[ability_index_to_use];
                int red_dice = 0;
                int blue_dice = 0;
                for (const auto& die : current_dice) {
                    if (die.type == DiceType::ATTACK) red_dice++;
                    if (die.type == DiceType::DEFENSE) blue_dice++;
                }

                if (red_dice >= ability.cost_red && blue_dice >= ability.cost_blue) {
                    apply_ability_effect(ability);
                    current_dice.clear();
                    if (current_enemy->health <= 0) {
                        if (current_level == 1) {
                            turn = TurnState::LEVEL_COMPLETE;
                            game_over_message = "Level Complete! Press Enter for Boss.";
                        } else {
                            turn = TurnState::GAME_OVER;
                            game_over_message = "You Win!";
                        }
                    } else {
                        turn = TurnState::ENEMY_TURN;
                        transition_timer = 1.5f;
                    }
                } else {
                    feedback_message = "Cannot afford '" + ability.name + "'.";
                }
            }
            break;
        }
        case TurnState::ENEMY_TURN: {
            current_enemy->perform_turn_action(player, feedback_message);

            if (player.health <= 0) {
                turn = TurnState::GAME_OVER;
                game_over_message = "You Lose!";
            }
            else {
                turn = TurnState::PLAYER_TURN;
                needs_dice_roll = true;
                transition_timer = 0.2f;
            }
            break;
        }
        case TurnState::LEVEL_COMPLETE: {
            if (IsKeyPressed(KEY_ENTER)) {
                current_level = 2;
                init_game();
            }
            break;
        }
        case TurnState::GAME_OVER: {
            if (IsKeyPressed(KEY_ENTER)) {
                current_level = 1;
                init_game();
            }
            break;
        }
    }
}

void Game::draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    scale = std::min((float)GetScreenWidth() / baseScreenWidth, (float)GetScreenHeight() / baseScreenHeight);

    DrawTexturePro(assets.background, {0, 0, (float)assets.background.width, (float)assets.background.height}, {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0}, 0, WHITE);

    // Player and Enemy Info
    draw_text_with_shadow(player.name, 20 * scale, 20 * scale, 45.0f * scale, WHITE, assets.font);
    std::string player_hp_text = "HP: " + std::to_string(player.health) + "/" + std::to_string(player.max_health);
    draw_text_with_shadow(player_hp_text, 20 * scale, 50 * scale, 36.0f * scale, WHITE, assets.font);

    Texture2D player_texture = (player.health <= player.max_health / 2) ? assets.player_hurt : assets.player;
    float sprite_height = (GetScreenHeight() / 2.5f);
    float sprite_width = sprite_height;
    float y_pos = GetScreenHeight() / 2.0f - sprite_height / 2.0f;
    DrawTexturePro(player_texture, {0, 0, (float)player_texture.width, (float)player_texture.height}, {40 * scale, y_pos, sprite_width, sprite_height}, {0, 0}, 0, WHITE);

    // Enemy Sprite
    Texture2D enemy_texture;
    float enemy_sprite_scale = 1.0f; // Default scale
    if (dynamic_cast<StartEnemy*>(current_enemy.get())) {
        enemy_texture = (current_enemy->health <= current_enemy->max_health / 2) ? assets.goblin_hurt : assets.goblin;
        enemy_sprite_scale = 0.5f; // Half size for goblin
    } else if (dynamic_cast<BossEnemy*>(current_enemy.get())) {
        enemy_texture = (current_enemy->health <= current_enemy->max_health / 2) ? assets.dragon_hurt : assets.dragon;
        enemy_sprite_scale = 1.3f; // 30% larger for dragon
    } else {
        // Default or error texture
        enemy_texture = assets.player; // Fallback, should not happen
    }

    float enemy_sprite_height = (GetScreenHeight() / 2.5f) * enemy_sprite_scale;
    float enemy_sprite_width = enemy_sprite_height; // Assuming square sprites
    float enemy_y_pos = GetScreenHeight() / 2.0f - enemy_sprite_height / 2.0f;
    float enemy_x_pos = GetScreenWidth() - enemy_sprite_width - (40 * scale); // Position on the right
    DrawTexturePro(enemy_texture, {0, 0, (float)enemy_texture.width, (float)enemy_texture.height}, {enemy_x_pos, enemy_y_pos, enemy_sprite_width, enemy_sprite_height}, {0, 0}, 0, WHITE);

    Vector2 enemy_name_size = MeasureTextEx(assets.font, current_enemy->name.c_str(), 45.0f * scale, 2);
    float enemy_name_x = GetScreenWidth() - enemy_name_size.x - (20 * scale);
    draw_text_with_shadow(current_enemy->name, enemy_name_x, 20 * scale, 45.0f * scale, WHITE, assets.font);
    std::string enemy_hp_text = "HP: " + std::to_string(current_enemy->health) + "/" + std::to_string(current_enemy->max_health);
    Vector2 enemy_hp_size = MeasureTextEx(assets.font, enemy_hp_text.c_str(), 36.0f * scale, 2);
    float enemy_hp_x = GetScreenWidth() - enemy_hp_size.x - (20 * scale);
    draw_text_with_shadow(enemy_hp_text, enemy_hp_x, 50 * scale, 36.0f * scale, WHITE, assets.font);

    switch (turn) {
        case TurnState::PLAYER_TURN: {
            float dice_size = 128.0f * scale;
            float dice_gap = 10.0f * scale;
            float num_dice = (float)current_dice.size();
            float total_dice_width = (num_dice * dice_size) + ((num_dice - 1.0f > 0 ? num_dice - 1.0f : 0) * dice_gap);
            float start_x = (GetScreenWidth() - total_dice_width) / 2.0f;

            for (size_t i = 0; i < current_dice.size(); ++i) {
                DrawTexturePro(assets.dice_tileset, current_dice[i].source_rect, {start_x + i * (dice_size + dice_gap), GetScreenHeight() / 2.0f - (dice_size / 2.0f), dice_size, dice_size}, {0,0}, 0, WHITE);
            }

            float base_y = GetScreenHeight() - (120.0f * scale);
            float line_height = 28.0f * scale;
            draw_text_with_shadow("Abilities (Space to Skip):", 20 * scale, base_y, 36.0f * scale, WHITE, assets.font);

            for (size_t i = 0; i < player.abilities.size(); ++i) {
                const auto& ability = player.abilities[i];
                std::string ability_line = std::to_string(i + 1) + ". " + ability.name + " (R: " + std::to_string(ability.cost_red) + ", B: " + std::to_string(ability.cost_blue) + ") - " + ability.description;
                draw_text_with_shadow(ability_line, 20 * scale, base_y + (i + 1) * line_height, 36.0f * scale, WHITE, assets.font);
            }
            break;
        }
        case TurnState::ENEMY_TURN: {
            const char* msg = "Enemy is thinking...";
            Vector2 text_size = MeasureTextEx(assets.font, msg, 45.0f * scale, 2);
            draw_text_with_shadow(msg, (GetScreenWidth() - text_size.x) / 2.0f, GetScreenHeight() / 2.0f, 45.0f * scale, WHITE, assets.font);
            break;
        }
        case TurnState::LEVEL_COMPLETE: {
            Vector2 msg_size = MeasureTextEx(assets.font, game_over_message.c_str(), 75.0f * scale, 2);
            draw_text_with_shadow(game_over_message, (GetScreenWidth() - msg_size.x) / 2.0f, GetScreenHeight() / 2.0f - 20 * scale, 75.0f * scale, YELLOW, assets.font);
            break;
        }
        case TurnState::GAME_OVER: {
            const char* restart_msg = "Press Enter to Restart";
            Vector2 msg_size = MeasureTextEx(assets.font, game_over_message.c_str(), 75.0f * scale, 2);
            Vector2 restart_size = MeasureTextEx(assets.font, restart_msg, 45.0f * scale, 2);
            draw_text_with_shadow(game_over_message, (GetScreenWidth() - msg_size.x) / 2.0f, GetScreenHeight() / 2.0f - 20 * scale, 75.0f * scale, YELLOW, assets.font);
            draw_text_with_shadow(restart_msg, (GetScreenWidth() - restart_size.x) / 2.0f, GetScreenHeight() / 2.0f + 30 * scale, 45.0f * scale, WHITE, assets.font);
            break;
        }
    }

    Vector2 text_size = MeasureTextEx(assets.font, feedback_message.c_str(), 30.0f * scale, 2);
    draw_text_with_shadow(feedback_message, (GetScreenWidth() - text_size.x) / 2.0f, GetScreenHeight() - 40.0f * scale, 30.0f * scale, YELLOW, assets.font);

    EndDrawing();
}

void Game::roll_dice() {
    current_dice.clear();
    float tile_width = assets.dice_tileset.width / 3.0f;
    float tile_height = assets.dice_tileset.height / 2.0f;

    for (int i = 0; i < player.dice_pool_count; ++i) {
        int value = GetRandomValue(1, 3);
        DiceType type = (GetRandomValue(0, 1) == 0) ? DiceType::ATTACK : DiceType::DEFENSE;
        Rectangle source_rect;
        if (type == DiceType::ATTACK) {
            source_rect = {(float)(value - 1) * tile_width, 0, tile_width, tile_height};
        } else {
            source_rect = {(float)(value - 1) * tile_width, tile_height, tile_width, tile_height};
        }
        RolledDie die;
        die.type = type;
        die.value = value;
        die.source_rect = source_rect;
        current_dice.push_back(die);
    }
    needs_dice_roll = false;
}

void Game::apply_ability_effect(const Ability& ability) {
    if (ability.effect.type == AbilityEffectType::DAMAGE) {
        std::vector<int> red_dice_values;
        std::vector<int> blue_dice_values;
        for (const auto& die : current_dice) {
            if (die.type == DiceType::ATTACK) {
                red_dice_values.push_back(die.value);
            }
            if (die.type == DiceType::DEFENSE) {
                blue_dice_values.push_back(die.value);
            }
        }
        std::sort(red_dice_values.rbegin(), red_dice_values.rend());
        std::sort(blue_dice_values.rbegin(), blue_dice_values.rend());

        int total_damage = ability.effect.base_amount;
        if (ability.cost_red > 0) {
            total_damage += std::accumulate(red_dice_values.begin(), red_dice_values.begin() + std::min((int)red_dice_values.size(), ability.cost_red), 0);
        }
        if (ability.cost_blue > 0 && ability.effect.base_amount > 0) {
            total_damage += std::accumulate(blue_dice_values.begin(), blue_dice_values.begin() + std::min((int)blue_dice_values.size(), ability.cost_blue), 0);
        }

        int total_healing = 0;
        if (ability.effect.base_healing > 0 && ability.cost_blue > 0) {
            int blue_value = std::accumulate(blue_dice_values.begin(), blue_dice_values.begin() + std::min((int)blue_dice_values.size(), ability.cost_blue), 0);
            total_healing = ability.effect.base_healing * blue_value;
        }

        current_enemy->health = std::max(0, current_enemy->health - total_damage);
        player.health = std::min(player.max_health, player.health + total_healing);

        feedback_message = "Used '" + ability.name + "' for " + std::to_string(total_damage) + " damage and " + std::to_string(total_healing) + " healing!";
    }
}

void Game::draw_text_with_shadow(const std::string& text, float x, float y, float font_size, Color color, Font font) {
    DrawTextEx(font, text.c_str(), {x + 2 * scale, y + 2 * scale}, font_size, 2, DARKGRAY);
    DrawTextEx(font, text.c_str(), {x, y}, font_size, 2, color);
}
