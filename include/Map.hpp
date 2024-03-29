#pragma once
#include <string>

class Map {
public:
	Map(std::string tID, int ms, int ts);
	~Map();
	
	void AddTile(int srX, int srY, int xpos, int ypos);
	void LoadMap(std::string path, int sizex, int sizey);

private:
	std::string texID;
	int mapScale;
	int tileSize;	
	int scaledSize;
};