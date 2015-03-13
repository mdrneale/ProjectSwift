#include "gamescreen.h"
#include "graphics.h"
#include "font.h"
#include "window.h"
#include "statemanager.h"
#include "mainmenu.h"
#include "winscreen.h"

#include "iohelper.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

static std::string hs = "";

GameState::GameState()
{
	Init();
}

bool GameState::Init()
{
	DisplayHighScore = false;
	HUDImage.SetTexture("assets/views/game.png");
	screen = Screen("assets/views/editor.json");
	collectableNumberImage.SetTexture(Font::GetFont("assets/oj.ttf",48),"100",180,80,80,255);
	collectable2NumberImage.SetTexture(Font::GetFont("assets/oj.ttf",48),"0",80,190,120,255);
	newHighScoreImage.SetTexture("assets/newhighscore.png");
	fallingRockSound = new Sound("assets/sounds/click.wav");
	squishSound = new Sound("assets/sounds/squish.wav");
	chokeSound = new Sound("assets/sounds/choke.wav");

	screen.SetClickResponder(this);
	Graphics::GetGraphics()->SetDrawColour(0.5,1.0,1);

	LoadLevel(game.currentlevel);
}

void GameState::Clicked(ScreenObject & ob, Button button)
{
	if (ob.name=="back")
	{
		StateManager::GetStateManager()->SwitchState( MainMenuState::GetMainMenu() );
	}
	printf("clicked on %s! %f, %f\n", ob.name.c_str(), Window::mouse.currentPosition.x, Window::mouse.currentPosition.y);
}
void GameState::Released	(ScreenObject & ob, Button button) {}
void GameState::Hovered 	(ScreenObject & ob, Button button) {}
void GameState::UnHovered	(ScreenObject & ob, Button button) {}

void GameState::KillPlayer()
{
	printf("player killed!\n");
	LoadLevel(game.currentlevel);
}

void GameState::LoadLevel(unsigned int levelnumber)
{
	if (levelnumber >= game.levelfiles.size())
	{
		StateManager::GetStateManager()->SwitchState( new WinScreenState() );		
	}

	game.LoadLevel(levelnumber);

	numberOfCollectables = 0;
	numberOfCollectables2 = 0;
	for (int i = 0; i < game.level.width * game.level.height; ++i)
	{
		if (game.level.data[i]==TileResource::SWEET)
		{
			numberOfCollectables++;
		}
		else if (game.level.data[i]==TileResource::BONUS)
		{
			numberOfCollectables2++;
		}
		else if (game.level.data[i]==TileResource::PLAYER)
		{
			playerx = i % game.level.width;
			playery = i / game.level.width;
		}
	}
	game.level.mapZoom = 4;
	game.level.SetOffset((playerx), (playery));

	printf("%s\n", NtoS(numberOfCollectables).c_str());
	collectableNumberImage.SetTexture(Font::GetFont("assets/oj.ttf",48), NtoS(numberOfCollectables).c_str(),180,80,80,255);
	collectable2NumberImage.SetTexture(Font::GetFont("assets/oj.ttf",48), NtoS(numberOfCollectables2).c_str(),80,190,120,255);
	levelTime = 0;
}

void GameState::MovePlayer(int x, int y)
{
	game.level.Move(x,y);
	game.level.SetTile(playerx + x, playery + y,TileResource::PLAYER);		
	game.level.SetTile(playerx, playery,TileResource::EMPTY);
	playerx+=x;
	playery+=y;
}

void GameState::MoveRock(int x, int y)
{
}

void GameState::UpdateInput()
{
	int nx=0;
	int ny=0;
	bool move = false;

	if (Window::keyboard.keys[Keyboard::KLEFT]==Keyboard::DOWN)
	{
		playerFlipped = true;
		playerRotation = 0;
		nx=-1;
		move = true;
	}
	else if (Window::keyboard.keys[Keyboard::KRIGHT]==Keyboard::DOWN)
	{
		playerFlipped = false;
		playerRotation = 0;
		nx=1;
		move = true;
	}
	else if (Window::keyboard.keys[Keyboard::KUP]==Keyboard::DOWN)
	{
		playerFlipped = false;
		playerRotation = -90;
		ny=1;
		move = true;
	}
	else if (Window::keyboard.keys[Keyboard::KDOWN]==Keyboard::DOWN)
	{
		playerFlipped = false;
		playerRotation = 90;
		ny=-1;
		move = true;
	}
	else if (Window::keyboard.keys['q']==Keyboard::DOWN)
	{
		KillPlayer();
	}

	if (Window::keyboard.keys['1']==Keyboard::DOWN)
	{
		if (game.currentlevel!=0)
		{
			LoadLevel(game.currentlevel-1);
		}
	}
	else if (Window::keyboard.keys['2']==Keyboard::DOWN)
	{
		if (game.currentlevel < highscore.scores.size())
		{
			LoadLevel(game.currentlevel+1);
		}
	}

	if (move)
	{
		int nexttile = game.level.GetTile(playerx+nx, playery+ny);
		if (nexttile==TileResource::EMPTY)
		{
			MovePlayer(nx,ny);
		}
		else if (nexttile == TileResource::ROCK)
		{
			int nextnexttile = game.level.GetTile(playerx+(2*nx), playery);//+(2*ny));
			if (nextnexttile == TileResource::EMPTY)
			{
				game.level.SetTile(playerx+(2*nx), playery+(2*ny), TileResource::ROCK);
				MovePlayer(nx,ny);
			}

		}
		else if (nexttile == TileResource::SWEET)
		{
			MovePlayer(nx,ny);
			numberOfCollectables -= 1;
			collectableNumberImage.SetTexture(Font::GetFont("assets/oj.ttf",48), NtoS(numberOfCollectables).c_str(),180,80,80,255);
			printf("%s\n", NtoS(numberOfCollectables).c_str());

		}
		else if (nexttile == TileResource::BONUS)
		{
			MovePlayer(nx,ny);
			numberOfCollectables2 -= 1;
			collectable2NumberImage.SetTexture(Font::GetFont("assets/oj.ttf",48), NtoS(numberOfCollectables2).c_str(),80,190,120,255);
		}
		else if (nexttile == TileResource::DEATH)
		{
			chokeSound->Play();
			KillPlayer();
		}
	}	
}

void GameState::UpdateRock(int x, int y)
{
	if (game.level.GetTile(x,y-1) == TileResource::EMPTY)
	{
		if (game.level.GetTile(x,y-2) == TileResource::PLAYER)
		{
			squishSound->Play();
			KillPlayer();
			return;
		}
		game.level.SetTile(x, y-1, TileResource::ROCK);		
		game.level.SetTile(x, y, TileResource::EMPTY);
		fallingRockSound->Play();
	}
	else if (game.level.GetTile(x,y-1) == TileResource::LEFTWALL && game.level.GetTile(x-1,y) == TileResource::EMPTY && game.level.GetTile(x-1,y-1) == TileResource::EMPTY)
	{
		game.level.SetTile(x-1, y, TileResource::ROCK);		
		game.level.SetTile(x, y, TileResource::EMPTY);
		fallingRockSound->Play();
	}
	else if (game.level.GetTile(x,y-1) == TileResource::RIGHTWALL && game.level.GetTile(x+1,y) == TileResource::EMPTY && game.level.GetTile(x+1,y-1) == TileResource::EMPTY)
	{
		game.level.SetTile(x+1, y, TileResource::ROCK);		
		game.level.SetTile(x, y, TileResource::EMPTY);
		fallingRockSound->Play();
	}
	else if (game.level.GetTile(x,y-1) == TileResource::ROCK && game.level.GetTile(x-1,y) == TileResource::EMPTY && game.level.GetTile(x-1,y-1) == TileResource::EMPTY)
	{
		game.level.SetTile(x-1, y, TileResource::ROCK);		
		game.level.SetTile(x, y, TileResource::EMPTY);
		fallingRockSound->Play();
	}
	else if (game.level.GetTile(x,y-1) == TileResource::ROCK && game.level.GetTile(x+1,y) == TileResource::EMPTY && game.level.GetTile(x+1,y-1) == TileResource::EMPTY)
	{
		game.level.SetTile(x+1, y, TileResource::ROCK);		
		game.level.SetTile(x, y, TileResource::EMPTY);
		fallingRockSound->Play();
	}

}

void GameState::UpdateRocks()
{
	for (int y = 0; y < game.level.height; y++)
	{
		for (int x = 0; x < game.level.width; x++)
		{
			if (game.level.GetTile(x,y) == TileResource::ROCK)
			{
				UpdateRock(x,y);
			}
		}
	}
}

bool GameState::Update(float timestep)
{
	if (DisplayHighScore)
	{
		DisplayHighScoreTime += timestep;
		if (DisplayHighScoreTime > 3.5f)
		{
			DisplayHighScore = false;
			LoadLevel(game.currentlevel+1);
		}
	}
	else
	{

		levelTime += timestep;
		levelTimeImage.SetTexture(Font::GetFont("assets/oj.ttf",48), NtoS(levelTime).c_str(),180,80,80,255);
		UpdateInput();
		UpdateRocks();
		if (numberOfCollectables <=0)
		{
			printf("player won!!!\n");
			if (!highscore.IsHighScore(game.currentlevel,Score(levelTime, numberOfCollectables2)))
			{
				LoadLevel(game.currentlevel+1);
			}
			else
			{
				DisplayHighScore = true;
				DisplayHighScoreTime=0;
				hs = "time taken: ";
				hs += NtoS(levelTime);
				hs += " with ";
				hs += NtoS(numberOfCollectables2);
				hs += " bonus\n collectables left to collect.";
				newHighScoreStatsImage.SetTexture(Font::GetFont("assets/oj.ttf",48), hs.c_str(), 0, 0, 0, 255);

			}
		}
	}
	screen.Update(timestep);
	return true;
}

void GameState::Draw()
{
	game.DrawWithoutPlayer();

	bool skip = false;
	for (int y = 0; y < game.level.height; y++)
	{
		if (skip)
		{
			break;
		}
		for (int x = 0; x < game.level.width; x++)
		{
			int tile = game.level.data[x+(y*game.level.width)];
			if (tile == TileResource::PLAYER)
			{
				Geometry::Square area;
				DrawPlayer(game.level.GetTileScreenPosition(x, y, area));
				skip = true;
				break;
			}
		}
	}


	HUDImage.RenderTexture(0,0,1.422,1);
	game.level.tiles.Draw(TileResource::SWEET, 0.02, 0.018, 0.03, 0.03);
	collectableNumberImage.RenderTexture(0.055,0.022,0.015*NtoS(numberOfCollectables).size(),0.03);

	game.level.tiles.Draw(TileResource::BONUS, 0.098, 0.018, 0.03, 0.03);
	collectable2NumberImage.RenderTexture(0.136,0.022,0.015*NtoS(numberOfCollectables2).size(),0.03);

	levelTimeImage.RenderTexture(0.2,0.022,0.015*NtoS(levelTime).size(),0.03);

	if (DisplayHighScore)
	{
		newHighScoreImage.RenderTexture(0.711f - 0.5f, 0.5f-0.25f, 1.0f, 0.5f);
		float ratio = newHighScoreStatsImage.width / (float) newHighScoreStatsImage.height;
		newHighScoreStatsImage.RenderTexture(0.711f - (0.5f * (ratio * 0.15f)), 0.55f-(0.5f * 0.15f), (ratio * 0.15f), 0.15f);
	}

	screen.Draw();
}

void GameState::DrawPlayer(Geometry::Square & area)
{
	game.level.tiles.Draw(TileResource::PLAYER, area.left, area.top, area.right,area.bottom, 0, 0, 1, 1, playerRotation, playerFlipped);
}
