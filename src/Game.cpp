#include "Game.hpp"
#include "Map.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include <sstream>
#include <vector>
#include <fstream>

SDL_Renderer* Game::renderer = nullptr;
Map* map;

Manager manager;

SDL_Event Game::event;

SDL_Rect Game::camera = {0,0,800,640};

AssetManager* Game::assets = new AssetManager(&manager);

auto& player(manager.addEntity());
auto& label(manager.addEntity());
auto& label1(manager.addEntity());
auto& exitEntity(manager.addEntity());
auto& finishedGame(manager.addEntity());
//auto& enemy(manager.addEntity());

auto& endGameLabel(manager.addEntity());

bool Game::isRunning = false;

GameState Game::gameState;

std::vector<Vector2D>lastEnemyPosition;
bool gotItem=false;
int level = 1;

Game::Game() {

}

Game::~Game() {

}

void Game::init(const char* title, int x, int y, int width, int height, bool fullscreen) {
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "SDL initialised! " << std::endl;
		window = SDL_CreateWindow(title, x, y, width, height, flags);
		if (window)
		{
			std::cout << "Window created! " << std::endl;
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			std::cout << "Renderer created! " << std::endl;
		}
		isRunning = true;
	}
	else {
		isRunning = false;
	}

	if(TTF_Init() == -1)
	{
		std::cout<<"Error: SDL_TTF"<<std::endl;
	}

	assets->AddTexture("terrain", "../res/mapTiles32.png");
	assets->AddTexture("player", "../res/doomGuy-walkSprites.png");
    assets->AddTexture("imp", "../res/imp-walkSprites.png");
	assets->AddTexture("projectile", "../res/projectile.png");
	assets->AddFont("arial", "../res/arial.ttf", 16);
	assets->AddFont("arial_start", "../res/arial.ttf", 36);
    assets->AddTexture("key", "../res/key.png");
    assets->AddTexture("door", "../res/door.png");
	SDL_Color white = {255, 255, 255, 255};
    SDL_Color red = {255,0,0,0};
    SDL_Color blue = {0,0,255,0};

	map = new Map("terrain",2,32);
	map->LoadMap("../res/map0.map", 25, 20);
    //map->LoadMap(mapLevel, 25, 20);

	//player.addComponent<TransformComponent>(4);
    player.addComponent<TransformComponent>(135,1100,86,64,1);
	player.addComponent<SpriteComponent>("player",true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
    player.addComponent<HealthComponent>(1);
	player.addGroup(groupPlayer);

	label.addComponent<UILabel>(10, 10, "Test String", "arial", white);
	label1.addComponent<UILabel>(25, 300, "Press any key to start the game s=save l=load", "arial_start", red);
    endGameLabel.addComponent<UILabel>(150, 250, "You died!", "arial_start", red);
    finishedGame.addComponent<UILabel>(250,250,"Game finished!","arial_start" , blue);

    exitEntity.addComponent<TransformComponent>(1092,1175);
    exitEntity.addComponent<ColliderComponent>("exit");


    gameState = START_MENU;
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayer));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& items(manager.getGroup(Game::groupItems));

std::ofstream saveFile;

void Game::handleEvents()
{

	SDL_PollEvent(&event);

	if(gameState == START_MENU && event.key.keysym.sym == SDLK_ESCAPE)
	{
		isRunning = false;
	}
    if(gameState == START_MENU && event.key.keysym.sym == SDLK_s)
    {
        save();
    }
    if(gameState == START_MENU && event.key.keysym.sym == SDLK_l)
    {
        load();
    }
	else
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYUP:
			if(gameState == START_MENU)
				gameState = PLAYING;
			break;
		default:
			break;
		}
	}
}

void Game::update() {

	switch(gameState)
	{
		case START_MENU:
			//start_menu();
            //label1.getComponent<UILabel>().SetLabelText("Press any key to start the game", "arial_start");
            //endGameLabel.getComponent<UILabel>().SetLabelText("You died!", "arial_start");
			break;

		case PLAYING:
            if(level>2)
            {
                gameState = START_MENU;
                return;
            }
            player.getComponent<HealthComponent>().hasDied = false;
			SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
			Vector2D playerPos = player.getComponent<TransformComponent>().position;

            lastEnemyPosition.clear();

            if(Collision::AABB(playerCol, exitEntity.getComponent<ColliderComponent>().collider))
            {
                nextLevel();
            }

            for(auto& e: enemies)
            {
                lastEnemyPosition.push_back(e->getComponent<TransformComponent>().position);
            }

			std::stringstream ss;
			ss << "Player position: "<< playerPos;
			label.getComponent<UILabel>().SetLabelText(ss.str(), "arial");

			manager.refresh();
			manager.update();

            for(auto e: enemies)
            {
                e->getComponent<EnemyComponent>().rangeCheck(playerPos);
                if (Collision::AABB(player.getComponent<ColliderComponent>().collider, e->getComponent<ColliderComponent>().collider))
                {
                    player.getComponent<HealthComponent>().hit();
                }
            }

			for(auto& c: colliders)
			{
				SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
				if(Collision::AABB(cCol, playerCol))
				{
					player.getComponent<TransformComponent>().position = playerPos;
				}
                unsigned int enemyIndex=0;
                for(auto& e: enemies)
                {
                    if(Collision::AABB(cCol, e->getComponent<ColliderComponent>().collider))
                    {
                        e->getComponent<TransformComponent>().position = lastEnemyPosition[enemyIndex];
                        //std::cout<<"Position on col: "<<e->getComponent<TransformComponent>().position
                        //<<" Last pos: "<<lastEnemyPosition[enemyIndex]<<std::endl;
                    }
                    enemyIndex++;
                }
			}

			for (auto& p : projectiles)
			{
                for(auto e: enemies)
                {
                    if (Collision::AABB(e->getComponent<ColliderComponent>(),
                                        p->getComponent<ColliderComponent>())) {
                        e->getComponent<HealthComponent>().hit();
                        p->destroy();
                    }
                }
			}

            for(auto&item:items)
            {
                if (Collision::AABB(playerCol,
                                    item->getComponent<ColliderComponent>().collider)) {
                    item->getComponent<InterractComponent>().access(gotItem);
                    player.getComponent<TransformComponent>().position = playerPos;
                    std::cout<<"omg"<<std::endl;
                }
            }

			camera.x = player.getComponent<TransformComponent>().position.x - 400;
			camera.y = player.getComponent<TransformComponent>().position.y - 320;

			if(camera.x < 0)
			{
				camera.x = 0;
			}
			if(camera.y < 0)
			{
				camera.y = 0;
			}
			if(camera.x > camera.w)
			{
				camera.x = camera.w;
			}
			if(camera.y > camera.h)
			{
				camera.y = camera.h;
			}
			break;
	}
}

void Game::render() {
	SDL_RenderClear(renderer);
	switch(gameState)
	{
		case START_MENU:
			start_menu();
            //label1.draw();
			break;
		case PLAYING:
			
			for(auto& t:tiles)
			{
				t->draw();
			}
			for(auto& c: colliders)
			{
				c->draw();
			}
			for(auto& t:players)
			{
				t->draw();
			}
			for (auto& p : projectiles)
			{
				p->draw();
			}
            for (auto& p : enemies)
            {
                p->draw();
            }
            for (auto& p : items)
            {
                p->draw();
            }

			label.draw();
			
			break;
	}
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned! " << std::endl;
}

void Game::start_menu()
{
    if(level>2)
    {
        finishedGame.draw();
    }

    if(player.getComponent<HealthComponent>().hasDied || player.getComponent<HealthComponent>().getHealth()<=0)
    {
        endGameLabel.draw();
        player.getComponent<HealthComponent>().setHealth(1);
        player.getComponent<HealthComponent>().hasDied = true;
        player.getComponent<TransformComponent>().position = Vector2D{135,1100};
    }

    // endGameLabel.draw();
    label1.draw();
}

void Game::nextLevel()
{
    gotItem = false;

    std::string mapPath ="../res/map0.map";
    mapPath[10] += level;

    level++;

    player.getComponent<TransformComponent>().position = Vector2D{135,1100};

    for(auto& c: colliders)
    {
        c->destroy();
    }

    delete map;
    map = new Map("terrain",2,32);

    map->LoadMap(mapPath, 25, 20);
}

void Game::save()
{
    saveFile.open("../saveFile.save");

    if(!saveFile.is_open())
    {
        std::cout<<"Error opening save file!";
        return;
    }
    std::cout<<"Saving game..."<<std::endl;

    std::stringstream ss;
    ss << player.getComponent<TransformComponent>().position<<" "<<gotItem<<" "<<level;

    saveFile<<ss.str();

    saveFile.close();
}

void Game::load()
{
    std::fstream saveFile;

    saveFile.open("../saveFile.save");

    if(!saveFile.is_open())
    {
        std::cout<<"Error opening save file!";
        return;
    }

    int data;
    saveFile.ignore();

    saveFile>>data;
    player.getComponent<TransformComponent>().position.x = data;

    saveFile.ignore();

    saveFile>>data;
    player.getComponent<TransformComponent>().position.y = data;

    saveFile.ignore();
    saveFile>>data;
    gotItem = data;

    saveFile.ignore();
    saveFile>>data;
    level = data;

    saveFile.close();

    if(map)
    {
        delete map;
        for(auto& c: colliders)
        {
            c->destroy();
        }
    }
    map = new Map("terrain",2,32);

    std::string mapPath ="../res/map0.map";
    mapPath[10] += level;

    map->LoadMap(mapPath, 25, 20);
}