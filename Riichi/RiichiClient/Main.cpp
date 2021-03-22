#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "Example";
	}

public:
	olc::Sprite * tileDemo, * tileFaceDemo;
	olc::Decal * tileDemoDecal, * tileFaceDemoDecal;

	float tileScale;

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		tileDemo = new olc::Sprite("./Regular/Front.png");
		tileFaceDemo = new olc::Sprite("./Regular/Chun.png");

		tileDemoDecal = new olc::Decal(tileDemo);
		tileFaceDemoDecal = new olc::Decal(tileFaceDemo);

		tileScale = 0.2;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::VERY_DARK_BLUE);
		olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

		DrawDecal(mouse, tileDemoDecal, { tileScale, tileScale });
		DrawDecal(mouse, tileFaceDemoDecal, { tileScale, tileScale });

		return true;
	}
};

int main()
{
	std::cout << "Start!\n";

	Example demo;
	if (demo.Construct(1280, 720, 1, 1, false, true))
		demo.Start();

	std::cout << "\nEnd!\n";

	return 0;
}