#include "Common.h"

using namespace std;

namespace ShockWaveApp
{

	//------------------------------------------------------------------------------
	//
	//
	Uint32 GetPixel( SDL_Surface *inSurface, int inX, int inY )
	{
		int bpp = inSurface->format->BytesPerPixel;
		// Here p is the address to the pixel we want to retrieve.
		Uint8 *p = (Uint8 *)inSurface->pixels + inY * inSurface->pitch + inX * bpp;

		if( bpp == 3 )
		{
			return p[0] | p[1] << 8 | p[2] << 16;
		}
		else
		{
			throw AppException( string( "SDL: Wrong BytesPerPixel format."
									    "Must be 3 bytes per pixel.")	 );
		}
	}

	//------------------------------------------------------------------------------
	//
	//
	void PutPixel( SDL_Surface *ioSurface, int inX, int inY, Uint32 inPixel )
	{
		int bpp = ioSurface->format->BytesPerPixel;
		// Here p is the address to the pixel we want to set
		Uint8 *p = (Uint8 *)ioSurface->pixels + inY * ioSurface->pitch + inX * bpp;

		if( bpp == 3 )
		{
			p[0] = inPixel & 0xff;
			p[1] = (inPixel >> 8) & 0xff;
			p[2] = (inPixel >> 16) & 0xff;
		}
		else
		{
			throw AppException(string( "SDL: Wrong BytesPerPixel format."
									   "Must be 3 bytes per pixel." )    );
		}
	}

} // namespace ShockWaveApp.
