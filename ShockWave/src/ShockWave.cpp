#include "ShockWave.h"
#include "AppException.h"

using namespace std;

namespace ShockWaveApp
{

//------------------------------------------------------------------------------
//
//
ShockWave::ShockWave(
					 SDL_Surface* inScreen,	// Screen surface to drawing animation.
					 const int inX,			// X position.
					 const int inY,			// Y position.
					 const int inAmplitude, // Amplitude of wave.
					 const int inLegth,		// Length from minimum ti maximum radius for every farme..
					 const int inMaxRadius, // Absolute radius.
					 const Uint32 inTimeStep ) // Animation timestep.
					 :
kX( inX ),
kY( inY ),
kAmplitude( inAmplitude ),
kLength( inLegth ),
kMaxRadius( inMaxRadius )
{
	mScreen    = inScreen;
	mCurRadius = 1;
	mScrCopy   = NULL;
	mTimeStep  = inTimeStep;
	mLastTime  = 0;
	mEnd = false;
}

//------------------------------------------------------------------------------
//	
//
ShockWave::~ShockWave(void)
{
	if( mScrCopy )
	{
		SDL_FreeSurface( mScrCopy );
		mScrCopy = NULL;
	}
}

//------------------------------------------------------------------------------
//	Update parameters and call draw function.
//
void ShockWave::Update()
{
	if( mCurRadius <= kMaxRadius )
	{
		Uint32 Time = SDL_GetTicks();
		
		if( (Time - mLastTime) > mTimeStep )
		{
			mCurRadius++;
			mLastTime = Time;
		}
		CopyScreen();
		Draw();
	}
	else
	{
		mEnd = true;
	}
}

//------------------------------------------------------------------------------
//	Draw animation frame to screen surface.
//
void ShockWave::Draw()
{
	double  Step    = Pi / (double) kLength;
	double  Angle	= 0.0;
	Uint32  Pixel	= 0;
	int		Rad		= 0;
	int		Off		= 0;

#pragma omp parallel for private( Angle, Pixel, Rad, Off )
	for( int i = 0; i <= kLength; ++i )
	{ 
		Rad			= mCurRadius + i;
		Angle		= Step * i; 
		Off			= int( kAmplitude * sin( Angle ) );	// Calculates offset for this angle.
		double CirStep = 0.5 / (double) Rad;		// Calculates the step actual for this radius.

		for( double Circle = 0.0; Circle <= Pi2; Circle += CirStep )
		{
			double Cos = cos( Circle );
			double Sin = sin( Circle );
			int OffRad = Rad - Off;
			int x = kX + int ( OffRad * Cos );
			int y = kY + int ( OffRad * Sin );
			if( x >= 0 && x < mScreen->w &&
				y >= 0 && y < mScreen->h )
				Pixel = GetPixel( mScrCopy, x, y );
			else
				continue;

			x = kX + int ( Rad * Cos );
			y = kY + int ( Rad * Sin );
			if( x >= 0 && x < mScreen->w &&
				y >= 0 && y < mScreen->h )
				PutPixel( mScreen, x, y, Pixel );
		}
	}
}

//------------------------------------------------------------------------------
//
//
void ShockWave::CopyScreen()
{
	if( !mScrCopy )
	{
		SDL_PixelFormat* fmt = mScreen->format; 
		mScrCopy = SDL_CreateRGBSurface(
										mScreen->flags, mScreen->w,
										mScreen->h,		fmt->BitsPerPixel,
										fmt->Rmask,		fmt->Gmask,
										fmt->Bmask,		fmt->Amask );
		if( !mScrCopy )
			throw AppException( string( "SDL ShockWave::CopyScreen(): "
										"Error creating copy surface" ) );
	}

	int Result = SDL_BlitSurface( mScreen, NULL, mScrCopy, NULL );
	if( Result < 0 ) throw AppException( string( "SDL: Error " ) + 
										 SDL_GetError() );
}

} // namespace ShockWaveApp
