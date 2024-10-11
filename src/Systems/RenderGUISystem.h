#ifndef RENDERGUISYSTEM_H
#define RENDERGUISYSTEM_H

#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
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

#define RAD2DEG(x) (180 / M_PI * x)

class RenderGUISystem : public System {
public:
    RenderGUISystem() = default;

    void Update(std::unique_ptr<Registry>& registry, std::unique_ptr<AssetManager>& assetMg, const SDL_Rect& camera) {
        ImGui::NewFrame();

        ImGuiWindowFlags windowFlags = 0;
        if(ImGui::Begin("Spawn ememies", NULL, windowFlags)) {
            static int positions[] = {0, 0};
            static float size[] = {1, 1};
            static float rotation = 0;
            static int selectedItem = 0;
            static float projectileSpeed = 100;
            static float projectileRepeatFrequency = 1;
            static int projectileDamage = 0;
            static int healthPercentage = 100;
            std::vector<const char*> textureIDs = assetMg->GetTextureAssetIDs();

            if(ImGui::CollapsingHeader("Transform component", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::InputInt2("Position", positions);
                ImGui::InputFloat2("Size", size);
                ImGui::SliderAngle("Rotation (deg)", &rotation, 0, 360);
            }

            if(ImGui::CollapsingHeader("Sprite component", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Combo("Sprite assetID", &selectedItem, textureIDs.data(), textureIDs.size());
            }

            if(ImGui::CollapsingHeader("Projectile emitter component", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SliderFloat("Projectile speed", &projectileSpeed, 1, 1000);
                ImGui::InputFloat("Projectile repeat frequency (secs)", &projectileRepeatFrequency);
                ImGui::InputInt("Projectile damage", &projectileDamage);
            }

            if(ImGui::CollapsingHeader("Health component", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::InputInt("Health", &healthPercentage);
            }

            ImGui::Spacing();

            if(ImGui::Button("Spawn")) {
                Entity enemy = registry->CreateEntity();
                enemy.Group("enemies");
                enemy.AddComponent<TransformComponent>(glm::vec2(positions[0], positions[1]), glm::vec2(size[0], size[1]), RAD2DEG(rotation));
                enemy.AddComponent<RigidBodyComponent>();
                enemy.AddComponent<SpriteComponent>(textureIDs[selectedItem], 32, 32, 2);
                enemy.AddComponent<BoxColliderComponent>(32, 32);
                enemy.AddComponent<DebugRenderComponent>(true);
                enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(cos(rotation) * projectileSpeed, sin(rotation) * projectileSpeed), projectileRepeatFrequency * 1000, 2000, projectileDamage);
                enemy.AddComponent<HealthComponent>(healthPercentage);
                enemy.AddComponent<HealthUIComponent>(glm::vec2(32, 0));
            }
        }
        ImGui::End();

        windowFlags = 
            ImGuiWindowFlags_NoDecoration | 
            ImGuiWindowFlags_NoMove | 
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoNav |
            ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_AlwaysAutoResize;
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.9f);
        if(ImGui::Begin("Map coordinates", NULL, windowFlags)) {
            ImGuiIO io = ImGui::GetIO();
            ImGui::Text("Map coordinates x : %.1f y : %.1f", io.MousePos.x + camera.x, io.MousePos.y + camera.y);
        }
        ImGui::End();

        ImGui::Render();
        ImGuiSDL::Render(ImGui::GetDrawData());
    }
};

#endif