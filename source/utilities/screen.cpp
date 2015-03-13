#include "screen.h"
#include "Window.h"
#include "iohelper.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

Screen::Screen(): editting(false)
{
}

Screen::Screen(const char * filename):editting(false)
{
    std::string contents = LoadFileAsString(filename);
    printf("%s\n", contents.c_str());
    rapidjson::Document doc;
    if (doc.Parse<0>(contents.c_str()).HasParseError())
    {
    	printf("Couldn't pass json string.\n");
    }
    else
    {
    	if (!FromJson(doc["base"]))
    	{
	    	printf("Couldn't create screen from json.\n");    		
    	}
    }
}

const ScreenObject * Screen::GetObject(const char * objName)
{
	for (int i = 0; i < objects.size(); ++i)
	{
		if (objects[i].name.compare(objName) == 0)
		{
			return &objects[i];
		}
	}

	return NULL;
}

void Screen::SetClickResponder(ClickResponder* responder) { clickResponder = responder; }

void Screen::UpdateObjects(float timestep)
{
	for (int i = 0; i < objects.size(); ++i)
	{
		objects[i].Update(timestep);
		if (objects[i].area.Intersecting(Window::mouse.Circle()))
		{
			if (Window::mouse.leftClicker == Mouse::CLICKED)
			{
				if (editting)
				{
					std::string newname;
					std::cin >> newname;
					objects[i].name = newname;
				}
				clickResponder->Clicked(objects[i], ClickResponder::LEFT);
			}
			if (Window::mouse.leftClicker == Mouse::HELD)
			{
				clickResponder->Hovered(objects[i], ClickResponder::LEFT);
			}
			if (Window::mouse.leftClicker == Mouse::RELEASED)
			{
				clickResponder->Released(objects[i], ClickResponder::LEFT);
			}
			if (Window::mouse.rightClicker == Mouse::CLICKED)
			{
				if (editting)
				{
					std::string newname;
					std::cin >> newname;
					objects[i].name = newname;
				}
				clickResponder->Clicked(objects[i], ClickResponder::RIGHT);
			}
			if (Window::mouse.rightClicker == Mouse::HELD)
			{
				clickResponder->Hovered(objects[i], ClickResponder::RIGHT);
			}
			if (Window::mouse.rightClicker == Mouse::RELEASED)
			{
				clickResponder->Released(objects[i], ClickResponder::RIGHT);
			}
			return;
		}
	}
}

void Screen::SaveToJson(const char * filename)
{
	rapidjson::Document doc;
	doc.SetObject();

	rapidjson::Value base(rapidjson::kObjectType);

	if (ToJson(base, doc))
	{
		doc.AddMember("base",base, doc.GetAllocator());

		rapidjson::GenericStringBuffer< rapidjson::UTF8<> > buffer;
		rapidjson::Writer< rapidjson::GenericStringBuffer< rapidjson::UTF8<> > > writer(buffer);
		doc.Accept(writer);

		const char* str = buffer.GetString();
		std::string ret = str;

	    std::ofstream out(filename);
	    out << str;
		printf("%s\n", str);
	}
}

void Screen::CheckInput()
{
	if (Window::keyboard.keys['e']==Keyboard::DOWN)
	{
		editting = !editting;
	}

	if (editting)
	{
		if (Window::mouse.leftClicker == Mouse::CLICKED)
		{
			temp.area.points.push_back(new Geometry::Point(Window::mouse.currentPosition.x,Window::mouse.currentPosition.y));
			temp.area.CalculateSDLPoints();
			temp.area.CalculateBoundingBox();
		}

		if (Window::mouse.rightClicker == Mouse::CLICKED)
		{
			if (temp.area.points.size()>0)
			{
				temp.area.points.pop_back();
				temp.area.CalculateSDLPoints();
				temp.area.CalculateBoundingBox();
			}
		}

		if (Window::keyboard.keys[' ']==Keyboard::DOWN)
		{
			objects.push_back(temp);
			temp = ScreenObject();
		}
		if (Window::keyboard.keys['s']==Keyboard::DOWN)
		{
			SaveToJson("test1.json");
		}
	}
}

bool Screen::Update(float timestep)
{
	UpdateObjects(timestep);
	CheckInput();
	return true;
}

bool Screen::ToJson(rapidjson::Value& val, rapidjson::Document& doc)
{
	val.AddMember("type", "screen", doc.GetAllocator());

	rapidjson::Value valObjects(rapidjson::kArrayType);

	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	for (int i = 0; i < objects.size(); i++)
	{
		rapidjson::Value valObject(rapidjson::kObjectType);
		objects[i].ToJson(valObject,doc);

		valObjects.PushBack(valObject , allocator);
	}

	val.AddMember("objects", valObjects, doc.GetAllocator());

	return true;
}

bool Screen::FromJson(const rapidjson::Value& val)
{
	if (std::string("screen").compare(val["type"].GetString())!=0)
	{
		return false;
	}
	const rapidjson::Value& valObjects = val["objects"];
	for (rapidjson::SizeType i = 0; i < valObjects.Size(); i++)
	{
		ScreenObject t;
		const rapidjson::Value& valObject = valObjects[i];
		if (t.FromJson(valObject))
		{
			objects.push_back(t);
		}
		else
		{
			printf("Unable to load ScreenObject: %i\n", i);
		}
	}
	return true;
}

void Screen::Draw()
{
	if (editting)
	{
		for (int i = 0; i < objects.size(); ++i)
		{
			objects[i].Draw();
		}
		temp.Draw();
	}
}
