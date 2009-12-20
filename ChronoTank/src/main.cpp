/************************************************/
/*	Program main entry point					*/
/************************************************/
#include <irrlicht.h>
#include <time.h>

#include "game.h"

using namespace ctank;
using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

// Main entry point
int main() {

	// Create game
	IGame* game = CreateGame();

	// Create the render device
	core::dimension2d<s32> dim(1024, 768);
	IrrlichtDevice* device =
		createDevice(video::EDT_DIRECT3D9, dim,
		16, false, false, false, game);

	//Initialize
	game->Init(device);

	// Random
	srand((unsigned int)time(NULL));


	// Main render loop
	clock_t lastupdate = clock();
	float avgupdatetime = 1.0f / 400.0f;
	float updatechangerate = 50.0f;
	while(device->run()) {

		// Time
		clock_t curtime = clock();
		clock_t elapsed = curtime - lastupdate;
		lastupdate = curtime;
		float update = (float)elapsed / 1000.0f;
		avgupdatetime = ((avgupdatetime * updatechangerate) + update) / (updatechangerate + 1);

		// Update
		game->Update(avgupdatetime);
		game->Render();

	}

	// Clean up
	game->Destroy();
	device->drop();
	delete game;
}
