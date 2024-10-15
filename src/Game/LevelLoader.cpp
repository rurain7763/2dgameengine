#include "LevelLoader.h"
#include "../Logger/Logger.h"

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
#include "../Systems/ScriptSystem.h"

#include <fstream>
#include <SDL2/SDL.h>

LevelLoader::LevelLoader() {
    LOG("level loader constructor");
}

LevelLoader::~LevelLoader() {
    LOG("level loader destructor");
}

glm::vec2 LevelLoader::LoadLevel(
    sol::state& lua,
    std::unique_ptr<Registry>& registry, 
    std::unique_ptr<AssetManager>& assetManager, 
    SDL_Renderer* renderer, 
    int windowWidth,
    int windowHeight,
    int level) 
{
    sol::load_result script = lua.load_file("./assets/scripts/Level" + std::to_string(level) + ".lua");
    if(!script.valid()) {
        sol::error err = script;
        LOG_ERR("Err load lua file : %s", err.what());
        return glm::vec2(0);
    }

    script();

    sol::table levelTable = lua["Level"];

    // lua가 원하는 asset 로드
    sol::table assets = levelTable["assets"];
    int i = 0;
    while(true) {
        sol::optional<sol::table> assetOpt = assets[i];
        if(assetOpt == sol::nullopt) break;
        sol::table asset = assets[i];
        std::string type = asset["type"];
        if(type == "texture") {
            assetManager->AddTexture(renderer, asset["id"], asset["file"]);
        } else if(type == "font") {
            assetManager->AddFont(asset["id"], asset["file"], asset["font_size"]);
        }
        i++;
    }

    sol::table entities = levelTable["entities"];
    i = 0;
    while(true) {
        sol::optional<sol::table> entityOpt = entities[i];
        if(entityOpt == sol::nullopt) break;
        sol::table entity = entities[i];

        Entity newEntity = registry->CreateEntity();

        sol::optional<std::string> tagOpt = entity["tag"];
        if(tagOpt != sol::nullopt) {
            newEntity.Tag(tagOpt.value());
        }

        sol::optional<std::string> groupOpt = entity["group"];
        if(groupOpt != sol::nullopt) {
            newEntity.Group(groupOpt.value());
        }

        sol::table components = entity["components"];

        sol::optional<sol::table> transformOpt = components["transform"];
        if(transformOpt != sol::nullopt) {
            sol::table transform = transformOpt.value();
            newEntity.AddComponent<TransformComponent>(
                glm::vec2(transform["position"]["x"].get_or(0), transform["position"]["y"].get_or(0)),
                glm::vec2(transform["scale"]["x"].get_or(1), transform["scale"]["y"].get_or(1)),
                transform["rotation"].get_or(0)
            );
        }

        sol::optional<sol::table> rigidBodyOpt = components["rigidbody"];
        if(rigidBodyOpt != sol::nullopt) {
            sol::table rigidBody = rigidBodyOpt.value();
            newEntity.AddComponent<RigidBodyComponent>(
                glm::vec2(rigidBody["velocity"]["x"].get_or(0), rigidBody["velocity"]["y"].get_or(0))
            );
        }

        sol::optional<sol::table> spriteOpt = components["sprite"];
        if(spriteOpt != sol::nullopt) {
            sol::table sprite = spriteOpt.value();
            newEntity.AddComponent<SpriteComponent>(
                sprite["texture_asset_id"],
                sprite["width"].get_or(0),
                sprite["height"].get_or(0),
                sprite["z_index"].get_or(0),
                sprite["src_rect_x"].get_or(0),
                sprite["src_rect_y"].get_or(0),
                sprite["is_fixed"].get_or(false)
            );
        }

        sol::optional<sol::table> animationOpt = components["animation"];
        if(animationOpt != sol::nullopt) {
            sol::table animation = animationOpt.value();
            newEntity.AddComponent<AnimationComponent>(
                animation["num_frames"].get_or(0),
                animation["speed_rate"].get_or(0),
                animation["is_looping"].get_or(false)
            );
        }

        sol::optional<sol::table> boxColliderOpt = components["box_collider"];
        if(boxColliderOpt != sol::nullopt) {
            sol::table boxCollider = boxColliderOpt.value();
            newEntity.AddComponent<BoxColliderComponent>(
                boxCollider["width"].get_or(0),
                boxCollider["height"].get_or(0),
                glm::vec2(boxCollider["offset"]["x"].get_or(0), boxCollider["offset"]["y"].get_or(0))
            );
        }

        sol::optional<sol::table> debugRenderOpt = components["debug_render"];
        if(debugRenderOpt != sol::nullopt) {
            sol::table debugRender = debugRenderOpt.value();
            newEntity.AddComponent<DebugRenderComponent>(
                debugRender["is_render_box_collider"].get_or(false)
            );
        }

        sol::optional<sol::table> keyboardControlledOpt = components["keyboard_controller"];
        if(keyboardControlledOpt != sol::nullopt) {
            sol::table keyboardControlled = keyboardControlledOpt.value();
            newEntity.AddComponent<KeyboardControlledComponent>(
                glm::vec2(keyboardControlled["up_velocity"]["x"].get_or(0), keyboardControlled["up_velocity"]["y"].get_or(0)),
                glm::vec2(keyboardControlled["right_velocity"]["x"].get_or(0), keyboardControlled["right_velocity"]["y"].get_or(0)),
                glm::vec2(keyboardControlled["down_velocity"]["x"].get_or(0), keyboardControlled["down_velocity"]["y"].get_or(0)),
                glm::vec2(keyboardControlled["left_velocity"]["x"].get_or(0), keyboardControlled["left_velocity"]["y"].get_or(0))
            );
        }

        sol::optional<sol::table> cameraFollowOpt = components["camera_follow"];
        if(cameraFollowOpt != sol::nullopt) {
            sol::table cameraFollow = cameraFollowOpt.value();
            newEntity.AddComponent<CameraFollowComponent>();
        }

        sol::optional<sol::table> projectileEmitterOpt = components["projectile_emitter"];
        if(projectileEmitterOpt != sol::nullopt) {
            sol::table projectileEmitter = projectileEmitterOpt.value();
            newEntity.AddComponent<ProjectileEmitterComponent>(
                glm::vec2(projectileEmitter["projectile_velocity"]["x"].get_or(0), projectileEmitter["projectile_velocity"]["y"].get_or(0)),
                projectileEmitter["repeat_frequency"].get_or(0) * 1000,
                projectileEmitter["projectile_duration"].get_or(0) * 1000,
                projectileEmitter["hit_percentage_damage"].get_or(0),
                projectileEmitter["is_friendly"].get_or(false)
            );
        }

        sol::optional<sol::table> healthOpt = components["health"];
        if(healthOpt != sol::nullopt) {
            sol::table health = healthOpt.value();
            newEntity.AddComponent<HealthComponent>(
                health["health_percentage"].get_or(0)
            );
        }

        sol::optional<sol::table> healthUIOpt = components["health_ui"];
        if(healthUIOpt != sol::nullopt) {
            sol::table healthUI = healthUIOpt.value();
            newEntity.AddComponent<HealthUIComponent>(
                glm::vec2(healthUI["offset"]["x"].get_or(0), healthUI["offset"]["y"].get_or(0))
            );
        }

        sol::optional<sol::table> scriptOpt = components["on_update_script"];
        if(scriptOpt != sol::nullopt) {
            sol::function func = scriptOpt.value()[0];
            newEntity.AddComponent<ScriptComponent>(func);
        }
        i++;
    }

    glm::vec2 mapSize = glm::vec2(0);

    sol::table tileMap = levelTable["tilemap"];

    std::string mapFilePath = tileMap["map_file"];
    std::string mapAssetID = tileMap["texture_asset_id"];

    const float tileSize = tileMap["tile_size"];
    const float tileScale = tileMap["scale"];
    const int numMapStride = 10;

    std::fstream file(mapFilePath);
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
            tile.AddComponent<SpriteComponent>(mapAssetID, tileSize, tileSize, 0, (idx % numMapStride) * tileSize, (idx / numMapStride) * tileSize);

            line.erase(0, pos + 1);
            x += tileSize * tileScale;
        }

        idx = stoi(line);

        Entity tile = registry->CreateEntity();
        tile.Tag("tiles");
        tile.AddComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(tileScale, tileScale), 0);
        tile.AddComponent<SpriteComponent>(mapAssetID, tileSize, tileSize, 0, (idx % numMapStride) * tileSize, (idx / numMapStride) * tileSize);
        x += tileSize * tileScale;
        mapSize.x = x;
        x = 0;

        y += tileSize * tileScale;
        mapSize.y = y;
    }
    file.close();

    return mapSize;
}