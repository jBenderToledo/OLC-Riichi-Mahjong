
// NOTE: WILL BREAK IN DEBUG MODE FOR SEMI-OBVIOUS REASONS.
#include <omp.h>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// #define TIME_LOADING_IMAGES // Turn on to record and display times via stdout for testing image loading.

#define MAHJONG_DEBUG // Turn on to view debug messages sent via stdout.

class RiichiClient : public olc::PixelGameEngine
{
public:
	RiichiClient()
	{
		// Name your application
		sAppName = "Riichi Client";
	}

private:
	const float TILE_SCALE = 0.225;
	const float FACE_SCALE = 0.875;

	const float FACE_SCALE_COMPLEMENT = 1 - FACE_SCALE;
	const float TILE_FACE_SCALE_PRODUCT = TILE_SCALE * FACE_SCALE;
	const float DRAW_TILE_CENTER_OFFSET_RATIO = 0.5 * TILE_SCALE * FACE_SCALE_COMPLEMENT;
	
	const std::string TILE_IMAGE_URL_BASE_FORMAT = "./%s/%s";
	const std::string TILE_FILENAME[38] = { // Source files are based on eastern names.
		"Pin1.png", "Pin2.png", "Pin3.png", "Pin4.png", "Pin5.png", "Pin5-Dora.png", "Pin6.png", "Pin7.png", "Pin8.png", "Pin9.png",
		"Sou1.png", "Sou2.png", "Sou3.png", "Sou4.png", "Sou5.png", "Sou5-Dora.png", "Sou6.png", "Sou7.png", "Sou8.png", "Sou9.png",
		"Man1.png", "Man2.png", "Man3.png", "Man4.png", "Man5.png", "Man5-Dora.png", "Man6.png", "Man7.png", "Man8.png", "Man9.png",
		"Ton.png", "Nan.png", "Shaa.png", "Pei.png", // East, south, west, north
		"Chun.png", "Haku.png", "Hatsu.png",         // Red, white, green
		"Blank.png"
	};
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
		olc::Decal* Base, * Face;
	};

	typedef struct TileModel
	{
		TileIndex ID;
		TileImage Image;
	};

	typedef struct Tile
	{
		TileModel* Model;
		olc::vf2d Position;
	};

	// Contains all of the visual information for black and non-black tile images.
	TileModel BlackTiles[39];
	TileModel RegularTiles[39];

	Tile MyHand[14];
	
	// Variables that are just here for an example and will probably be removed between versions.
	int FrameCounter;
	int RandIndex;
	float TotalElapsedTime;
	TileModel* TileModelPtr;

	void MakeTiles_PARALLEL()
	{
		// 1. Determine filenames, make sprites, make decals from sprites for the bases.
		static std::string BASE_BLACK_FILENAME = "./Black/Front.png";
		static std::string BASE_REGULAR_FILENAME = "./Regular/Front.png";
		static olc::Sprite* BASE_BLACK_SPRITE = new olc::Sprite(BASE_BLACK_FILENAME);
		static olc::Sprite* BASE_REGULAR_SPRITE = new olc::Sprite(BASE_REGULAR_FILENAME);
		static olc::Decal* BASE_BLACK_DECAL = new olc::Decal(BASE_BLACK_SPRITE);
		static olc::Decal* BASE_REGULAR_DECAL = new olc::Decal(BASE_REGULAR_SPRITE);

		// 2. Start determining file names.
		static std::string BLACK_PATH = "./Black/";
		static std::string REGULAR_PATH = "./Regular/";

#ifdef TIME_LOADING_IMAGES
		clock_t allstart, allend;

		allstart = clock();
#endif

		// 4.
		// For each unique tile face, load both the black and non-black variate images
		//    and store them inside sprites that, anonymous, go into decals.
		// Done in parallel because file I/O is extremely time-expensive and it takes
		//    almost four times as long if I don't.
#pragma omp parallel for
		for (int i = 0; i < 38; i++)
		{
			BlackTiles[i].ID = (TileIndex) i;
			RegularTiles[i].ID = (TileIndex) i;

			BlackTiles[i].Image.Base = BASE_BLACK_DECAL;
			RegularTiles[i].Image.Base = BASE_REGULAR_DECAL;

			BlackTiles[i].Image.Face = new olc::Decal(new olc::Sprite(BLACK_PATH + TILE_FILENAME[i]));
			RegularTiles[i].Image.Face = new olc::Decal(new olc::Sprite(REGULAR_PATH + TILE_FILENAME[i]));
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

		for (int i = 0; i < 38; i++)
		{
			BlackTiles[i].Image.Base = BASE_BLACK_DECAL;
			RegularTiles[i].Image.Base = BASE_REGULAR_DECAL;

			BlackTiles[i].Image.Face = new olc::Decal(new olc::Sprite(BLACK_PATH + TILE_FILENAME[i]));
			RegularTiles[i].Image.Face = new olc::Decal(new olc::Sprite(REGULAR_PATH + TILE_FILENAME[i]));
		}
	}

	void DrawTileDecal(Tile* DrawableTile, olc::vf2d Position)
	{
		if (DrawableTile->Model->ID != TileIndex::BLANK)
		{
			float dw_center, dh_center;
			dw_center = DrawableTile->Model->Image.Face->sprite->width * DRAW_TILE_CENTER_OFFSET_RATIO;
			dh_center = DrawableTile->Model->Image.Face->sprite->height * DRAW_TILE_CENTER_OFFSET_RATIO;
			olc::vf2d tileCenteringOffset = { dw_center, dh_center };

			DrawDecal(
				DrawableTile->Position,
				DrawableTile->Model->Image.Base,
				{ TILE_SCALE,TILE_SCALE }
			);
			DrawDecal(
				DrawableTile->Position + tileCenteringOffset,
				DrawableTile->Model->Image.Face,
				{ TILE_FACE_SCALE_PRODUCT, TILE_FACE_SCALE_PRODUCT }
			);
		}
	}

public:

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		TotalElapsedTime = 0.0f;

		// MakeTiles_PROCEDURAL();
		MakeTiles_PARALLEL();

		RandIndex = rand() % 38;
		TileModelPtr = &RegularTiles[RandIndex];

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		TotalElapsedTime += fElapsedTime;

		Clear(olc::VERY_DARK_BLUE);
		olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };
		Tile ourTile;

		while (TotalElapsedTime >= 1.0f)
		{
			TotalElapsedTime -= 1.0f;
			RandIndex = rand() % 38;
			TileModelPtr = &RegularTiles[RandIndex];
		}

		ourTile.Position = mouse;
		ourTile.Model = TileModelPtr;

		DrawTileDecal(&ourTile, mouse);

		return true;
	}
};

int main()
{
#ifdef MAHJONG_DEBUG
		ShowWindow(GetConsoleWindow(), 1);
#else
		ShowWindow(GetConsoleWindow(), 0);
#endif

	RiichiClient demo;
	if (demo.Construct(1920, 1080, 1, 1, false, false))
	{

		demo.Start();

	}
	return 0;
}