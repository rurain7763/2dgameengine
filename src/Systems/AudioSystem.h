#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../Logger/Logger.h"
#include "../Components/AudioComponent.h"

#include <SDL2/SDL_mixer.h>

class AudioSystem : public System {
public:
    AudioSystem() {
        RequireComponent<AudioComponent>();
    }

    void Update(std::unique_ptr<AssetManager>& assetMg) {
        for(auto entity : GetEntities()) {
            auto& audioComponent = entity.GetComponent<AudioComponent>();

            Mix_Chunk* audio = assetMg->GetAudio(audioComponent.assetID);
            if(!audio) {
                LOG_ERR("Error getting music asset with id = %s", audioComponent.assetID.c_str());
                continue;
            }

            if(audioComponent.isOneshot) {
                if(audioComponent.channelID == -1) {
                    audioComponent.channelID = Mix_PlayChannel(-1, audio, 0);
                }
            } else {
                if(audioComponent.channelID == -1) {
                    audioComponent.channelID = Mix_PlayChannel(-1, audio, -1);
                }
            }

            if(audioComponent.channelID != -1) {
                Mix_Volume(audioComponent.channelID, audioComponent.volume);
            }
        }
    }
};

#endif