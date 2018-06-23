#ifndef _MAIN_APP_
#define _MAIN_APP_

#include <cstdio>
#include <cstdlib>
#include <SDL.h>
#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include "FishnetSimulator.h"


////////////////////////////////////////////////////////////////////////////////
// Exceptions for SDL subsystem errors.
//
class SDLError
{

};

//------------------------------------------------------------------------------
class MainApp
{
	public:
		//----------------------------------------------------------------------
		MainApp( Uint32 inFlag = SDL_INIT_EVERYTHING );
		
		//----------------------------------------------------------------------
		~MainApp( void );
	
	
	//--------------------------------------------------------------------------
	void	InitSimulator( 
					Vector3F inGridPos,
					int inNumOfColumns,			// Number of columns.
					int inNumOfRows,			// Number of rows in fishnet.
					float inNodeMass,			// Mass of every node.
					float inLinkLength,			// Length of link, which connect two node's.		
					float inLinkForce,			// Force which occurs when link stretching.
					float inLinkFrictionForce,	// Link friction force.
					Vector3F inGravity,			// Earth gravity force.
					float inAirFrictionForce );	// Air friction force.
	
	//--------------------------------------------------------------------------
	void Init(
			int inWidth,				// Resolution in SDL or OpenGL window.
			int inHeight,				// Width and Height.
			Uint32 inFlags );				// Flags, which init some SDL video mode.

	//--------------------------------------------------------------------------
	void	SceneLoop( void );
	
private:
	
	//--------------------------------------------------------------------------
	void	DoSimulate( Uint32 inTime );
	//--------------------------------------------------------------------------
	void	DrawScene( void );
	
	//--------------------------------------------------------------------------
	// Data.
private:

	// Simulator, which contains nodes and link.
	FishnetSimulator*	mSimulator;

	// Main SDL screen 
	SDL_Surface* pScreen;
	SDL_Event	 mEvent;
	
	// OpenGL coordinates borders. 
	float		 mGLminX;
	float		 mGLminY;
	float		 mGLmaxX;
	float		 mGLmaxY;
	
	// Frames per second counter.
	float		mFramesPerSecond;
	
	// Mouse stats.
	float		mPixelX;
	float		mPixelY;
	float		mAccX;
	float		mAccY;

	//Last FPS time in ms.
	Uint32		mLastFPSTick;
	
	// Last simulation time in ms.
	Uint32		 mLastTime;

};

#endif