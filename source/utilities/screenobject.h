#ifndef SCREENOBJECT
#define SCREENOBJECT

#include "area.h"
#include <string>
#include "jsonobject.h"

class ScreenObject: public JsonObject
{
public:
	ScreenObject();

	virtual bool Init();
	virtual bool Update(float timestep=0.02);
	virtual void Draw();

	virtual bool ToJson(rapidjson::Value& val, rapidjson::Document& doc);
	virtual bool FromJson(const rapidjson::Value& val);

	Geometry::Area area;
	std::string name;
};

#endif