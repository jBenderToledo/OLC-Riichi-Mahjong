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

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here



		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		int w = ScreenWidth(), h = ScreenHeight();

		

		return true;
	}
};

int main()
{
	unsigned int k = 4, w = 640, h = 480;

	Example demo;
	if (demo.Construct(w / k, h / k, k, k, false, true))
		demo.Start();

	return 0;
}