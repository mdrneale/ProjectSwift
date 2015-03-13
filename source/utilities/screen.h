#ifndef SCREEN
#define SCREEN

#include <vector>
#include "screenobject.h"
#include "clickresponder.h"
#include "jsonobject.h"

class Screen: public JsonObject
{
public:
	Screen();
	Screen(const char * filename);

	virtual bool Update(float timestep=0.02);
	virtual void Draw();

	virtual bool ToJson(rapidjson::Value& val, rapidjson::Document& doc);
	virtual bool FromJson(const rapidjson::Value& val);

	virtual void SetClickResponder(ClickResponder* responder);

	virtual void UpdateObjects(float timestep);
	virtual void SaveToJson(const char * filename);
	virtual void CheckInput();

	virtual const ScreenObject* GetObject(const char * objName);

	ClickResponder * clickResponder;
	std::vector<ScreenObject> objects;
	ScreenObject temp;

	bool editting;
};

#endif