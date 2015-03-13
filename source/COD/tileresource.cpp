#include "tileresource.h"

TileResource::TileResource()
{
	textureFilename = "";
}

TileResource::~TileResource()
{
}

TileResource::TileResource(const char * filename)
{
	textureFilename = filename;
	tileTexture.SetTexture(filename);
	int xtiles = 6;
	int ytiles = 5;

	float xtilesize = 1.0f / 6.0f;
	float ytilesize = 1.0f / 5.0f;

	for (int i = 0; i < 25; i++)
	{
		squares[i].left 	= (i % xtiles) / (float)xtiles;
		squares[i].top 		= (i / xtiles) / (float)ytiles;
		squares[i].right 	= xtilesize;
		squares[i].bottom 	= ytilesize;
	}
}

void TileResource::Draw(const int tile, float x, float y, float w, float h, float sx, float sy, float sw, float sh, float rotation, bool flip)
{
	if (tile < 0 || tile >= 25 || textureFilename == "")
	{
		return;
	}

	float tilewidth 	= squares[tile].right;
	float tileheight	= squares[tile].bottom;
	tileTexture.RenderTexture(  x, 
								y, 
								w, 
								h,
								rotation, 
								squares[tile].left 	+ (sx * tilewidth), 
								squares[tile].top 	+ (sy * tileheight), 
								(sw * tilewidth), 
								(sh * tileheight),
								flip);
}
