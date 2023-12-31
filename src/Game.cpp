#include "Game.hpp"
//#include "GameObject.hpp"
#include "Map.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include <sstream>

//SDL_Texture* playerTex;
//SDL_Rect srcR, destR;

SDL_Renderer* Game::renderer = nullptr;
Map* map;

Manager manager;

SDL_Event Game::event;

SDL_Rect Game::camera = {0,0,800,640};

AssetManager* Game::assets = new AssetManager(&manager);

//std::vector<ColliderComponent*> Game::colliders;

auto& player(manager.addEntity());
auto& label(manager.addEntity());

bool Game::isRunning = false;


//auto& enemies(manager.getGroup(groupEnemies));

//auto& tile0(manager.addEntity());
//auto& tile1(manager.addEntity());
//auto& tile2(manager.addEntity());

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

	//playerTex = TextureManager::LoadTexture("Assets/player.png", renderer);
	//player = new GameObject("Assets/simple_knight.png", 0, 0);
	//enemy = new GameObject("Assets/player1.png", 50, 50);

	//map = new Map("res/terrain_ss.png",3,32);

	if(TTF_Init() == -1)
	{
		std::cout<<"Error: SDL_TTF"<<std::endl;
	}

	assets->AddTexture("terrain", "res/terrain_ss.png");
	assets->AddTexture("player", "res/player_anims.png");
	assets->AddTexture("projectile", "res/proj.png");
	assets->AddFont("arial", "res/arial.ttf", 16);	


	map = new Map("terrain",3,32);
	map->LoadMap("res/map.map", 25, 20);
	
	player.addComponent<TransformComponent>(4);
	player.addComponent<SpriteComponent>("player",true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayer);
	
	SDL_Color white = {255, 255, 255, 255};
	label.addComponent<UILabel>(10, 10, "Test String", "arial", white);

	assets->CreateProjectile(Vector2D(600, 600), Vector2D(2,0),200, 2, "projectile");

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayer));
auto& colliders(manager.getGroup(Game::groupColliders));
//auto& enemies(manager.getGroup(Game::groupEnemies));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

void Game::handleEvents()
{

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}
}

void Game::update() {

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	std::stringstream ss;
	ss << "Player position: "<< playerPos;
	label.getComponent<UILabel>().SetLabelText(ss.str(), "arial");

	manager.refresh();
	manager.update();

	for(auto& c: colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if(Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	
	for (auto& p : projectiles)
	{
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
		{
			std::cout << "Hit player!" << std::endl;
			p->destroy();
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

}


void Game::render() {
	SDL_RenderClear(renderer);

	//manager.draw();
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

	label.draw();
	SDL_RenderPresent(renderer);

}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned! " << std::endl;
}
