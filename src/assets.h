#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"

struct GameAssets {
    Texture2D background;
    Texture2D player;
    Texture2D player_hurt;
    Texture2D dice_tileset;
    Font font;
};

GameAssets LoadGameAssets();
void UnloadGameAssets(const GameAssets& assets);

#endif // ASSETS_H
