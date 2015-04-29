#include "engineSystem.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	EngineSystem* system;
	bool result;
	
	
	// Create the system object.
	system = new EngineSystem;
	if(!system)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = system->Initialize();
	if(result)
	{
		system->Run();
	}

	// Shutdown and release the system object.
	system->Shutdown();
	delete system;
	system = 0;

	return 0;
}
