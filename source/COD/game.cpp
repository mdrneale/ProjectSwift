#include "game.h"
#include "graphics.h"
#include "window.h"
#include "statemanager.h"
#include "mainmenu.h"

#include "iohelper.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

Game::Game()
{
	Load("assets/levels/levels.json");
	currentlevel = 0;
	LoadLevel(currentlevel);
}

void Game::Load(const char * filename)
{
    std::string contents = LoadFileAsString(filename);
    printf("%s\n", contents.c_str());
    rapidjson::Document doc;
    if (doc.Parse<0>(contents.c_str()).HasParseError())
    {
    	printf("Couldn't pass json string.\n");
    	return;
    }

	if (std::string("levels").compare(doc["type"].GetString())!=0)
	{
    	printf("json not of type: levels.\n");
		return;
	}
	const rapidjson::Value& valLevels = doc["levels"];
	for (rapidjson::SizeType i = 0; i < valLevels.Size(); i++)
	{
		levelfiles.push_back(valLevels[i].GetString());
	}
}

void Game::Save(const char * filename)
{
	rapidjson::Document doc;
	doc.SetObject();

	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	rapidjson::Value valLevels(rapidjson::kArrayType);
	for (int i = 0; i < levelfiles.size(); i++)
	{
		valLevels.PushBack(levelfiles[i].c_str() , allocator);
	}

	doc.AddMember("levels", valLevels, 	doc.GetAllocator());
	doc.AddMember("type", 	"levels", 	doc.GetAllocator());

	rapidjson::GenericStringBuffer< rapidjson::UTF8<> > buffer;
	rapidjson::Writer< rapidjson::GenericStringBuffer< rapidjson::UTF8<> > > writer(buffer);
	doc.Accept(writer);

	const char* str = buffer.GetString();
	std::string ret = str;

    std::ofstream out(filename);
    out << str;
	printf("%s\n", str);
}

void Game::LoadLevel(int levelnumber)
{
	if (levelnumber < 0)
	{
		return;
	}

	if (levelnumber >= levelfiles.size())
	{
		level.Reset();
		currentlevel = levelfiles.size();
		return;
	}
	level.Load(levelfiles[levelnumber].c_str());
	currentlevel = levelnumber;
	level.Zoom(level.height >= level.width ? level.height + 1 : (level.width/1.44f) + 1);
	level.SetOffset(level.width / 2, level.height / 2);
}

void Game::SaveLevel()
{
	std::string filename = "assets/levels/level" + NtoS(currentlevel) + ".json";
	if (std::find(levelfiles.begin(), levelfiles.end(), filename) == levelfiles.end())
	{
		levelfiles.push_back(filename);
		return;
	}
	level.Save(filename.c_str());
	Save("assets/levels/levels.json");
}

void Game::Draw()
{
	level.Draw();
}

void Game::Draw(Geometry::Square area)
{
	level.Draw(area);
}

void Game::DrawWithoutPlayer()
{
	level.DrawWithoutPlayer();
}
