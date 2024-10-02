#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <SDL2/SDL.h>

struct AnimationComponent {
    int numFrames;
    int frameSpeedRate;
    bool isLoop;
    int currentFrame;
    int startTime;

    AnimationComponent(int numFrames = 0, int frameSpeedRate = 1, bool isLoop = false) {
        this->numFrames = numFrames;
        this->frameSpeedRate = frameSpeedRate;
        this->isLoop = isLoop;

        currentFrame = 0;
        startTime = SDL_GetTicks();
    }
};

#endif
