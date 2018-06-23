#include <Windows.h>
#include "Scene.h"
#include "AppException.h"

using namespace ShockWaveApp;

int main(int argc, char *argv[])
{
	try
	{
		Scene* scene = Scene::GetScene( 640, 480, 24, "ShockWaves" );
		scene->MainLoop();
	}
	catch( AppException& exc )
	{
		MessageBoxA( NULL, exc.Text.c_str(), "Error", MB_OK | MB_ICONERROR );
	}
	return 0;
}
