#include "highscore.h"
#include <string>

#include "iohelper.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

HighScore::HighScore()
{
	Load("assets/levels/scores.json");
}

void HighScore::Load(const char * filename)
{
    std::string contents = LoadFileAsString(filename);
    printf("%s\n", contents.c_str());
    rapidjson::Document doc;
    if (doc.Parse<0>(contents.c_str()).HasParseError())
    {
    	printf("Couldn't pass json string.\n");
    	return;
    }

	if (std::string("highscores").compare(doc["type"].GetString())!=0)
	{
    	printf("json not of type: levels.\n");
		return;
	}
	const rapidjson::Value& valScores = doc["highscores"];
	for (rapidjson::SizeType i = 0; i < valScores.Size(); i++)
	{
		scores.push_back(Score(valScores[i]["time"].GetDouble(),valScores[i]["bonus"].GetInt()));
	}

}

void HighScore::Save(const char * filename)
{
	rapidjson::Document doc;
	doc.SetObject();

	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	rapidjson::Value valScores(rapidjson::kArrayType);
	for (int i = 0; i < scores.size(); i++)
	{
		rapidjson::Value valScore(rapidjson::kObjectType);

		valScore.AddMember("time", scores[i].time, allocator);
		valScore.AddMember("bonus", scores[i].bonus, allocator);
		valScores.PushBack(valScore , allocator);
	}

	doc.AddMember("highscores", valScores, 	doc.GetAllocator());
	doc.AddMember("type", 	"highscores", 	doc.GetAllocator());

	rapidjson::GenericStringBuffer< rapidjson::UTF8<> > buffer;
	rapidjson::Writer< rapidjson::GenericStringBuffer< rapidjson::UTF8<> > > writer(buffer);
	doc.Accept(writer);

	const char* str = buffer.GetString();
	std::string ret = str;

    std::ofstream out(filename);
    out << str;
	printf("%s\n", str);
}

bool HighScore::IsHighScore(int levelnumber, Score score)
{
	if (levelnumber==scores.size())
	{
		scores.push_back(score);
		Save("assets/levels/scores.json");
		return true;
	}

	if (score.bonus < scores[levelnumber].bonus)
	{
		scores[levelnumber] = score;
		Save("assets/levels/scores.json");
		return true;
	}
	
	if (score.time < scores[levelnumber].time)
	{
		scores[levelnumber] = score;
		Save("assets/levels/scores.json");
		return true;
	}
	return false;
}