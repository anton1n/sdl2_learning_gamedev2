#include "Map.hpp"
#include "Game.hpp"
#include <fstream>
#include "ECS.hpp"
#include "Components.hpp"

Map::Map(std::string tID, int ms, int ts) : texID(tID), mapScale(ms), tileSize(ts)
{
	scaledSize = ms*ts;
}

Map::~Map() {
	
}


extern Manager manager;


void Map::LoadMap(std::string path, int sizex, int sizey)
{
	char c;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;

	for (int y = 0; y < sizey; y++)
	{
		for (int x = 0; x < sizex; x++)
		{
			
			mapFile.get(c);

			if (c == 10 || c == 13) {
				mapFile.get(c);
				if (c == 10 || c == 13) {
					mapFile.get(c);
				}
			}

			srcY = atoi(&c) * tileSize;
			mapFile.get(c);

			if (c == 10 || c == 13) {
				mapFile.get(c);
				if (c == 10 || c == 13) {
					mapFile.get(c);
				}
			}

			srcX = atoi(&c) * tileSize;
			AddTile(srcX, srcY, x*scaledSize, y*scaledSize);
			mapFile.ignore();
		}
	}

	mapFile.ignore();
	//std::cout<<(int)c<<std::endl;
	for (int y = 0; y < sizey; y++)
	{
		for (int x = 0; x < sizex; x++)
		{
			mapFile.get(c);
			if (c == 10 || c == 13 || c ==',') {
				mapFile.get(c);
				if (c == 10 || c == 13 || c == ',') {
					mapFile.get(c);
				}
			}
			//std::cout<<(int)c<<std::endl;
			if (c == 49)
			{
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				tcol.addGroup(Game::groupColliders);
			}
			mapFile.ignore();
		}
	}

	mapFile.close();
}


void Map::AddTile(int srX, int srY, int xpos, int ypos)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srX, srY, xpos, ypos, tileSize, mapScale, texID);
	tile.addGroup(Game::groupMap);
}
