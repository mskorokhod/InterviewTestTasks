#ifndef _SHOCKWAVE_H_
#define _SHOCKWAVE_H_

#include "Common.h"
#include "SDL.h"
#include <cmath>

namespace ShockWaveApp 
{

class ShockWave
{
public:
			 ShockWave( 
						SDL_Surface* inScreen,
						const int	 inX,
						const int    inY,
						const int    inAmplitude,
						const int	 inLegth,
						const int    inMaxRadius,
						const Uint32 inTimeStep = 1
						);
			 ~ShockWave( void );
		void Update();
private:
		// Make local copy of screen before every  drawing function.
		void	CopyScreen();
		void    Draw();
public:
		bool			mEnd;
private:
	// Read-only.
	const int		kAmplitude;	// Amplitude.
	const int		kLength;
	const int		kMaxRadius;
	const int		kX;
	const int		kY;
	// Variables.
	SDL_Surface*	mScreen;
	SDL_Surface*	mScrCopy;
	Uint32			mTimeStep;
	int				mCurRadius;
	int				mLastTime;
};

}
#endif //_SHOCKWAVE_H_