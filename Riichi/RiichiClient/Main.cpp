
// NOTE: WILL BREAK IN DEBUG MODE FOR SEMI-OBVIOUS REASONS.
#include <omp.h>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// #define TIME_LOADING_IMAGES // Turn on to record and display times via stdout for testing image loading.

// #define MAHJONG_DEBUG // Turn on to view debug messages sent via stdout.

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "Example";
	}

private:
	static std::string TILE_IMAGE_URL_BASE_FORMAT;
	static std::string TILE_FILENAME[38];
	static enum class TileIndex : int {
		PIN_1 = 0, PIN_2, PIN_3, PIN_4, PIN_5, PIN_5_DORA, PIN_6, PIN_7, PIN_8, PIN_9,  // Suit: Dots, AKA pins.
		SOU_1, SOU_2, SOU_3, SOU_4, SOU_5, SOU_5_DORA, SOU_6, SOU_7, SOU_8, SOU_9,		// Suit: Bamboo, AKA sou/souzu.
		MAN_1, MAN_2, MAN_3, MAN_4, MAN_5, MAN_5_DORA, MAN_6, MAN_7, MAN_8, MAN_9,		// Suit: Character, AKA man/kanji.
		EAST, SOUTH, WEST, NORTH,														// Winds; honors.
		RED, WHITE, GREEN,																// Dragons; honors.
		BLANK																			// Undefined or hidden.
	};

	typedef struct TileImage
	{
		olc::Decal* base, * face;
	};

	typedef struct Tile
	{
		TileIndex ID;
		TileImage image;
	};

	Tile BlackTiles[39];
	Tile RegularTiles[39];
	
	float TileScale;
	int FrameCounter;

	Tile* t;

	void MakeTiles_PARALLELIZED()
	{
		static std::string BASE_BLACK_FILENAME = "./Black/Front.png";
		static std::string BASE_REGULAR_FILENAME = "./Regular/Front.png";
		static olc::Sprite* BASE_BLACK_SPRITE = new olc::Sprite(BASE_BLACK_FILENAME);
		static olc::Sprite* BASE_REGULAR_SPRITE = new olc::Sprite(BASE_REGULAR_FILENAME);
		static olc::Decal* BASE_BLACK_DECAL = new olc::Decal(BASE_BLACK_SPRITE);
		static olc::Decal* BASE_REGULAR_DECAL = new olc::Decal(BASE_REGULAR_SPRITE);

		static std::string BLACK_PATH = "./Black/";
		static std::string REGULAR_PATH = "./Regular/";

#ifdef TIME_LOADING_IMAGES
		clock_t allstart, allend;

		allstart = clock();
#endif

		// Figure out how to parallelize

#pragma omp parallel for
		for (int i = 0; i < 38; i++)
		{
			BlackTiles[i].image.base = BASE_BLACK_DECAL;
			RegularTiles[i].image.base = BASE_REGULAR_DECAL;

			BlackTiles[i].image.face = new olc::Decal(new olc::Sprite(BLACK_PATH + TILE_FILENAME[i]));
			RegularTiles[i].image.face = new olc::Decal(new olc::Sprite(REGULAR_PATH + TILE_FILENAME[i]));
		}

#ifdef TIME_LOADING_IMAGES
		allend = clock();

		std::cout << "\nTime to load all:\t" << (allend - allstart) / (float)CLOCKS_PER_SEC << "s\n";
#endif
	}

	void MakeTiles_PROCEDURAL()
	{
		static std::string BASE_BLACK_FILENAME = "./Black/Front.png";
		static std::string BASE_REGULAR_FILENAME = "./Regular/Front.png";
		static olc::Sprite* BASE_BLACK_SPRITE = new olc::Sprite(BASE_BLACK_FILENAME);
		static olc::Sprite* BASE_REGULAR_SPRITE = new olc::Sprite(BASE_REGULAR_FILENAME);
		static olc::Decal* BASE_BLACK_DECAL = new olc::Decal(BASE_BLACK_SPRITE);
		static olc::Decal* BASE_REGULAR_DECAL = new olc::Decal(BASE_REGULAR_SPRITE);

		static std::string BLACK_PATH = "./Black/";
		static std::string REGULAR_PATH = "./Regular/";

		// Figure out how to parallelize

		for (int i = 0; i < 38; i++)
		{
			BlackTiles[i].image.base = BASE_BLACK_DECAL;
			RegularTiles[i].image.base = BASE_REGULAR_DECAL;

			BlackTiles[i].image.face = new olc::Decal(new olc::Sprite(BLACK_PATH + TILE_FILENAME[i]));
			RegularTiles[i].image.face = new olc::Decal(new olc::Sprite(REGULAR_PATH + TILE_FILENAME[i]));
		}
	}

public:

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		TileScale = 1.0;
		FrameCounter = 15;

		// MakeTiles_PROCEDURAL();
		MakeTiles_PARALLELIZED();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		int randIndex;

		FrameCounter++;
		while (FrameCounter >= 15)
		{
			FrameCounter -= 15;
			randIndex = rand() % 74;
			t = (randIndex & 1) ? &BlackTiles[randIndex >> 1] : &RegularTiles[randIndex >> 1];
		}



		Clear(olc::VERY_DARK_BLUE);

		olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

		DrawDecal(mouse, t->image.base, { TileScale,TileScale });
		DrawDecal(mouse, t->image.face, { TileScale,TileScale });

		return true;
	}
};

std::string Example::TILE_FILENAME[38] = { // Source files are based on eastern names.
		"Pin1.png", "Pin2.png", "Pin3.png", "Pin4.png", "Pin5.png", "Pin5-Dora.png", "Pin6.png", "Pin7.png", "Pin8.png", "Pin9.png",
		"Sou1.png", "Sou2.png", "Sou3.png", "Sou4.png", "Sou5.png", "Sou5-Dora.png", "Sou6.png", "Sou7.png", "Sou8.png", "Sou9.png",
		"Man1.png", "Man2.png", "Man3.png", "Man4.png", "Man5.png", "Man5-Dora.png", "Man6.png", "Man7.png", "Man8.png", "Man9.png",
		"Ton.png", "Nan.png", "Shaa.png", "Pei.png", // East, south, west, north
		"Chun.png", "Haku.png", "Hatsu.png",         // Red, white, green
		"Blank.png"
};

std::string Example::TILE_IMAGE_URL_BASE_FORMAT = "./%s/%s";

int main()
{
	Example demo;
	if (demo.Construct(1280, 720, 1, 1, false, true))
	{
#ifndef MAHJONG_DEBUG
		ShowWindow(GetConsoleWindow(), 0);
#endif
		demo.Start();
	}
	return 0;
}