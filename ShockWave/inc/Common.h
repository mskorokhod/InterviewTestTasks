#ifndef _COMMON_H_
#define _COMMON_H_

#include "AppException.h"
#include <cmath>
#include <cstdlib>
#include <string>
#include <list>
#include <vector>
#include <omp.h>
#include <Windows.h>

#include "SDL.h"
#include "SDL_ttf.h"

namespace ShockWaveApp
{

class	ShockWave;

using	std::cos;
using	std::sin;
using	std::abs;
using   std::list;
using   std::vector;
using   std::rand;

typedef list<ShockWave*> ShockWaveList;

const	int	   kTestCount = 6;
const	double Pi		  = 3.14159265358979323846;
const   double Pi2		  = 2 * Pi;

void	PutPixel( SDL_Surface *ioSurface, int inX, int inY, Uint32 inPixel );

Uint32	GetPixel( SDL_Surface *inSurface, int inX, int inY );

} // 
#endif	//_COMMON_H_