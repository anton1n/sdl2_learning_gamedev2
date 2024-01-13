#pragma once
#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <iostream>
#include "TextureManager.hpp"
#include <vector>
#include "AssetManager.hpp"


class AssetManager;
class ColliderComponent;



	enum GameState 
	{
	    START_MENU,
	    PLAYING
	};

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int x, int y, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();
	bool running() { return isRunning; }
	void start_menu();


    /////////////
 ////*omfg omfg*////
    ////////////

    void nextLevel();
    void save();
    void load();

	static SDL_Renderer* renderer;
	static SDL_Event event;
	//static std::vector<ColliderComponent*> colliders;
	static bool isRunning;
	static SDL_Rect camera;
	static AssetManager* assets;

	static GameState gameState;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayer,
		groupEnemies,
		groupColliders,
		groupProjectiles,
        groupItems
	};




private:
	int cnt=0;
	SDL_Window* window=nullptr;
	
};