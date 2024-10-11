#include "LevelLoader.h"
#include "../Logger/Logger.h"
#include "../../libs/sol/sol.hpp"

#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/DebugRenderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardMovementSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/ProjectileLifeCycleSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/HealthUISystem.h"
#include "../Systems/RenderGUISystem.h"

#include <fstream>

LevelLoader::LevelLoader() {
    LOG("level loader constructor");
}

LevelLoader::~LevelLoader() {
    LOG("level loader destructor");
}

glm::vec2 LevelLoader::LoadLevel(
    std::unique_ptr<Registry>& registry, 
    std::unique_ptr<AssetManager>& assetManager, 
    SDL_Renderer* renderer, 
    int windowWidth,
    int windowHeight,
    int level) 
{
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    assetManager->AddTexture(renderer, "tank_image_right", "./assets/images/tank-tiger-right.png");
    assetManager->AddTexture(renderer, "tank_image_left", "./assets/images/tank-tiger-left.png");
    assetManager->AddTexture(renderer, "truck_image", "./assets/images/truck-ford-right.png");
    assetManager->AddTexture(renderer, "chopper_spritesheet", "./assets/images/chopper-spritesheet.png");
    assetManager->AddTexture(renderer, "radar_image", "./assets/images/radar.png");
    assetManager->AddTexture(renderer, "jungle_map", "./assets/tilemaps/jungle.png");
    assetManager->AddTexture(renderer, "bullet_image", "./assets/images/bullet.png");
    assetManager->AddTexture(renderer, "tree_image", "./assets/images/tree.png");
    assetManager->AddFont("charriot_font", "./assets/fonts/charriot.ttf", 20);

    Entity tank = registry->CreateEntity();
    tank.Group("enemies");
    tank.AddComponent<TransformComponent>(glm::vec2(400, 620), glm::vec2(1, 1), 0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(0, 0));
    tank.AddComponent<SpriteComponent>("tank_image_right", 32, 32, 2);
    tank.AddComponent<BoxColliderComponent>(32, 32);
    tank.AddComponent<DebugRenderComponent>(true);
    tank.AddComponent<ProjectileEmitterComponent>(glm::vec2(100, 0), 2000, 2000);
    tank.AddComponent<HealthComponent>();
    tank.AddComponent<HealthUIComponent>(glm::vec2(32, 0));

    Entity truck = registry->CreateEntity();
    truck.Group("enemies");
    truck.AddComponent<TransformComponent>(glm::vec2(540, 620), glm::vec2(1, 1), 0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(100, 0));
    truck.AddComponent<SpriteComponent>("truck_image", 32, 32, 1);
    truck.AddComponent<BoxColliderComponent>(32, 32);
    truck.AddComponent<DebugRenderComponent>(true);
    truck.AddComponent<ProjectileEmitterComponent>(glm::vec2(0, 100), 3000, 3000);
    truck.AddComponent<HealthComponent>();
    truck.AddComponent<HealthUIComponent>(glm::vec2(32, 0));

    Entity chopper = registry->CreateEntity();
    chopper.Tag("player");
    chopper.AddComponent<TransformComponent>(glm::vec2(0, 0), glm::vec2(1, 1), 0);
    chopper.AddComponent<RigidBodyComponent>(glm::vec2(0, 0));
    chopper.AddComponent<SpriteComponent>("chopper_spritesheet", 32, 32, 3);
    chopper.AddComponent<BoxColliderComponent>(32, 32);
    chopper.AddComponent<DebugRenderComponent>(true);
    chopper.AddComponent<AnimationComponent>(2, 5, true);
    chopper.AddComponent<KeyboardControlledComponent>(glm::vec2(0, -80), glm::vec2(80, 0), glm::vec2(0, 80), glm::vec2(-80, 0));
    chopper.AddComponent<CameraFollowComponent>();
    chopper.AddComponent<ProjectileEmitterComponent>(glm::vec2(1000, 1000), 0, 10000, 10, true);
    chopper.AddComponent<HealthComponent>();
    chopper.AddComponent<HealthUIComponent>(glm::vec2(32, 0));

    Entity treeA = registry->CreateEntity();
    treeA.Group("obstacles");
    treeA.AddComponent<TransformComponent>(glm::vec2(720, 620), glm::vec2(1, 1), 0);
    treeA.AddComponent<SpriteComponent>("tree_image", 16, 32, 3);
    treeA.AddComponent<BoxColliderComponent>(16, 32);

    Entity treeB = registry->CreateEntity();
    treeB.Group("obstacles");
    treeB.AddComponent<TransformComponent>(glm::vec2(530, 620), glm::vec2(1, 1), 0);
    treeB.AddComponent<SpriteComponent>("tree_image", 16, 32, 3);
    treeB.AddComponent<BoxColliderComponent>(16, 32);

    Entity radar = registry->CreateEntity();
    radar.AddComponent<TransformComponent>(glm::vec2(windowWidth - 74, windowHeight - 74), glm::vec2(1, 1), 0);
    radar.AddComponent<SpriteComponent>("radar_image", 64, 64, 3, 0, 0, true);
    radar.AddComponent<AnimationComponent>(8, 3, true);

    Entity label = registry->CreateEntity();
    label.AddComponent<TransformComponent>(glm::vec2(windowWidth / 2 - 40, 10), glm::vec2(1, 1), 0);
    label.AddComponent<TextLabelComponent>("chopper 1.0", "charriot_font", SDL_Color{0, 255, 0}, true);

    glm::vec2 mapSize = glm::vec2(0);

    std::fstream file("./assets/tilemaps/jungle.map");
    const float tileSize = 32.f;
    const float tileScale = 2.5f;
    const int numMapStride = 10;
    int x = 0, y = 0;
    std::string line;
    while(std::getline(file, line)) {
        int pos;
        int idx;
        while((pos = line.find(',')) != std::string::npos) {
            idx = stoi(line.substr(0, pos));

            Entity tile = registry->CreateEntity();
            tile.Tag("tiles");
            tile.AddComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(tileScale, tileScale), 0);
            tile.AddComponent<SpriteComponent>("jungle_map", tileSize, tileSize, 0, (idx % numMapStride) * tileSize, (idx / numMapStride) * tileSize);

            line.erase(0, pos + 1);
            x += tileSize * tileScale;
        }

        idx = stoi(line);

        Entity tile = registry->CreateEntity();
        tile.Tag("tiles");
        tile.AddComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(tileScale, tileScale), 0);
        tile.AddComponent<SpriteComponent>("jungle_map", tileSize, tileSize, 0, (idx % numMapStride) * tileSize, (idx / numMapStride) * tileSize);
        x += tileSize * tileScale;
        mapSize.x = x;
        x = 0;

        y += tileSize * tileScale;
        mapSize.y = y;
    }
    file.close();

    return mapSize;
}