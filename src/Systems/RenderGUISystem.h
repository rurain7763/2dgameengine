#ifndef RENDERGUISYSTEM_H
#define RENDERGUISYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/DebugRenderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/HealthUIComponent.h"

#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/imgui_sdl.h"
#include "../../libs/imgui/imgui_impl_sdl.h"

class RenderGUISystem : public System {
public:
    RenderGUISystem() = default;

    void Update(std::unique_ptr<Registry>& registry) {
        ImGui::NewFrame();

        ImGuiWindowFlags windowFlags = 0;
        if(ImGui::Begin("Spawn ememies", NULL, windowFlags)) {
            static int positions[] = {0, 0};
            ImGui::InputInt2("enemy position", positions);
            if(ImGui::Button("Spawn")) {
                Entity enemy = registry->CreateEntity();
                enemy.Group("enemies");
                enemy.AddComponent<TransformComponent>(glm::vec2(positions[0], positions[1]), glm::vec2(1, 1), 0);
                enemy.AddComponent<RigidBodyComponent>(glm::vec2(0, 0));
                enemy.AddComponent<SpriteComponent>("tank_image", 32, 32, 2);
                enemy.AddComponent<BoxColliderComponent>(32, 32);
                enemy.AddComponent<DebugRenderComponent>(true);
                enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(100, 0), 2000, 2000);
                enemy.AddComponent<HealthComponent>();
                enemy.AddComponent<HealthUIComponent>(glm::vec2(32, 0));
            }
        }
        ImGui::End();

        ImGui::Render();
        ImGuiSDL::Render(ImGui::GetDrawData());
    }
};

#endif