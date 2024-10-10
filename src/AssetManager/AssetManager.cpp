#include "AssetManager.h"
#include "../Logger/Logger.h"
#include "../Logger/Logger.h"

#include <SDL2/SDL_image.h>

AssetManager::AssetManager() {
    LOG("Asset manager was created!");
}

AssetManager::~AssetManager() {
    ClearAssets();
    LOG("Asset manager was destroyed");
}

void AssetManager::AddTexture(SDL_Renderer* renderer, const std::string& assetID, const std::string& filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    _textures.insert({assetID, texture});

    LOG("New texture added to the asset manager with id = %s", assetID.c_str());
}

SDL_Texture* AssetManager::GetTexture(const std::string& assetID) const {
    return _textures.at(assetID);
}

void AssetManager::AddFont(const std::string& assetID, const std::string& filePath, int fontSize) {
    TTF_Font* font = TTF_OpenFont(filePath.c_str(), fontSize);
    _fonts.emplace(assetID, font);
}

TTF_Font* AssetManager::GetFont(const std::string& assetID) const {
    return _fonts.at(assetID);
}

void AssetManager::ClearAssets() {
    for(auto pair : _textures) {
        SDL_DestroyTexture(pair.second);
    }
    _textures.clear();

    for(auto pair : _fonts) {
        TTF_CloseFont(pair.second);
    }
    _fonts.clear();
}