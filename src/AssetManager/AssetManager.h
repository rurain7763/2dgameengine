#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <SDL2/SDL.h>
#include <map>

class AssetManager {
public:
    AssetManager();
    ~AssetManager();

    void AddTexture(SDL_Renderer* renderer, const std::string& assetID, const std::string& filePath);
    SDL_Texture* GetTexture(const std::string& assetID) const;
    void ClearAssets();

private:
    std::map<std::string, SDL_Texture*> _textures;
};

#endif