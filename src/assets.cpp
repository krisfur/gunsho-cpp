#include "assets.h"

GameAssets LoadGameAssets() {
    GameAssets assets;
    assets.background = LoadTexture("assets/background.png");
    assets.player = LoadTexture("assets/player.png");
    assets.player_hurt = LoadTexture("assets/player-hurt.png");
    assets.dice_tileset = LoadTexture("assets/dice.png");
    assets.font = LoadFont("assets/fonts/Vanilla Whale.otf");
    assets.goblin = LoadTexture("assets/goblin.png");
    assets.goblin_hurt = LoadTexture("assets/goblin-hurt.png");
    assets.dragon = LoadTexture("assets/dragon.png");
    assets.dragon_hurt = LoadTexture("assets/dragon-hurt.png");
    return assets;
}

void UnloadGameAssets(const GameAssets& assets) {
    UnloadTexture(assets.background);
    UnloadTexture(assets.player);
    UnloadTexture(assets.player_hurt);
    UnloadTexture(assets.dice_tileset);
    UnloadFont(assets.font);
    UnloadTexture(assets.goblin);
    UnloadTexture(assets.goblin_hurt);
    UnloadTexture(assets.dragon);
    UnloadTexture(assets.dragon_hurt);
}
