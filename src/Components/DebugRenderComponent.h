#ifndef DEBUGRENDERCOMPONENT_H
#define DEBUGRENDERCOMPONENT_H

struct DebugRenderComponent {
    bool isRenderBoxCollider;

    DebugRenderComponent(bool isRenderBoxCollider = true) {
        this->isRenderBoxCollider = isRenderBoxCollider;
    }
};

#endif