#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

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
	static std::string TileFileName[37];
	static enum class TileIndex {
		EAST, SOUTH, WEST, NORTH,													// Winds; honors.
		RED, WHITE, GREEN,															// Dragons; honors.
		PIN_1, PIN_2, PIN_3, PIN_4, PIN_5, PIN_5_DORA, PIN_6, PIN_7, PIN_8, PIN_9,  // Suit: Dots, AKA pins.
		SOU_1, SOU_2, SOU_3, SOU_4, SOU_5, SOU_5_DORA, SOU_6, SOU_7, SOU_8, SOU_9,  // Suit: Bamboo, AKA sou/souzu.
		MAN_1, MAN_2, MAN_3, MAN_4, MAN_5, MAN_5_DORA, MAN_6, MAN_7, MAN_8, MAN_9   // Suit: Character, AKA man/kanji.
	};

	typedef struct TileImage
	{
		olc::Sprite* baseSprite, * faceSprite;
		olc::Decal* baseDecal, * faceDecal;
	};

	TileImage t;
	float tileScale;

	void MakeTile(TileImage* tDestination)
	{
		tDestination->baseSprite = new olc::Sprite("./Regular/Front.png");
		tDestination->faceSprite = new olc::Sprite("./Regular/Chun.png");

		tDestination->baseDecal = new olc::Decal(t.baseSprite);
		tDestination->faceDecal = new olc::Decal(t.faceSprite);
	}

public:

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		tileScale = 0.3;

		MakeTile(&t);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::VERY_DARK_BLUE);

		olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

		DrawDecal(mouse, t.baseDecal, { tileScale,tileScale });
		DrawDecal(mouse, t.faceDecal, { tileScale,tileScale });

		return true;
	}
};

std::string Example::TileFileName[37] = { // Source files are based on eastern names.
		"Ton.png", "Nan.png", "Shaa.png", "Pei.png", // East, south, west, north
		"Chun.png", "Haku.png", "Hatsu.png",         // Red, white, green
		"Pin1.png", "Pin2.png", "Pin3.png", "Pin4.png", "Pin5.png", "Pin5-Dora.png", "Pin6.png", "Pin7.png", "Pin8.png", "Pin9.png",
		"Sou1.png", "Sou2.png", "Sou3.png", "Sou4.png", "Sou5.png", "Sou5-Dora.png", "Sou6.png", "Sou7.png", "Sou8.png", "Sou9.png",
		"Man1.png", "Man2.png", "Man3.png", "Man4.png", "Man5.png", "Man5-Dora.png", "Man6.png", "Man7.png", "Man8.png", "Man9.png"
};

std::string Example::TILE_IMAGE_URL_BASE_FORMAT = "./%s/%s";

int main()
{
	std::cout << "Start!\n";

	Example demo;
	if (demo.Construct(1280, 720, 1, 1, false, true))
		demo.Start();

	std::cout << "\nEnd!\n";

	return 0;
}