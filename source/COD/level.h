#ifndef CODLEVEL
#define CODLEVEL

#include "tileresource.h"
#include "square.h"

class Level
{
public:

	Level();

	void Load(const char * filename);
	void Save(const char * filename);

	void Draw(float sx=0, float sy=0, float ex=1.3, float ey=1.0);
	void Draw(Geometry::Square area);
	void DrawWithoutPlayer();

	void Reset();
	bool SetTile(int x, int y, int tileID);
	int  GetTile(int x, int y);
	Geometry::Square & GetTileScreenPosition(int x, int y, Geometry::Square & area);

	void Move(int x, int y);
	void SetOffset(int x, int y);
	void Zoom(float zoom);
	void GetTilePositionFromScreenCoords(float x, float y, int &lx, int &ly, float sx=0, float sy=0, float ex=1.3, float ey=1.0);
	void GetTilePositionFromScreenCoords(float x, float y, int &lx, int &ly, const Geometry::Square & area);

	void LoadTileTexture(const char * filename);

	TileResource tiles;

	int 	width;
	int 	height;
	int 	*data;

	float 	tilesize;
	float	mapZoom;
	int		mapXoffset;
	int		mapYoffset;

};

#endif