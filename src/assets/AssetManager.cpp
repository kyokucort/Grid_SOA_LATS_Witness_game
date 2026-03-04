#include "AssetManager.hpp"


void AssetsLoad(AssetManager& a)
{
    a.textures[(int)TextureID::Player]  = LoadTexture("resources/textures/mage_idle.png");
    a.textures[(int)TextureID::Wall]  = LoadTexture("resources/textures/wall.png");
    //a.textures[(int)TextureID::Enemy]   = LoadTexture("assets/textures/enemy.png");
    //a.textures[(int)TextureID::Tileset] = LoadTexture("assets/textures/tileset.png");
}

void AssetsUnload(AssetManager& a)
{
    for(int i=0;i<(int)TextureID::COUNT;i++)
        UnloadTexture(a.textures[i]);
}

Texture2D& AssetsGet(AssetManager& a, TextureID id)
{
    return a.textures[(int)id];
}
