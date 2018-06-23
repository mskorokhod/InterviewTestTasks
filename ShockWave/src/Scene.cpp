#include "Common.h"
#include "Scene.h"
#include "ShockWave.h"

#include <cstdio>
#include <cstdlib>

namespace ShockWaveApp
{
	using std::sprintf;
//------------------------------------------------------------------------------
//
//
	Scene::Scene( int inWidth, int inHeight, int inBitsPerPixel, string inTitle )
{
	//-------- Init base SDL subsystems.
	int Result = SDL_Init( SDL_INIT_VIDEO );
	if( Result < 0 ) throw AppException(
										"SDL: Initialization error: " +
										string( SDL_GetError() ) ); 
	//------- Init TrueType font subsystem for text output.
	Result  = TTF_Init();
	if( Result < 0 ) throw AppException( 
										 "SDL: TTF initialization error: " +
										 string( SDL_GetError() ) );
	ArialFontPath();
	mFont = TTF_OpenFont( mFontPath.c_str(), mFontSize ); // 
	mFontColor.r = 255; mFontColor.g = 255; mFontColor.b = 255; // Blue color.
	// To position the text in the upper left corner with 10 pixels shift.
	mTextPosition.x = 10; mTextPosition.y = 10;				
	if( !mFont ) throw AppException( string( "SDL: Font opening error" ) );
	// Setting application window caption.
	mTitle = inTitle;
	SDL_WM_SetCaption( mTitle.c_str(), NULL );
	// Setting video mode.
	mScreen = SDL_SetVideoMode(
								inWidth,
								inHeight,
								inBitsPerPixel,
								SDL_DOUBLEBUF | SDL_HWSURFACE );
	if(!mScreen) throw AppException( string( "SDL: Video mode changing error" ) );
	// Setting default values for some variables.
	mGenLastTime = 0;	 
	mGenMinTime  = 5000;  
	mGenMaxTime	 = 10000; // 10 seconds.
	mGenTimeStep = 0;
	mLoopRunOn	 = true;
	mSpeed		 = 5;	  // Number of pixels which will added to each wave animation. 
	mWaveLen	 = 15;	  // 
	mWaveAmp	 = 20;
	mSurfWaveOn	 = true;
	mTestWave	 = NULL;
	mTestResult	 = new Uint32[kTestCount];

}

//------------------------------------------------------------------------------
//
//
Scene*	Scene::GetScene( int inWidth, int inHeight, int inBitsPerPixel, string inTitle )
{	
	static Scene scene( inWidth, inHeight, inBitsPerPixel, inTitle );
	return &scene;
}

//------------------------------------------------------------------------------
//
//
		Scene::~Scene( void )
{
	CleanScene();
}

//------------------------------------------------------------------------------
//
//
void	Scene::ChangeVideoMode(int inWidth, int inHeight, int inBitsPerPixel)
{
	SDL_Surface* tmpSrf = mScreen;
	mScreen = SDL_SetVideoMode(
		inWidth,
		inHeight,
		inBitsPerPixel,
		SDL_DOUBLEBUF | SDL_HWSURFACE );

	if(!mScreen)
	{
		SDL_FreeSurface( tmpSrf );
		tmpSrf = NULL;
		throw AppException( string( "SDL: Video mode changing error" ) );
	}
	else
	{
		SDL_BlitSurface(tmpSrf, NULL, mScreen, NULL);
		SDL_Flip(mScreen);
		SDL_FreeSurface(tmpSrf);
		tmpSrf = NULL;
		ClearShockWaveList();
	}
}

//------------------------------------------------------------------------------
//
//		
void	Scene::MainLoop( void )
{
	LoadBackground( "back.bmp" );
	mAnimatedOn = true;
	while( mLoopRunOn )
	{
		SDL_PollEvent( &mEvents );
		if( !mTestModeOn )
		{
			SwitchEvents();
		}
		else
		{
			SwitchTestEvents();
		}
		UpdateScene();
	}
}

//------------------------------------------------------------------------------
//
//
void	Scene::SwitchEvents( void )
{
	if( mEvents.type == SDL_KEYDOWN )
	{

		switch( mEvents.key.keysym.sym )
		{
			case SDLK_ESCAPE:	mLoopRunOn  = false;
								break;
			case SDLK_o:		{
									string BmpPath = "";
									if( LoadBmp( BmpPath ) )
									{
										LoadBackground( BmpPath );
									}
								}
								break;
			case SDLK_UP	:	mWaveAmp += 1;
								break;
			case SDLK_DOWN	:   if( mWaveAmp > 0 ) mWaveAmp -= 1;
								break;
			case SDLK_RIGHT :	mWaveLen += 1;
								break;
			case SDLK_LEFT	:	if( mWaveLen > 1 ) mWaveLen -= 1;
								break;
			case SDLK_EQUALS:	++mThreadsNum;
								omp_set_num_threads( mThreadsNum );
								break;
			case SDLK_MINUS	:	if(  mThreadsNum > 1 )
								{
									--mThreadsNum;
									omp_set_num_threads( mThreadsNum );
								} // if(  mThreadsNum > 1 ).
								break;
			case SDLK_t		:	mTestModeOn = true;
								omp_set_num_threads( 1 );
								break;
		} // Switch.
	}
	else if(mEvents.type == SDL_QUIT)
	{
		mLoopRunOn = false;
	}
}

//------------------------------------------------------------------------------
//
//
void Scene::UpdateScene( void )
{

	SDL_BlitSurface( mBack, NULL, mScreen, NULL );
	Uint32 Time = SDL_GetTicks();
	if( !mTestModeOn )
	{
		if( ( Time - mGenLastTime ) >= mGenTimeStep )
		{
			mGenTimeStep = mGenMinTime + rand() % ( mGenMaxTime - mGenMinTime );
			GenerateShockWave();
			mGenLastTime = Time;
		}
		UpdateShockWaves();
		if( mSurfWaveOn ) UpdateSurfaceWave( Time );
	}
	else
	{
		UpdateTest();
	}
	UpdateText();
	UpdateFPS( Time );
	SDL_Flip(mScreen);
	++mFramesCounter;
}

//------------------------------------------------------------------------------
//
//
void Scene::UpdateSurfaceWave( Uint32 inTime )
{
	if( (inTime - mSurfLastTime) >= mSurfTimeStep )
	{
		mSurfWaveX++;
	}
	SurfaceWave(); 
}

//------------------------------------------------------------------------------
//
//
void	Scene::LoadBackground( string inBackFilename )
{
	SDL_Surface* tmp = SDL_LoadBMP( inBackFilename.c_str() );
	if(!tmp)
	{
		CleanScene();
		throw AppException(
							string( "SDL: Background file \"" )
							+ inBackFilename
							+ " loading error" );
	}
	else
	{
		mBack = SDL_DisplayFormat( tmp ); 
		SDL_FreeSurface( tmp );
		ChangeVideoMode( mBack->w, mBack->h, 24 );
		SDL_BlitSurface( mBack, NULL, mScreen, NULL );
		mRadius = mScreen->h; // Set new shockwave radius as quater of hight;
	}
}

//------------------------------------------------------------------------------
//
//

void	Scene::SurfaceWave( void )
{
	int		EndX  = mSurfWaveX + mWaveLen; // X coordinate barrier. 
	int		EndY  = mScreen->h;			   // Y coordiante barrier.
	int		NewY  = 0;
	Uint32	Pixel = 0;
	double	Step = Pi / mWaveLen;
	double  Angle = 0.0;
	if( EndX >= mScreen->w ) EndX = mScreen->w; 
	
	for( int x = mSurfWaveX; Angle <= Pi && x < mScreen->w; ++x, Angle += Step )
	{
		for( int y = 0; y < EndY; ++y )
		{
			NewY = y - int ( mWaveAmp * sin( Angle ) );				
			Pixel = GetPixel( mBack, x, y );
			if( NewY >= 0 && NewY < mScreen->h )
				PutPixel( mScreen, x, NewY, Pixel );
		}
	}
}

//------------------------------------------------------------------------------
//
//
void    Scene::GenerateShockWave( void )
{
	srand(rand());
	int	GenX = rand() % mScreen->w;
	int GenY = rand() % mScreen->h;
	ShockWave* Wave = new ShockWave( 
		mScreen, GenX, GenY, mWaveAmp, mWaveLen, mRadius );
	mWavesList.push_back( Wave );
}

//------------------------------------------------------------------------------
//
//
void	Scene::UpdateShockWaves( void )
{	
	ShockWaveList::iterator i   = mWavesList.begin();
	ShockWaveList::iterator End = mWavesList.end();
	while( i != End )
	{
		if( !(*i)->mEnd )
		{
			(*i)->Update();
		}
		else
		{
			delete *i;
			*i = NULL;
			ShockWaveList::iterator t = i++;
			mWavesList.erase( t );
			continue;
		}
		++i;
	}
	
}

//------------------------------------------------------------------------------
//
//
void Scene::UpdateText( void )
{
	char Str[100] = { 0 };
	SDL_Rect  Pos = mTextPosition;
#pragma omp parallel
{
	mThreadsNum = omp_get_num_threads();
}
	sprintf( Str, "O              open other BMP", mThreadsNum );
	Pos.y += int ( 1.5f * (float)mFontSize );
	DrawText( Str, Pos );

	sprintf( Str, "T              start performance test", mThreadsNum );
	Pos.y += int ( 1.5f * (float)mFontSize );
	DrawText( Str, Pos );

	sprintf( Str, "+/-            change threads number %d", mThreadsNum );
	Pos.y += int ( 1.5f * (float)mFontSize );
	DrawText( Str, Pos );

	sprintf( Str, "Up/Down  change wave amplitude  %d", mWaveAmp );
	Pos.y += int ( 1.5f * (float)mFontSize );
	DrawText( Str,Pos ); 
	
	sprintf( Str, "Left/Right change wave length       %d", mWaveLen );
	Pos.y += int ( 1.5f * (float)mFontSize );
	DrawText( Str,Pos ); 

	if( mTestModeOn )
	{
		sprintf( Str, "TEST MODE ( press ENTER to stop ) Step %d of %d", mThreadsNum, kTestCount );
		SDL_Surface* Text =	TTF_RenderText_Solid( mFont, Str, mFontColor );
		Pos.x = mScreen->w / 2 - Text->w / 2;
		Pos.y += int ( 1.5f * (float)mFontSize );
		SDL_BlitSurface( Text, NULL, mScreen, &Pos );
		SDL_FreeSurface( Text );
	}
}

//------------------------------------------------------------------------------
//
//
void Scene::DrawText( const char* inText, SDL_Rect& inRect )
{
	SDL_Surface* Text = TTF_RenderText_Solid( mFont, inText, mFontColor );
	if( Text )
	{
		SDL_BlitSurface( Text, NULL, mScreen, &inRect );
		SDL_FreeSurface( Text );
	}
}

//------------------------------------------------------------------------------
//
//
void	Scene::CleanScene( void )
{
	if( mScreen )
	{
		SDL_FreeSurface( mScreen );
		mScreen = NULL;
	}
	
	if( mTestWave )
	{
		delete mTestWave;
		mTestWave = NULL;
	}
	
	if( mBack )
	{
		delete mBack;
		mBack = NULL;
	}
	
	if( mTestResult ) delete [] mTestResult;

	if( mFont )
	{
		TTF_CloseFont( mFont );
		mFont = NULL;
	}
	TTF_Quit();
	SDL_Quit();
}

//------------------------------------------------------------------------------
//
//
void	Scene::ClearShockWaveList( void )
{
	ShockWaveList::iterator i   = mWavesList.begin();
	ShockWaveList::iterator end = mWavesList.end();
	while( i != end )
	{
		if( *i ) delete *i;
		*i = NULL;
		++i;
	}
	mWavesList.clear();
}

//------------------------------------------------------------------------------
//
//
void	Scene::UpdateFPS( Uint32 inTime )
{
	if( ( inTime - mLastFpsTime ) >= 1000 )
	{
		char Str[100]	= { 0 };		
		mCurrentFPS		= mFramesCounter;
		mFramesCounter	= 0;
		mLastFpsTime	= inTime;
		sprintf( Str, "%s<FPS: %u>", mTitle.c_str(), mCurrentFPS );
		SDL_WM_SetCaption( Str, NULL );
	}
}

//------------------------------------------------------------------------------
//
//
void	Scene::UpdateTest( void )
{
	if( !mTestWave )
	{
		if( mThreadsNum <= kTestCount )
		{
			mTestWave = new ShockWave( 
								mScreen, mScreen->w / 2,
								mScreen->h / 2, mWaveAmp,
								mWaveLen, mRadius );
			mTestStart = SDL_GetTicks();
			mTestWave->Update();
		}
		else
		{
			mTestModeOn = false;
			ShowTestResults();
			omp_set_num_threads( 1 );
		}
	}
	else
	{
		if( mTestWave->mEnd )
		{
			mTestResult[mThreadsNum - 1] = SDL_GetTicks() - mTestStart;
			delete mTestWave;
			mTestWave = NULL;
			++mThreadsNum;
			omp_set_num_threads( mThreadsNum );
		}
		else
		{
			mTestWave->Update();
		}
	}
}

//------------------------------------------------------------------------------
//
//
void Scene::SwitchTestEvents( void )
{
	if( mEvents.type == SDL_KEYDOWN )
	{
		if( mEvents.key.keysym.sym == SDLK_RETURN )
		{
			mTestModeOn = false;
			delete mTestWave;
			mTestWave = NULL;
		}
		if( mEvents.key.keysym.sym == SDLK_ESCAPE )
		{
			mLoopRunOn = false;
		}
	}
	if(mEvents.type == SDL_QUIT)
	{
		mLoopRunOn = false;
	}
}

//------------------------------------------------------------------------------
//
//
void	Scene::ArialFontPath( void )
{
	mFontPath = mFontPath + getenv( "SYSTEMROOT" ) + "\\Fonts\\arial.ttf";
	mFontSize = 14;
}

void Scene::ShowTestResults( void )
{
	char	Buff[100];
	string  Result;

	Result  +=  "TEST RESULTS\n"
				"Threads number:\tSpent time (ms):\n";
	Uint32 Min = 0;
	for( int i = 0; i < kTestCount; ++i )
	{
		if( mTestResult[Min] > mTestResult[i] ) Min = i;
		sprintf( Buff,"%d\t\t%u\n", i+1, mTestResult[i] );
		Result += Buff;
	}
	double Percent = 
		double (mTestResult[0] - mTestResult[Min]) / double (mTestResult[0]) * 100.0;
	sprintf( 
			Buff, "The best result was obtained"
				  " using %d threads: %ums\n"
				  "%.2f%% better than single thread.",
			Min+1, mTestResult[Min], Percent );
	Result += Buff;
	MessageBoxA( 
				NULL,
				Result.c_str(),
				"Performance test finished",
				MB_OK | MB_ICONINFORMATION );
}

bool	Scene::LoadBmp( string& outFilename )
{
	char Filter[]	 = "BMP-files\0*.bmp\0All files\0*.*\0\0"; // Filter for BMP-open file dialog
	char Caption[]   = "Select BMP file"; // Caption for open file dialog
	char FileName[MAX_PATH];
	FileName[0]		 = FileName[1] = 0;
	LPOPENFILENAMEA ofn;
	ofn				 = new OPENFILENAMEA;
	ZeroMemory( ofn, sizeof(OPENFILENAME) );
	ofn->lpstrTitle	 = (LPSTR)Caption;
	ofn->lStructSize = sizeof(OPENFILENAME);
	ofn->hInstance	 = NULL;
	ofn->hwndOwner   = NULL;         
	ofn->lpstrFilter = (LPSTR)Filter;
	ofn->lpstrFile	 = (LPSTR)FileName;     
	ofn->Flags		 = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn->nMaxFile	 = sizeof(FileName);
	bool Res		 = (bool) GetOpenFileNameA( ofn );
	if( Res ) outFilename = FileName; 
	return Res;
}

} // namespace ShockWaveApp