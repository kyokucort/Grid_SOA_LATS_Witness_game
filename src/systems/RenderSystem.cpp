#include "RenderSystem.hpp"


std::vector<DrawCmd> queue;

void CollectRenders(World& world, AssetManager assets)
{
    for (int i=0 ; i < MAX_ENTITIES; i++)
    {
        if (!world.entity.alive[i]) continue;

        DrawCmd cmd;
        
        cmd.tex = &AssetsGet(assets, world.render.texture[i]);
        cmd.src = world.render.src[i];
        cmd.position = world.transform.pos[i];
        cmd.layer = world.render.layer[i];

        Rectangle _dest = {
            cmd.position.x - ((world.transform.size[i].x * world.transform.scale[i].x) / 2), 
            cmd.position.y - ((world.transform.size[i].y * world.transform.scale[i].y) / 2), 
            world.transform.size[i].x * world.transform.scale[i].x, 
            world.transform.size[i].y * world.transform.scale[i].y
        };

        cmd.dst = _dest;

        queue.push_back(cmd);
    }

    //
    // TO DO: get rendered element from level
    //


    
    std::sort(queue.begin(), queue.end(),
          [](const DrawCmd& a, const DrawCmd& b)
          {
              return a.layer < b.layer;
          });
}

void DrawPool(World& world, AssetManager assets)
{  
    queue.clear();
    CollectRenders(world, assets);
    for(auto& cmd : queue)
    {
        DrawTexturePro(*cmd.tex, cmd.src, cmd.dst, {0, 0}, 0.0f, WHITE);
    }

    for (int i=0 ; i < world.loaded_levels.size(); i++)
    {
        DrawLevel(world, i);
    }
    
}

void DrawLevel(World& world, int index)
{
    Grid _grid = world.loaded_levels[index].grid;
    for (int _x=0; _x < _grid.width; _x++){
        for (int _y=0; _y < _grid.height; _y++){
                Vector2 _draw_pos = {_grid.position.x + (_x * _grid.cell_size), _grid.position.y + (_y * _grid.cell_size)};
                Rectangle _rec = {_draw_pos.x, _draw_pos.y, _grid.cell_size, _grid.cell_size};
                DrawRectangleLinesEx(_rec, 0.5f, BLACK);
        }
    }
    DrawText(TextFormat("Level : %i", index), _grid.position.x, _grid.position.y, 32, RED);
    Rectangle _border = {_grid.position.x, _grid.position.y, _grid.width * _grid.cell_size, _grid.height * _grid.cell_size};
    DrawRectangleLinesEx(_border, 4.0f, GRAY);
}


void DrawCursor(World& world) {
    DrawCircleV(world.cursor.position, 32, BLUE);
}

/*
void DrawPool(World world)
{
    for (int i=0 ; i < MAX_ENTITIES; i++)
    {
        if (!world.entity.alive[i]) continue;
        switch (world.entity.type[i]) 
        {
            case EntityType::ENTITY_PLAYER:
            DrawPlayer(world.transform.pos[i], {10, 10}, world.render.color[i]);
            break;
            case EntityType::ENTITY_LEVEL:
            DrawLevel(world.transform.pos[i], {10, 10}, world.render.color[i]);
            break;
            default:
            break;
        }
    }
}
*/

void DrawPlayer(Vector2 position, Vector2 size, Color color)
{
    DrawCircleV(position, size.x, color);
}




void DrawWorld(World& world, AssetManager& assets)
{
    DrawPool(world, assets);
}
