//******************************************************************************//
//	main.cpp																	//
//																				//
//	Fishnet Simulation program													//
//																				//
//	Maksim	Skorokhod	2011													//	
//																				//
//******************************************************************************//
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <assert.h>
#include <iostream>

#include "MainApp.h"

using std::cout;
using std::cin;
using std::endl;

#define MY_WIDTH	640
#define MY_HEIGHT	480


int main( int argc, char** argv )
{
	MainApp	App;
	int NumOfColumns = 15;
	int NumOfRows = 10; 			
	float NodeMass = 0.08f;
	float LinkLength = 0.12f;				
	float LinkForce = 900.0f;			
	float LinkFrictionForce = 2.0f;	
	Vector3F Gravity( 0.0f, 9.8f, 0.0f );	
	Vector3F GridPos( 0.2f, 0.2f, 0.0 );
	float AirFrictionForce = 0.05f;
	try
	{
		App.InitSimulator(
							GridPos,
							NumOfColumns,
							NumOfRows,
							NodeMass,
							LinkLength,		
							LinkForce,
							LinkFrictionForce,	
							Gravity,		
							AirFrictionForce );

		App.Init( MY_WIDTH, MY_HEIGHT, SDL_OPENGL );
		App.SceneLoop();

	}
	catch( SDLError )
	{
			cout<<"Some SDL error: "<<SDL_GetError()<<endl;
	}
	return 0;
}
