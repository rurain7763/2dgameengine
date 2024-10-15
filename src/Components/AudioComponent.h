#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include <string>
#include <SDL2/SDL_mixer.h>

struct AudioComponent {
    std::string assetID;
    bool isOneshot;
    int volume;
    int channelID;

    AudioComponent(const std::string& assetID = "", bool isOneshot = false, int volume = MIX_MAX_VOLUME) {
        this->assetID = assetID;
        this->isOneshot = isOneshot;
        this->volume = volume;
        this->channelID = -1;
    }
};

#endif