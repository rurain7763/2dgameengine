#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <vector>

class AssetManager {
public:
    AssetManager();
    ~AssetManager();

    void AddTexture(SDL_Renderer* renderer, const std::string& assetID, const std::string& filePath);
    SDL_Texture* GetTexture(const std::string& assetID) const;
    std::vector<const char*> GetTextureAssetIDs() const;

    void AddFont(const std::string& assetID, const std::string& filePath, int fontSize);
    TTF_Font* GetFont(const std::string& assetID) const;

    void ClearAssets();

private:
    std::map<std::string, SDL_Texture*> _textures;
    std::map<std::string, TTF_Font*> _fonts;
};

#endif