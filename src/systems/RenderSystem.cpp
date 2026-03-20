#include "RenderSystem.hpp"
#include "modules/math/grid_math.hpp"
#include "editor/Editor.hpp"


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
            if (world.hover.hovered[i] && world.path.has[i])
            {
                cmd.color = BLUE;
            }

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

        
    }

    void DrawColliders(World& w)
    {
        for (int i=0 ; i < MAX_ENTITIES; i++)
        {
            if (!w.entity.alive[i]) continue;
            DrawRectangleLinesEx(w.collider.bounds[i], 2.0f, PINK);
        }

    }

    void DrawGrid(World& w)
    {
        Grid _grid = w.global_grid;
        for (int _x=0; _x < _grid.width; _x++){
            for (int _y=0; _y < _grid.height; _y++){
                    Vector2 _draw_pos = {_grid.origin.x + (_x * _grid.cell_size), _grid.origin.y + (_y * _grid.cell_size)};
                    Rectangle _rec = {_draw_pos.x, _draw_pos.y, _grid.cell_size, _grid.cell_size};
                    DrawRectangleLinesEx(_rec, 2.0f, GRAY);
            }
        }
        for (int i = 0; i < _grid.cells.size(); i++)
        {
            Vector2i _coords = CellCoords(i, _grid.width);
            Vector2 _center =  CellCenter(_coords, _grid.origin, _grid.cell_size);
            DrawText(TextFormat("Entities = %i", _grid.cells[i].count), _center.x - 32, _center.y - 32, 10, GRAY);
            DrawText(TextFormat("%i - %i", _coords.x, _coords.y), _center.x - 32, _center.y - 16, 10, GRAY);
        }

    }

    void DrawPath(World& w)
    {
        Grid _grid = w.global_grid;

        for (int i=0 ; i < MAX_ENTITIES; i++)
        {
            if (!w.entity.alive[i]) continue;
            if (!w.path.has[i]) continue;

            std::vector<Vector2i>& _path = w.path.path[i].points;

            if (_path.size() < 2) continue;

            for (int p=0; p < _path.size() - 1; p++)
            {
                Vector2 a;
                Vector2 b;
                a.x = _grid.origin.x + (_path[p].x * CELL_SIZE_WORLD) + CELL_SIZE_WORLD/2;
                a.y = _grid.origin.y + (_path[p].y * CELL_SIZE_WORLD) + CELL_SIZE_WORLD/2;
                b.x = _grid.origin.x + (_path[p+1].x * CELL_SIZE_WORLD) + CELL_SIZE_WORLD/2;
                b.y = _grid.origin.y + (_path[p+1].y * CELL_SIZE_WORLD) + CELL_SIZE_WORLD/2;

                DrawLineEx(a, b, 16, Fade(BLUE, 0.8f));
            }
        }
    }

    void DrawWorld(World& world, AssetManager& assets)
    {
        DrawGrid(world);
        DrawPool(world, assets);
        DrawPath(world);
        DrawColliders(world);
        if (world.editor.enabled)
        {
            Editor::Draw(world);
        }
    }
}
