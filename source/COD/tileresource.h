#ifndef CODTILERESOURCE
#define CODTILERESOURCE

#include "texture.h"
#include "square.h"
#include <string>

class TileResource
{
public:
	TileResource();
	TileResource(const char * filename);
	virtual ~TileResource();

	void Draw(const int tile, float x, float y, float w, float h, float sx=0, float sy=0, float sw=1.0f, float sh=1.0f, float rotation=0, bool flip=false);

	Geometry::Square squares[25];
	Texture tileTexture;

	std::string textureFilename;

	enum TileType 
	{
		EMPTY 		= -1,
		LEFTWALL	= 9,
		RIGHTWALL	= 10,
		SWEET 		= 18,
		BONUS 		= 19,
		ROCK 		= 20,
		DEATH		= 21,
		PLAYER 		= 24
	};
};

#endif