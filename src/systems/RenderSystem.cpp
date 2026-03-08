#include "RenderSystem.hpp"


namespace RenderSystem
{
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
            cmd.color = world.render.color[i];

            queue.push_back(cmd);
        }


        
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
            DrawTexturePro(*cmd.tex, cmd.src, cmd.dst, {0, 0}, 0.0f, cmd.color);
        }

        for (int i=0 ; i < world.loaded_levels.size(); i++)
        {
            DrawLevel(world, i);
        }

        for (int i=0 ; i < MAX_ENTITIES; i++)
        {
            if (!world.entity.alive[i]) continue;
            //DrawRectangleLinesEx(world.collider.bounds[i], 2.0f, PINK);
        }
        
    }



    void DrawLevel(World& world, int index)
    {
        Grid _grid = world.loaded_levels[index].grid;
        for (int _x=0; _x < _grid.width; _x++){
            for (int _y=0; _y < _grid.height; _y++){
                    Vector2 _draw_pos = {_grid.position.x + (_x * _grid.cell_size), _grid.position.y + (_y * _grid.cell_size)};
                    Rectangle _rec = {_draw_pos.x, _draw_pos.y, _grid.cell_size, _grid.cell_size};
                    DrawRectangleLinesEx(_rec, 2.0f, GRAY);
            }
        }

        for (int i = 0; i < _grid.cells.size(); i++)
        {
            DrawCircleV(_grid.cells[i].center, 2.0f, RAYWHITE);
            DrawText(TextFormat("%0.1f - %0.1f", _grid.cells[i].coords.x, _grid.cells[i].coords.y), _grid.cells[i].center.x - 48, _grid.cells[i].center.y - 48, 12, WHITE);
            DrawText(TextFormat("WALL = %i", _grid.cells[i].is_wall), _grid.cells[i].center.x - 48, _grid.cells[i].center.y - 24, 12, WHITE);
        }

        if (_grid.path.size() > 1){
            for (int i = 0; i < _grid.path.size() - 1; i++)
            {
                Vector2 a;
                Vector2 b;
                a.x = _grid.position.x + (_grid.path[i].x * CELL_SIZE_WORLD) + CELL_SIZE_WORLD/2;
                a.y = _grid.position.y + (_grid.path[i].y * CELL_SIZE_WORLD) + CELL_SIZE_WORLD/2;
                b.x = _grid.position.x + (_grid.path[i+1].x * CELL_SIZE_WORLD) + CELL_SIZE_WORLD/2;
                b.y = _grid.position.y + (_grid.path[i+1].y * CELL_SIZE_WORLD) + CELL_SIZE_WORLD/2;

                DrawLineEx(a, b, 16, Fade(RAYWHITE, 0.8f));
            }
        }

        DrawText(TextFormat("Level : %i", index), _grid.position.x, _grid.position.y, 32, RED);
        //DrawText(TextFormat("Collisions : %i", world.collision_events.size()), _grid.position.x, _grid.position.y + 128, 32, WHITE);
        DrawText(TextFormat("PATH SIZE : %i", _grid.path.size()), _grid.position.x, _grid.position.y + 128, 32, WHITE);
        Rectangle _border = {_grid.position.x, _grid.position.y, _grid.width * _grid.cell_size, _grid.height * _grid.cell_size};
        DrawRectangleLinesEx(_border, 4.0f, GRAY);
        if (_grid.path.size() > 0){
            DrawText(TextFormat("Last Node : %0.1f - %0.1f", _grid.path.back().x, _grid.path.back().y), _grid.position.x, _grid.position.y + 256, 32, WHITE);
            DrawText(TextFormat("First Node : %0.1f - %0.1f", _grid.path[0].x, _grid.path[0].y), _grid.position.x, _grid.position.y + 512, 32, WHITE);
        }
    }


    void DrawWorld(World& world, AssetManager& assets)
    {
        DrawPool(world, assets);
    }

}
