#include "AIManager.h"

#include <list>
#include <lcs.h>

extern std::atomic<Game*> game;

//LConsoleScreen lcs; // for debug

AIManager::AIManager()
{
	process_finished = true;
}

void AIManager::AddEnemy(AIComponent* c)
{
	enemies.emplace_back(c);
}

void AIManager::Read()
{
	process_finished = false;
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
    //lcs.Init(map.width, map.height, 16);
}

void AIManager::StartProcess()
{
    ai_thread = std::thread(&AIManager::AIProcess, this);
}

void AIManager::AIProcess()
{
    lecs::EntityManager* entity_man = game.load()->ecs_managers.entity_manager;

    //auto DebugMap = [&](AIComponent* e) 
    //{ 
    //    lcs.FullFill(' ', BG_BLACK);
    //    for (int x = 0; x < map.width; ++x)
    //    {
    //        for (int y = 0; y < map.height; ++y)
    //        {
    //            if (nodes[y * map.width + x].isObstacle)
    //            {
    //                lcs.Draw(x, y, PIXEL_SOLID);
    //            }
    //        }
    //    }
    //    for (auto& p : e->path)
    //    {
    //        lcs.Draw(p.x, p.y, PIXEL_SOLID, FG_YELLOW);
    //    }
    //    lcs.Draw(n_start->pos.x, n_start->pos.y, PIXEL_SOLID, FG_GREEN);
    //    lcs.Draw(n_end->pos.x, n_end->pos.y, PIXEL_SOLID, FG_RED);
    //    lcs.Display();
    //};

    // ai clock
    DeltaTime delta_time = 0u;
    sf::Clock delta_clock;

    // main process
	for (;;)
	{
		while (process_finished == true) {}

		for (auto& e : enemies)
		{
            lecs::Entity* enemy = &entity_man->GetEntity(e->entity);

            // update gun pt dir
            if (*e->movement.load() != Vector2Df::Zero())
            {
                e->gun_pt_dir.store(new Vector2Df(*e->movement.load()));
            }

            // if dead no action
            if (enemy->GetComponent<HealthComponent>().is_dead)
            {
                e->Dead();
                continue;
            }
            
            // gen idle action
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
                IdleWalking(enemy);
                break;

            default:
                break;
            }

            // debug pathfinding
            //DebugMap(e);

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

		process_finished = true;
        delta_time = static_cast<float>(delta_clock.restart().asMicroseconds()) / 1000;
	}
}

AIComponent::STATE AIManager::IdleActionSelect()
{
    return AIComponent::STATE(rand() % (AIComponent::STATE::SIZE - 1) + 1);
}

void AIManager::IdleWalking(lecs::Entity* enemy)
{
    AIComponent* e = &enemy->GetComponent<AIComponent>();
    TransformComponent* transform = &enemy->GetComponent<TransformComponent>();

    if (e->path.empty())
    {
        // initialize the path
        SetDest(e, transform);
        SolveAStar();
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
    e->movement.store(new Vector2Df((scaled_dest - transform->position).Normalize()));

    // check arrived to pop
    if ((scaled_dest - transform->position).Magnitude() <= 16 * game.load()->world_scale.x)
    {
        if (e->path.size() == 1)
        {
            e->ended_path = new Vector2Di(e->path.front());
        }
        e->path.pop_front();
    }

    // reset state
    if (e->path.empty())
    {
        e->movement.store(new Vector2Df(0.f, 0.f));
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

void AIManager::SetDest(AIComponent* e, TransformComponent* transform)
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
