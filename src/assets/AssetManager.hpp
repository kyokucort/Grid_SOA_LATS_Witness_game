#pragma once
#include "raylib.h"
#include "TextureID.hpp"

struct AssetManager
{
    Texture2D textures[(int)TextureID::COUNT];
};

void AssetsLoad(AssetManager& a);
void AssetsUnload(AssetManager& a);
Texture2D& AssetsGet(AssetManager& a, TextureID id);
