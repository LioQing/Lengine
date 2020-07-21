#include "AIManager.h"

#include <list>
#include <lcs.h>
#include <SFML/Graphics.hpp>

#include "../SpawnEntity.h"

extern std::atomic<Game*> game;

namespace intersect
{
    // Given three colinear points p, q, r, the function checks if 
    // point q lies on line segment 'pr' 
    bool onSegment(Vector2Df p, Vector2Df q, Vector2Df r)
    {
        if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
            q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
            return true;

        return false;
    }

    // To find orientation of ordered triplet (p, q, r). 
    // The function returns following values 
    // 0 --> p, q and r are colinear 
    // 1 --> Clockwise 
    // 2 --> Counterclockwise 
    int orientation(Vector2Df p, Vector2Df q, Vector2Df r)
    {
        // See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
        // for details of below formula. 
        int val = (q.y - p.y) * (r.x - q.x) -
            (q.x - p.x) * (r.y - q.y);

        if (val == 0) return 0;  // colinear 

        return (val > 0) ? 1 : 2; // clock or counterclock wise 
    }

    // The main function that returns true if line segment 'p1q1' 
    // and 'p2q2' intersect. 
    bool doIntersect(Vector2Df p1, Vector2Df q1, Vector2Df p2, Vector2Df q2)
    {
        // Find the four orientations needed for general and 
        // special cases 
        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);

        // General case 
        if (o1 != o2 && o3 != o4)
            return true;

        // Special Cases 
        // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
        if (o1 == 0 && onSegment(p1, p2, q1)) return true;

        // p1, q1 and q2 are colinear and q2 lies on segment p1q1 
        if (o2 == 0 && onSegment(p1, q2, q1)) return true;

        // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
        if (o3 == 0 && onSegment(p2, p1, q2)) return true;

        // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
        if (o4 == 0 && onSegment(p2, q1, q2)) return true;

        return false; // Doesn't fall in any of the above cases 
    }
}

LConsoleScreen lcs; // for debug

AIManager::AIManager()
{
	process_finished = true;
}

void AIManager::AddEnemy(AIComponent* c)
{
	enemies.emplace_back(c);
}

void AIManager::Read(lecs::EntityManager* entity_manager)
{
    lecs::Entity* player = entity_manager->GetGroup(lecs::GRP_PLAYER).entities.at(0);

    *player_pos.load() = player->GetComponent<TransformComponent>().position;
    *player_vel.load() = player->GetComponent<TransformComponent>().velocity;
    is_player_dead.store(player->GetComponent<HealthComponent>().is_dead);
}

void AIManager::ReadMap()
{
    LevelComponent* lvl = &game.load()->ecs_managers.entity_manager->EntityFilter<LevelComponent>().entities.at(0)->GetComponent<LevelComponent>();
    map = lvl->map + lvl->static_layer;

    for (int x = 0; x < map.width; ++x)
        for (int y = 0; y < map.height; ++y)
            if (map.At(x, y) != 0) map.At(x, y) = 1;

    // initialize nodes
    nodes = new Node[map.width * map.height];
    for (int x = 0; x < map.width; ++x)
    {
        for (int y = 0; y < map.height; ++y)
        {
            nodes[y * map.width + x].pos = { x, y };
            nodes[y * map.width + x].isObstacle = map.At(x, y);
            nodes[y * map.width + x].parent = nullptr;
            nodes[y * map.width + x].isVisited = false;
        }
    }

    for (int x = 0; x < map.width; ++x)
    {
        for (int y = 0; y < map.height; ++y)
        {
            if (y > 0)
                nodes[y * map.width + x].neighbours.push_back(&nodes[(y - 1) * map.width + (x + 0)]);
            if (y < map.height - 1)
                nodes[y * map.width + x].neighbours.push_back(&nodes[(y + 1) * map.width + (x + 0)]);
            if (x > 0)
                nodes[y * map.width + x].neighbours.push_back(&nodes[(y + 0) * map.width + (x - 1)]);
            if (x < map.width - 1)
                nodes[y * map.width + x].neighbours.push_back(&nodes[(y + 0) * map.width + (x + 1)]);
        }
    }

    rooms = lvl->rooms;
    if (debug_path) lcs.Init(map.width, map.height, 16);
}

void AIManager::SpawnEnemy()
{
    auto Random = [&](int lower, int upper)
    {
        return rand() % (upper - lower + 1) + lower;
    };

    for (auto itr = std::next(rooms.begin()); itr != rooms.end(); ++itr)
    {
        auto& r = *itr;
        int n = static_cast<int>(std::roundf((static_cast<float>(r.width) * r.height) / 25));

        for (int i = 0; i < n; ++i)
        {
            for (;;)
            {
                Vector2Di m_position(Random(r.x, r.x + r.width), Random(r.y, r.y + r.height));
                Vector2Df spawn_pos = m_position.Cast<float>() * game.load()->world_scale * 32.f;
                if (map.At(m_position.x, m_position.y) == 0)
                {
                    spawn::Enemy(spawn_pos, itr - rooms.begin());
                    break;
                }
            }
            
        }
    }
}

void AIManager::StartProcess()
{
    ai_thread = std::thread(&AIManager::AIProcess, this);
}

void AIManager::AIProcess()
{
    lecs::EntityManager* entity_man = game.load()->ecs_managers.entity_manager;

    auto DebugMap = [&](AIComponent* e) 
    { 
        if (!debug_path) return;

        lcs.FullFill(' ', BG_BLACK);
        for (int x = 0; x < map.width; ++x)
        {
            for (int y = 0; y < map.height; ++y)
            {
                if (nodes[y * map.width + x].isObstacle)
                {
                    lcs.Draw(x, y, PIXEL_SOLID);
                }
            }
        }
        for (auto& p : e->path)
        {
            lcs.Draw(p.x, p.y, PIXEL_SOLID, FG_YELLOW);
        }
        lcs.Draw(n_start->pos.x, n_start->pos.y, PIXEL_SOLID, FG_GREEN);
        lcs.Draw(n_end->pos.x, n_end->pos.y, PIXEL_SOLID, FG_RED);
        lcs.Display();
    };

    // ai clock
    DeltaTime delta_time = 0u;
    sf::Clock delta_clock;

    // main process
	for (;;)
	{
		for (auto& e : enemies)
		{
            lecs::Entity* enemy = &entity_man->GetEntity(e->entity);

            if ((enemy->GetComponent<TransformComponent>().position - *player_pos).Magnitude() > 600.f * game.load()->world_scale.x &&
                (e->state == AIComponent::STATE::IDLE || e->state == AIComponent::STATE::IDLE_WALKING))
                continue;

            // if dead no action
            if (enemy->GetComponent<HealthComponent>().is_dead)
            {
                if (*e->movement.load() != Vector2Df::Zero()) e->Dead();
                continue;
            }
            
            // movement action
            // gen idle move action
            if (!e->state)
            {
                e->state = IdleActionSelect();
            }

            // perform action
            switch (e->state)
            {
            case AIComponent::STATE::IDLE:
                IdleTime(enemy, delta_time);
                break;

            case AIComponent::STATE::IDLE_WALKING:
            case AIComponent::STATE::CHASE_PLAYER:
                Walking(enemy);
                break;

            default:
                break;
            }

            // gun action
            // can see player shoot
            Vector2Df e_pos = enemy->GetComponent<TransformComponent>().position;
            if (game.load()->InsideView(sf::FloatRect(e_pos.sfVector2f(), sf::Vector2f(0.f, 0.f))) && CanSeePlayer(enemy))
            {
                *e->gun_pt_dir.load() = (*player_pos.load() - e_pos).Normalize();
                e->is_firing.store(true);
                e->see_player = true;
            }
            else
            {
                // update gun pt dir
                if (*e->movement.load() != Vector2Df::Zero())
                {
                    *e->gun_pt_dir.load() = *e->movement.load();
                }
                e->is_firing.store(false);
                
                // chase player
                if (e->see_player)
                {
                    e->path.clear();
                    e->state = AIComponent::STATE::CHASE_PLAYER;
                    e->see_player = false;
                }
            }

            // debug pathfinding
            if (debug_path) DebugMap(e);

            // debug state
            //switch (e->state)
            //{
            //case AIComponent::STATE::IDLE:
            //    std::cout << "idle" << std::endl;
            //    break;

            //case AIComponent::STATE::IDLE_WALKING:
            //    std::cout << "idle walking" << std::endl;
            //    break;

            //default:
            //    break;
            //}
		}
        delta_time = static_cast<float>(delta_clock.restart().asMicroseconds()) / 1000;
	}
}

bool AIManager::CanSeePlayer(lecs::Entity* enemy)
{
    AIComponent* e = &enemy->GetComponent<AIComponent>();
    TransformComponent* transform = &enemy->GetComponent<TransformComponent>();

    Vector2Df e_pos = transform->position;
    std::bitset<5> blocked = std::bitset<5>().reset();

    for (int x = 0; x < map.width; ++x)
    {
        for (int y = 0; y < map.height; ++y)
        {
            if (!nodes[y * map.width + x].isObstacle) continue;

            Vector2Df obstacle = Vector2Df(x, y) * game.load()->world_scale * 32.f;
            Vector2Df size = game.load()->world_scale * 32.f;

            sf::FloatRect obsRect = { obstacle.sfVector2f(), size.sfVector2f() };
            if (!game.load()->InsideView(obsRect)) continue;

            if (!blocked.test(0))
            {
                if (intersect::doIntersect(e_pos + Vector2Df(-4.f, 0.f) * game.load()->world_scale, *player_pos.load() + Vector2Df(8.f, -16.f) * game.load()->world_scale, obstacle, obstacle + size)) blocked.set(0);
                if (intersect::doIntersect(e_pos + Vector2Df(-4.f, 0.f) * game.load()->world_scale, *player_pos.load() + Vector2Df(8.f, -16.f) * game.load()->world_scale, obstacle + Vector2Df(size.x, 0.f), obstacle + Vector2Df(0.f, size.y))) blocked.set(0);
            }

            if (!blocked.test(1))
            {
                if (intersect::doIntersect(e_pos + Vector2Df(-4.f, 0.f) * game.load()->world_scale, *player_pos.load() + Vector2Df(-8.f, 16.f) * game.load()->world_scale, obstacle, obstacle + size)) blocked.set(1);
                if (intersect::doIntersect(e_pos + Vector2Df(-4.f, 0.f) * game.load()->world_scale, *player_pos.load() + Vector2Df(-8.f, 16.f) * game.load()->world_scale, obstacle + Vector2Df(size.x, 0.f), obstacle + Vector2Df(0.f, size.y))) blocked.set(1);
            }

            if (!blocked.test(2))
            {
                if (intersect::doIntersect(e_pos + Vector2Df(-4.f, 0.f) * game.load()->world_scale, *player_pos.load() + Vector2Df(8.f, 16.f) * game.load()->world_scale, obstacle, obstacle + size)) blocked.set(2);
                if (intersect::doIntersect(e_pos + Vector2Df(-4.f, 0.f) * game.load()->world_scale, *player_pos.load() + Vector2Df(8.f, 16.f) * game.load()->world_scale, obstacle + Vector2Df(size.x, 0.f), obstacle + Vector2Df(0.f, size.y))) blocked.set(2);
            }

            if (!blocked.test(3))
            {
                if (intersect::doIntersect(e_pos + Vector2Df(-4.f, 0.f) * game.load()->world_scale, *player_pos.load() + Vector2Df(-8.f, -16.f) * game.load()->world_scale, obstacle, obstacle + size)) blocked.set(3);
                if (intersect::doIntersect(e_pos + Vector2Df(-4.f, 0.f) * game.load()->world_scale, *player_pos.load() + Vector2Df(-8.f, -16.f) * game.load()->world_scale, obstacle + Vector2Df(size.x, 0.f), obstacle + Vector2Df(0.f, size.y))) blocked.set(3);
            }

            if (!blocked.test(4))
            {
                if (intersect::doIntersect(e_pos + Vector2Df(-4.f, 0.f) * game.load()->world_scale, *player_pos.load(), obstacle, obstacle + size)) blocked.set(4);
                if (intersect::doIntersect(e_pos + Vector2Df(-4.f, 0.f) * game.load()->world_scale, *player_pos.load(), obstacle + Vector2Df(size.x, 0.f), obstacle + Vector2Df(0.f, size.y))) blocked.set(4);
            }
        }
    }


    return !blocked.all();
}

AIComponent::STATE AIManager::IdleActionSelect()
{
    return AIComponent::STATE(rand() % 2 + 1);
}

void AIManager::Walking(lecs::Entity* enemy)
{
    AIComponent* e = &enemy->GetComponent<AIComponent>();
    TransformComponent* transform = &enemy->GetComponent<TransformComponent>();

    if (e->path.empty())
    {
        // initialize the path
        if (e->state == AIComponent::STATE::CHASE_PLAYER)
            ChasePlayerDest(e, transform);
        else
            GenRandPath(e, transform);


        SolveAStar();
        if (e->ended_path) delete e->ended_path;
        e->ended_path = nullptr;

        // assign the path to ai component
        Node* p = n_end;
        while (p->parent != nullptr)
        {
            e->path.push_front(Vector2Di(p->pos.x, p->pos.y));
            p = p->parent;
        }
    }

    // set dest
    if (!e->path.empty() && e->current_path != e->path.front())
        e->current_path = e->path.front();

    // movement
    Vector2Df scaled_dest = Vector2Df((e->current_path.x * 32 + 16) * game.load()->world_scale.x, e->current_path.y * 32 * game.load()->world_scale.y);
    *e->movement.load() = (scaled_dest - transform->position).Normalize();

    // check arrived to pop
    float arrive_radius;
    if (e->path.size() == 1) arrive_radius = 8.f;
    else arrive_radius = 16.f;

    if ((scaled_dest - transform->position).Magnitude() <= arrive_radius * game.load()->world_scale.x)
    {
        if (e->path.size() == 1)
        {
            if (e->ended_path == nullptr) e->ended_path = new Vector2Di(e->path.front());
            else *e->ended_path = e->path.front();
        }

        if (e->path.size() >= 1)
            e->path.pop_front();
    }

    // reset state
    if (e->path.empty())
    {
        *e->movement.load() = Vector2Df(0.f, 0.f);
        e->state = AIComponent::STATE::NONE;
    }
}

void AIManager::IdleTime(lecs::Entity* enemy, DeltaTime dt)
{
    AIComponent* e = &enemy->GetComponent<AIComponent>();

    // set new time threshold
    if (e->timer < 0.f)
    {
        e->idle_time_threshold = rand() % (max_idle - min_idle) + min_idle;
        e->timer == 0.f;
    }

    // time
    e->timer += dt;

    // check if reached threshold
    if (e->timer > e->idle_time_threshold)
    {
        e->timer = -1.f;
        e->state = AIComponent::STATE::NONE;
    }
}

void AIManager::ChasePlayerDest(AIComponent* e, TransformComponent* transform)
{
    // set start position
    Vector2Di start_pos = (transform->position / 32 / game.load()->world_scale).Cast<int>();
    if (nodes[start_pos.y * map.width + start_pos.x].isObstacle) ++start_pos.y;
    n_start = &nodes[start_pos.y * map.width + start_pos.x];

    // set end postition
    Vector2Di end_pos = (*player_pos.load() / 32 / game.load()->world_scale).Cast<int>();
    if (nodes[end_pos.y * map.width + end_pos.x].isObstacle) ++end_pos.y;
    n_end = &nodes[end_pos.y * map.width + end_pos.x];
}

void AIManager::GenRandPath(AIComponent* e, TransformComponent* transform)
{
    // set start position
    Vector2Di start_pos;

    if (e->ended_path)
        start_pos = *e->ended_path;
    else
        start_pos = (transform->position / 32 / game.load()->world_scale).Cast<int>();

    n_start = &nodes[start_pos.y * map.width + start_pos.x];

    // set end position
    Vector2Di end_pos;
    do
    {
        end_pos.x = rand() % rooms.at(e->room).width + rooms.at(e->room).x;
        end_pos.y = rand() % rooms.at(e->room).height + rooms.at(e->room).y;
    }
    while (map.At(end_pos.x, end_pos.y) == 1 || start_pos == end_pos);

    n_end = &nodes[end_pos.y * map.width + end_pos.x];
}

void AIManager::SolveAStar()
{
    for (int x = 0; x < map.width; ++x)
    {
        for (int y = 0; y < map.height; ++y)
        {
            nodes[y * map.width + x].isVisited = false;
            nodes[y * map.width + x].g = std::numeric_limits<float>::max();
            nodes[y * map.width + x].h = std::numeric_limits<float>::max();
            nodes[y * map.width + x].parent = nullptr;
        }
    }

    Node* n_current = n_start;
    n_start->g = 0.f;
    n_start->h = n_start->pos.Distance(n_end->pos);

    std::list<Node*> not_tested_nodes;
    not_tested_nodes.push_back(n_start);

    while (!not_tested_nodes.empty() && n_current != n_end)
    {
        not_tested_nodes.sort([](const Node* lhs, const Node* rhs) { return lhs->h < rhs->h; });

        while (!not_tested_nodes.empty() && not_tested_nodes.front()->isVisited)
            not_tested_nodes.pop_front();

        if (not_tested_nodes.empty()) break;

        n_current = not_tested_nodes.front();
        n_current->isVisited = true;

        for (auto neighbour : n_current->neighbours)
        {
            if (!neighbour->isVisited && !neighbour->isObstacle)
            {
                not_tested_nodes.push_back(neighbour);
            }

            float lowest = n_current->g + n_current->pos.Distance(neighbour->pos);

            if (lowest < neighbour->g)
            {
                neighbour->parent = n_current;
                neighbour->g = lowest;
                neighbour->h = neighbour->g + neighbour->pos.Distance(n_end->pos);
            }
        }
    }
}
