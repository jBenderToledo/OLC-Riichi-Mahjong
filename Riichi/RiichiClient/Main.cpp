#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <omp.h>

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
		EAST = 0, SOUTH, WEST, NORTH,												// Winds; honors.
		RED, WHITE, GREEN,															// Dragons; honors.
		PIN_1, PIN_2, PIN_3, PIN_4, PIN_5, PIN_5_DORA, PIN_6, PIN_7, PIN_8, PIN_9,  // Suit: Dots, AKA pins.
		SOU_1, SOU_2, SOU_3, SOU_4, SOU_5, SOU_5_DORA, SOU_6, SOU_7, SOU_8, SOU_9,  // Suit: Bamboo, AKA sou/souzu.
		MAN_1, MAN_2, MAN_3, MAN_4, MAN_5, MAN_5_DORA, MAN_6, MAN_7, MAN_8, MAN_9,  // Suit: Character, AKA man/kanji.
		BLANK                                                                       // Undefined or hidden.
	};

	typedef struct TileImage
	{
		olc::Decal* base, * face;
	};

	typedef struct Tile
	{
		TileImage image;
	};

	Tile BLACK_TILES[39];
	Tile REGULAR_TILES[39];
	float tileScale;

	void MakeTiles()
	{
		static std::string BASE_BLACK_FILENAME = "./Black/Front.png";
		static std::string BASE_REGULAR_FILENAME = "./Regular/Front.png";
		static olc::Sprite *BASE_BLACK_SPRITE = new olc::Sprite(BASE_BLACK_FILENAME);
		static olc::Sprite *BASE_REGULAR_SPRITE = new olc::Sprite(BASE_REGULAR_FILENAME);
		static olc::Decal* BASE_BLACK_DECAL = new olc::Decal(BASE_BLACK_SPRITE);
		static olc::Decal* BASE_REGULAR_DECAL = new olc::Decal(BASE_REGULAR_SPRITE);

		static std::string BLACK_PATH = "./Black/";
		static std::string REGULAR_PATH = "./Regular/";

		clock_t start[38], end[38], allstart, allend;

		std::string baseBlackPath = BASE_BLACK_FILENAME;
		std::string baseRegularPath = BASE_REGULAR_FILENAME;

		allstart = clock();
		{
			for (int i = 0; i < 38; i++)
			{
				BLACK_TILES[i].image.base = BASE_BLACK_DECAL;
				REGULAR_TILES[i].image.base = BASE_REGULAR_DECAL;

				start[i] = clock();

				BLACK_TILES[i].image.face = new olc::Decal(new olc::Sprite(BLACK_PATH + TILE_FILENAME[i]));
				REGULAR_TILES[i].image.face = new olc::Decal(new olc::Sprite(REGULAR_PATH + TILE_FILENAME[i]));

				end[i] = clock();

				std::cout << "Time to load 2x" << TILE_FILENAME[i] << ":\t" << (end[i] - start[i]) / (float)CLOCKS_PER_SEC << "s\n";
			}
		}
		allend = clock();

		std::cout << "\nTime to load all:\t" << (allend - allstart) / (float)CLOCKS_PER_SEC << "s\n";
	}

public:

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		tileScale = 1.0;

		MakeTiles();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Tile& t = REGULAR_TILES[(int) TileIndex::GREEN];

		Clear(olc::VERY_DARK_BLUE);

		olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

		DrawDecal(mouse, t.image.base, { tileScale,tileScale });
		DrawDecal(mouse, t.image.face, { tileScale,tileScale });

		return true;
	}
};

std::string Example::TILE_FILENAME[38] = { // Source files are based on eastern names.
		"Ton.png", "Nan.png", "Shaa.png", "Pei.png", // East, south, west, north
		"Chun.png", "Haku.png", "Hatsu.png",         // Red, white, green
		"Pin1.png", "Pin2.png", "Pin3.png", "Pin4.png", "Pin5.png", "Pin5-Dora.png", "Pin6.png", "Pin7.png", "Pin8.png", "Pin9.png",
		"Sou1.png", "Sou2.png", "Sou3.png", "Sou4.png", "Sou5.png", "Sou5-Dora.png", "Sou6.png", "Sou7.png", "Sou8.png", "Sou9.png",
		"Man1.png", "Man2.png", "Man3.png", "Man4.png", "Man5.png", "Man5-Dora.png", "Man6.png", "Man7.png", "Man8.png", "Man9.png",
		"Blank.png"
};

std::string Example::TILE_IMAGE_URL_BASE_FORMAT = "./%s/%s";

int main()
{
	Example demo;
	if (demo.Construct(1280, 720, 1, 1, false, false))
		demo.Start();

	return 0;
}