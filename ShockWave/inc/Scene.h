#ifndef _SCENE_H_
#define _SCENE_H_

#include "Common.h"

namespace ShockWaveApp
{

using namespace std;
class ShockWave;

class Scene
{

public:
	//--------- Public methods.
		// Return scene instance.
static  Scene*	GetScene	( int inWidth, int inHeight, int inBitsPerPixel, string inTitle );
		// Change current video mode to other.  
		void	ChangeVideoMode
							( int inWidth, int inHeight, int inBitsPerPixel );
		// Change background.
		void	LoadBackground
							( string inBackFilename );
		// Main loop which will do all animation actions. 
		void	MainLoop	( void );

private:
	//--------- Constructor's and assignment operator.
				Scene		( int inWidth, int inHeight, int inBitsPerPixel, string inTitle );
				Scene		( const Scene& ){};
		Scene&  operator =  ( const Scene& ){};
				~Scene		( void );
	//--------- Private methods.
		// Update all scene.	
		void	UpdateScene ( void );
		// Update surface wave animation.
		void	UpdateSurfaceWave
							( Uint32 );
		// Update FPS indicator. 
		void	UpdateFPS	( Uint32 inTime );
		// Update shock waves animation.
		void	UpdateShockWaves
							( void );

		void	UpdateTest	( void );
		// Update text lines and draw it/
		void	UpdateText	( void );
		// Check events and make an actions which was associated with each.
		void	SwitchEvents( void );
		void	SwitchTestEvents
							( void );
		// Clean all scene objects.
		void	CleanScene	( void );
		// 
		void	ClearShockWaveList
							( void );
		// Draw surface wave frame.
		void	SurfaceWave	( void );
		// Draw text menu to screen.
		void	DrawText	( const char* inText, SDL_Rect& inRect );
		// Generate one shock wave in random position.
		void    GenerateShockWave
							( void );
		
		void	ArialFontPath
							( void );
		
		void ShowTestResults( void );

		bool	LoadBmp		( string& outFilename );
private:
		SDL_Surface*		mScreen;		// Main scene surface.
		SDL_Surface*		mBack;			// Background surface.	
		string				mTitle;			// Window title.
		Uint32*				mTestResult;
		Uint32				mTestStart;

		string				mFontPath;		// Path to TTF font file.
		TTF_Font*			mFont;			// TTF font for text output. 
		Uint32				mFontSize;		// Size of font chars in pixels.
		SDL_Color			mBackColor;		// 
		SDL_Color			mFontColor;		// Text color.
		SDL_Rect			mTextPosition;  // Text position rectangle.
		vector<string>		mTextLines;		// Text lines to output.
		
		
		ShockWaveList		mWavesList;		// List for generating shock waves.
		ShockWave*			mTestWave;
		Uint32				mGenLastTime;	// Last time when shock wave was generated.
		Uint32				mGenTimeStep;   // Time step for generating next wave.
		Uint32				mGenMinTime;	// Minimum time step for wave generator. 
		Uint32				mGenMaxTime;	// Maximum.
		
		Uint32				mSpeed;			// Speed of wave propagation in pixels. 
		SDL_Event			mEvents;		// Event queue.
		Uint32				mLastFpsTime;	// Last time when FPS was calculated.
		Uint32				mFramesCounter; // Counts frames. Reset after 1 second.
		Uint32				mCurrentFPS;
		int					mThreadsNum;
		
		Uint32				mSurfLastTime;	// Last time when surface wave was updated.
		Uint32				mSurfTimeStep;	// Step for updating surface wave.
		int					mSurfWaveX;		// Surface wave X position.
		int					mWaveLen;		// Length of both waves type. 
		int					mWaveAmp;		// Amplitude for both waves.
		int					mRadius;		// Current radius of shock wave.
		
		bool				mDrawTextOn;
		bool				mLoopRunOn;		// Main loop running flag.
		bool				mAnimatedOn;		// Flag for surface wave animation.
		bool				mSurfWaveOn;	// Flag for surface wave showing.
		bool				mTestModeOn;
};

}

#endif //_SCENE_H_
