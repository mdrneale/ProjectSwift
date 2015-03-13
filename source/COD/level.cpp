#include "level.h"
#include "graphics.h"
#include "window.h"

#include "iohelper.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

Level::Level():tiles("assets/tiles.png"), data(NULL), width(0), height(0)
{
	mapZoom 	= 1;
	mapXoffset 	= 0;
	mapYoffset 	= 0;

	tilesize 	= 0;

	// temp
	width 	= 16;
	height = 16;
	Reset();

}

void Level::Load(const char * filename)
{
	mapZoom 	= 1;
	mapXoffset 	= 0;
	mapYoffset 	= 0;

    std::string contents = LoadFileAsString(filename);
    printf("%s\n", contents.c_str());
    rapidjson::Document doc;
    if (doc.Parse<0>(contents.c_str()).HasParseError())
    {
    	printf("Couldn't pass json string.\n");
    	return;
    }

	if (std::string("level").compare(doc["type"].GetString())!=0)
	{
    	printf("json not of type: Level.\n");
		return;
	}
	const rapidjson::Value& valLevel = doc["level"];
	width 	= valLevel["width"].GetInt();
	height 	= valLevel["height"].GetInt();
	Reset();

	const rapidjson::Value& valdata = valLevel["data"];
	for (rapidjson::SizeType i = 0; i < valdata.Size(); i++)
	{
		data[i] = valdata[i].GetInt();
	}
}

void Level::LoadTileTexture(const char * filename)
{
	tiles = TileResource(filename);
}


void Level::Save(const char * filename)
{
	rapidjson::Document doc;
	doc.SetObject();

	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	rapidjson::Value valLevel(rapidjson::kObjectType);

	valLevel.AddMember("width"	, width							, doc.GetAllocator());
	valLevel.AddMember("height"	, height						, doc.GetAllocator());
	valLevel.AddMember("tileset", tiles.textureFilename.c_str()	, doc.GetAllocator());

	rapidjson::Value valData(rapidjson::kArrayType);
	for (int i = 0; i < width * height; i++)
	{
		valData.PushBack(data[i] , allocator);
	}
	valLevel.AddMember("data", valData, doc.GetAllocator());

	doc.AddMember("level", 	valLevel, 	doc.GetAllocator());
	doc.AddMember("type", 	"level", 	doc.GetAllocator());

	rapidjson::GenericStringBuffer< rapidjson::UTF8<> > buffer;
	rapidjson::Writer< rapidjson::GenericStringBuffer< rapidjson::UTF8<> > > writer(buffer);
	doc.Accept(writer);

	const char* str = buffer.GetString();
	std::string ret = str;

    std::ofstream out(filename);
    out << str;
	printf("%s\n", str);
}


void Level::Reset()
{
	if (data)
	{
		delete data;
		data = NULL;
	}
	data = new int[width*height];
	for (int i = 0; i < width * height; i++)
	{
		data[i] = 7;
	}	
}

bool Level::SetTile(int x, int y, int tileID)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
	{
		return false;
	}

	data[x+(y*width)] = tileID;
	return true;
}

int Level::GetTile(int x, int y)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
	{
		return -1;
	}

	return data[x + ( y * width)];
}

void Level::GetTilePositionFromScreenCoords(float x, float y, int &lx, int &ly, float sx, float sy, float ex, float ey)
{
	float halftilesize = tilesize / 2.0f;

	float midx = (ex-sx) * 0.5f;
	float xpnt = (x - sx) + halftilesize;
	float flx = ((xpnt - midx) / tilesize) + mapXoffset;
	lx = (flx>0)?flx:flx-1;

	float midy = (ey - sy) * 0.5f;
	float ypnt = ((ey - sy) - (y - sy)) + halftilesize;
	float fly = ((ypnt - midy) / tilesize) + mapYoffset;
	ly = (fly>0)?fly:fly-1;
}

void Level::GetTilePositionFromScreenCoords(float x, float y, int &lx, int &ly, const Geometry::Square & area)
{
	GetTilePositionFromScreenCoords(x, y, lx, ly, area.left, area.bottom, area.right, area.top);
}

Geometry::Square & Level::GetTileScreenPosition(int x, int y, Geometry::Square & area)
{
	area.left 	= 0;
	area.right 	= 0;
	area.top 	= 0;
	area.bottom = 0;

	float fwidth = width;
	float fHeight = height;

	tilesize = (1.0f / mapZoom);
	float halftilesize = tilesize / 2.0f;

	float ypos = ((0.5f-(y * tilesize)) - (-mapYoffset * tilesize) - tilesize) + halftilesize;
	if (ypos < 0 - tilesize || ypos > 1.0f + tilesize)
	{
		return area;
	}
	float xpos = ((0.5f*1.3f) + (x * tilesize)) - (mapXoffset * tilesize) - halftilesize;
	if (xpos < 0 - tilesize || xpos > 1.0f)
	{
		return area;
	}

	area.left 	= xpos;
	area.right 	= tilesize;
	area.top 	= ypos;
	area.bottom = tilesize;

	return area;
}

void Level::Move(int x, int y)
{
	mapXoffset += x;
	mapYoffset += y;
}

void Level::SetOffset(int x, int y)
{
	mapYoffset = y;
	mapXoffset = x;
}

void Level::Zoom(float zoom)
{
	mapZoom *= zoom;
}

void Level::Draw(float sx, float sy, float ex, float ey)
{
	float fwidth = width;
	float fHeight = height;

	tilesize = ((ey-sy) / mapZoom);
	float halftilesize = tilesize / 2;

	for (int y = 0; y < height; y++)
	{
		float txOffset = 0, tyOffset = 0;
		float tx = 0, ty = 0, tw = 1.0f, th = 1.0f;

		float ypos = ((0.5-(y * tilesize)) - (-mapYoffset * tilesize) - tilesize) + ((1.0f-(ey-sy))*0.5) + halftilesize;
		if (ypos < sy - tilesize || ypos > ey + tilesize)
		{
			continue;
		}
		if (ypos < sy)
		{
			tyOffset = (sy - ypos);
			th = 1 - ((sy - ypos) / tilesize);
			ty = 1 - th;
		}
		for (int x = 0; x < width; x++)
		{
			float xpos = (x * tilesize) - (mapXoffset * tilesize) + ((ex-sx) * 0.5f) - halftilesize;
			if (xpos < sx - tilesize || xpos > ex)
			{
				continue;
			}
			if (xpos > ex - tilesize)
			{
				tw = ((ex - xpos) / tilesize);
			}
			int tile = data[x+(y*width)];
			tiles.Draw(tile, xpos + txOffset, ypos + tyOffset, tilesize * tw, tilesize * th, tx, ty, tw, th);
		}
	}
}

void Level::Draw(Geometry::Square area)
{
	Draw(area.left, area.bottom, area.right, area.top);
}

void Level::DrawWithoutPlayer()
{
	float sx=0;
	float sy=0;
	float ex=1.3;
	float ey=1.0;

	float fwidth = width;
	float fHeight = height;

	tilesize = ((ey-sy) / mapZoom);
	float halftilesize = tilesize / 2;

	for (int y = 0; y < height; y++)
	{
		float txOffset = 0, tyOffset = 0;
		float tx = 0, ty = 0, tw = 1.0f, th = 1.0f;

		float ypos = ((0.5-(y * tilesize)) - (-mapYoffset * tilesize) - tilesize) + ((1.0f-(ey-sy))*0.5) + halftilesize;
		if (ypos < sy - tilesize || ypos > ey + tilesize)
		{
			continue;
		}
		if (ypos < sy)
		{
			tyOffset = (sy - ypos);
			th = 1 - ((sy - ypos) / tilesize);
			ty = 1 - th;
		}
		for (int x = 0; x < width; x++)
		{
			float xpos = (x * tilesize) - (mapXoffset * tilesize) + ((ex-sx) * 0.5f) - halftilesize;
			if (xpos < sx - tilesize || xpos > ex)
			{
				continue;
			}
			if (xpos > ex - tilesize)
			{
				tw = ((ex - xpos) / tilesize);
			}
			int tile = data[x+(y*width)];
			if (tile != TileResource::PLAYER)
			{
				tiles.Draw(tile, xpos + txOffset, ypos + tyOffset, tilesize * tw, tilesize * th, tx, ty, tw, th);
			}
		}
	}
}