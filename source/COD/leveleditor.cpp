#include "leveleditor.h"
#include "graphics.h"
#include "window.h"
#include "statemanager.h"
#include "mainmenu.h"

LevelEditorState::LevelEditorState()
{
	BGImage.SetTexture("assets/views/editorbg.png");

	Init();
}

void LevelEditorState::Load(const char * filename)
{

}

void LevelEditorState::Save(const char * filename)
{
}

bool LevelEditorState::Init()
{
	screen = Screen("assets/views/editor.json");
	screen.SetClickResponder(this);
	Graphics::GetGraphics()->SetDrawColour(0.5,0.5,1);
	currenttile = 0;
}

void LevelEditorState::Clicked(ScreenObject & ob, Button button)
{
	if (ob.name=="selecttile")
	{
		float x = (Window::mouse.currentPosition.x - ob.area.boundingbox.left) / (ob.area.boundingbox.right - ob.area.boundingbox.left);
		float y = (Window::mouse.currentPosition.y - ob.area.boundingbox.bottom) / (ob.area.boundingbox.top - ob.area.boundingbox.bottom);
		int xcoord = (int) (x * 5);
		int ycoord = (int) (y * 5);		
		currenttile = (ycoord * 5) + xcoord;
		printf("%i\n", currenttile);
		printf(	"clicked on %s! %f, %i\n", 
				ob.name.c_str(), 
				y, 
				ycoord);
	}
	else if (ob.name=="back")
	{
		StateManager::GetStateManager()->SwitchState( MainMenuState::GetMainMenu() );
	}
	printf("clicked on %s! %f, %f\n", ob.name.c_str(), Window::mouse.currentPosition.x, Window::mouse.currentPosition.y);
}

void LevelEditorState::Released(ScreenObject & ob, Button button)
{

}

void LevelEditorState::Hovered(ScreenObject & ob, Button button)
{
	if (ob.name == "editingarea")
	{
		int xtile, ytile;
		game.level.GetTilePositionFromScreenCoords(Window::mouse.currentPosition.x, Window::mouse.currentPosition.y, xtile, ytile, ob.area.boundingbox);

		printf("tile x: %i tile y: %i\n", xtile, ytile);

		if (button == ClickResponder::LEFT)
		{
			if (!game.level.SetTile(xtile, ytile, currenttile))
			{
				if (xtile >= game.level.width)
				{
					printf("add to width!\n");
					Level newlevel;
					newlevel.width = game.level.width + 1;
					newlevel.height = game.level.height;
					newlevel.Reset();
					for (int i = 0; i < game.level.width * game.level.height; i++)
					{
						newlevel.SetTile(i % game.level.width, i / game.level.width, game.level.data[i]);
					}
					delete []game.level.data;
					game.level.data = newlevel.data;
					game.level.width= newlevel.width;
				}
				if (ytile >= game.level.height)
				{
					Level newlevel;
					newlevel.width = game.level.width;
					newlevel.height = game.level.height + 1;
					newlevel.Reset();
					for (int i = 0; i < game.level.width * game.level.height; i++)
					{
						newlevel.SetTile(i % game.level.width, i / game.level.width, game.level.data[i]);
					}
					delete []game.level.data;
					game.level.data		= newlevel.data;
					game.level.height	= newlevel.height;
				}
				if (xtile < 0)
				{
					printf("add to width!\n");
					Level newlevel;
					newlevel.width = game.level.width + 1;
					newlevel.height = game.level.height;
					newlevel.Reset();
					for (int i = 0; i < game.level.width * game.level.height; i++)
					{
						newlevel.SetTile(1 + (i % game.level.width), i / game.level.width, game.level.data[i]);
					}
					delete []game.level.data;
					game.level.data = newlevel.data;
					game.level.width= newlevel.width;
					game.level.Move(1,0);
				}
				if (ytile < 0)
				{
					Level newlevel;
					newlevel.width = game.level.width;
					newlevel.height = game.level.height + 1;
					newlevel.Reset();
					for (int i = 0; i < game.level.width * game.level.height; i++)
					{
						newlevel.SetTile(i % game.level.width, 1 + (i / game.level.width), game.level.data[i]);
					}
					delete []game.level.data;
					game.level.data		= newlevel.data;
					game.level.height	= newlevel.height;
					game.level.Move(0,1);
				}
			}
		}
		else
		{
			game.level.SetTile(xtile, ytile, -1);
		}
	}
	printf("hovered on %s! %f, %f\n", ob.name.c_str(), Window::mouse.currentPosition.x, Window::mouse.currentPosition.y);
}

void LevelEditorState::UnHovered(ScreenObject & ob, Button button)
{

}

bool LevelEditorState::Update(float timestep)
{
	if (Window::keyboard.keys['[']==Keyboard::DOWN)
	{
		game.level.Zoom(0.9f);
	}
	else if (Window::keyboard.keys[']']==Keyboard::DOWN)
	{
		game.level.Zoom(1.1f);
	}
	else if (Window::keyboard.keys[Keyboard::KUP]==Keyboard::DOWN)
	{
		game.level.Move(0, -1);
	}
	else if (Window::keyboard.keys[Keyboard::KDOWN]==Keyboard::DOWN)
	{
		game.level.Move(0, 1);
	}
	else if (Window::keyboard.keys[Keyboard::KLEFT]==Keyboard::DOWN)
	{
		game.level.Move(1, 0);
	}
	else if (Window::keyboard.keys[Keyboard::KRIGHT]==Keyboard::DOWN)
	{
		game.level.Move(-1, 0);
	}
	else if (Window::keyboard.keys['1']==Keyboard::DOWN)
	{
		game.LoadLevel(game.currentlevel-1);
	}
	else if (Window::keyboard.keys['2']==Keyboard::DOWN)
	{
		game.LoadLevel(game.currentlevel+1);
	}
	else if (Window::keyboard.keys['s']==Keyboard::DOWN)
	{
		game.SaveLevel();
	}
	screen.Update(timestep);
	return true;
}

void LevelEditorState::Draw()
{
	BGImage.RenderTexture(0,0,1.422,1);
	const ScreenObject * obj = screen.GetObject("editingarea");
	if (obj!=NULL)
	{
		game.Draw(obj->area.boundingbox);
	}
	else
	{
		game.Draw();		
	}

	const ScreenObject * obj2 = screen.GetObject("selecttile");
	float zoom		= obj2->area.boundingbox.top - obj2->area.boundingbox.bottom;// 0.162f;
	float uiOffset 	= obj2->area.boundingbox.bottom;// 0.011f;
	for (int i = 0; i < 25; ++i)
	{
		game.level.tiles.Draw(i, 
						uiOffset + zoom + uiOffset + ((i % 5) / 5.0f * zoom), 
						uiOffset + ((i/5) / 5.0f * zoom), 0.2*zoom, 0.2*zoom);
	}
	game.level.tiles.Draw(currenttile, uiOffset, uiOffset, zoom, zoom);
	screen.Draw();
}
