#include "screenobject.h"
#include "graphics.h"
#include "window.h"
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

ScreenObject::ScreenObject(): name("")
{
}

bool ScreenObject::Init()
{

}

bool ScreenObject::Update(float timestep)
{
}

void ScreenObject::Draw()
{
 	Graphics::GetGraphics()->DrawArea(area,true);
}

bool ScreenObject::ToJson(rapidjson::Value& val, rapidjson::Document& doc)
{
	val.AddMember("type", "screenobject", doc.GetAllocator());

	val.AddMember("name", name.c_str(), doc.GetAllocator());

	rapidjson::Value valArea(rapidjson::kObjectType);
	if (!area.ToJson(valArea, doc))
	{
		return false;
	}
	val.AddMember("area", valArea, doc.GetAllocator());

	return true;
}

bool ScreenObject::FromJson(const rapidjson::Value& val)
{
	if (std::string("screenobject").compare(val["type"].GetString())!=0)
	{
		return false;
	}

	name = val["name"].GetString();

	const rapidjson::Value& valArea = val["area"];
	if (!area.FromJson(valArea))
	{
		printf("Unable to load Area\n");
		return false;
	}
	return true;
}
