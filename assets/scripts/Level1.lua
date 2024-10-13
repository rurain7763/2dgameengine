-- Define a table with the values of the first level
Level = {
    ----------------------------------------------------
    -- Table to define the list of assets
    ----------------------------------------------------
    assets = {
        [0] =
        { type = "texture", id = "tank_image_right", file = "./assets/images/tank-tiger-right.png" },
        { type = "texture", id = "tank_image_left", file = "./assets/images/tank-tiger-left.png" },
        { type = "texture", id = "truck_image",    file = "./assets/images/truck-ford-right.png" },
        { type = "texture", id = "chopper_spritesheet",  file = "./assets/images/chopper-spritesheet.png" },
        { type = "texture", id = "radar_image",  file = "./assets/images/radar.png" },
        { type = "texture", id = "jungle_map",  file = "./assets/tilemaps/jungle.png" },
        { type = "texture", id = "bullet_image",  file = "./assets/images/bullet.png" },
        { type = "texture", id = "tree_image",  file = "./assets/images/tree.png" },
        { type = "font"   , id = "charriot_font",    file = "./assets/fonts/charriot.ttf", font_size = 20 },
    },

    ----------------------------------------------------
    -- table to define the map config variables
    ----------------------------------------------------
    tilemap = {
        map_file = "./assets/tilemaps/jungle.map",
        texture_asset_id = "jungle_map",
        tile_size = 32,
        scale = 2.5
    },

    ----------------------------------------------------
    -- table to define entities and their components
    ----------------------------------------------------
    entities = {
        [0] =
        {
            -- Player
            tag = "player",
            components = {
                transform = {
                    position = { x = 242, y = 110 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                sprite = {
                    texture_asset_id = "chopper_spritesheet",
                    width = 32,
                    height = 32,
                    z_index = 4,
                    fixed = false,
                    src_rect_x = 0,
                    src_rect_y = 0
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10, -- fps
                    is_looping = true
                },
                box_collider = {
                    width = 32,
                    height = 25,
                    offset = { x = 0, y = 5 }
                },
                debug_render = {
                    is_render_box_collider = true
                },
                health = {
                    health_percentage = 100
                },
                health_ui = {
                    offset = { x = 32, y = 0 }
                },
                projectile_emitter = {
                    projectile_velocity = { x = 200, y = 200 },
                    projectile_duration = 10, -- seconds
                    repeat_frequency = 0, -- seconds
                    hit_percentage_damage = 10,
                    is_friendly = true
                },
                keyboard_controller = {
                    up_velocity = { x = 0, y = -50 },
                    right_velocity = { x = 50, y = 0 },
                    down_velocity = { x = 0, y = 50 },
                    left_velocity = { x = -50, y = 0 }
                },
                camera_follow = { }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 400, y = 620 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank_image_right",
                    width = 32,
                    height = 32,
                    z_index = 2
                },
                box_collider = {
                    width = 25,
                    height = 18,
                    offset = { x = 0, y = 7 }
                },
                debug_render = {
                    is_render_box_collider = true
                },
                health = {
                    health_percentage = 100
                },
                health_ui = {
                    offset = { x = 32, y = 0 }
                },
                projectile_emitter = {
                    projectile_velocity = { x = 100, y = 0 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 20,
                    is_friendly = false
                }
            }
        }
    }
}
