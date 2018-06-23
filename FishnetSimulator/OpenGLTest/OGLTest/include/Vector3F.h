//******************************************************************************//
//	Vector3F.h																	//
//																				//
//	Fishnet Simulation program													//
//																				//
//	Maksim	Skorokhod	2011													//	
//																				//
//******************************************************************************//
#ifndef _VECTOR3F_
#define _VECTOR3F_

#include <cmath>

using std::sqrtf;

struct Vector3F
{
	float	mX;
	float	mY;
	float	mZ;

		Vector3F( float inX = 0.0f, float inY = 0.0f, float inZ = 0.0f )
	{
		mX = inX;
		mY = inY;
		mZ = inZ;
	}

	Vector3F operator + ( Vector3F inVect )
	{
		Vector3F Result;
		Result.mX = this->mX + inVect.mX;
		Result.mY = this->mY + inVect.mY;
		Result.mZ = this->mZ + inVect.mZ;

		return Result;
	}

	Vector3F operator - ( Vector3F inVect )
	{
		Vector3F Result;
		Result.mX = this->mX - inVect.mX;
		Result.mY = this->mY - inVect.mY;
		Result.mZ = this->mZ - inVect.mZ;

		return Result;
	}

	Vector3F operator / ( float inValue )
	{
		Vector3F Result;
		Result.mX = this->mX/inValue;
		Result.mY = this->mY/inValue;
		Result.mZ = this->mZ/inValue;

		return Result;
	}

	Vector3F operator * ( float inValue )
	{
		Vector3F Result;
		Result.mX = this->mX*inValue;
		Result.mY = this->mY*inValue;
		Result.mZ = this->mZ*inValue;

		return Result;
	}

	Vector3F& operator = ( Vector3F inVect )
	{
		mX = inVect.mX;
		mY = inVect.mY;
		mZ = inVect.mZ;
		
		return *this;
	}

	Vector3F& operator += ( Vector3F inVect )
	{
		mX += inVect.mX;
		mY += inVect.mY;
		mZ += inVect.mZ;

		return *this;
	}

	Vector3F& operator -= ( Vector3F inVect )
	{
		mX -= inVect.mX;
		mY -= inVect.mY;
		mZ -= inVect.mZ;

		return *this;
	}

	Vector3F& operator *= ( float inValue )
	{
		mX *= inValue;
		mY *= inValue;
		mZ *= inValue;

		return *this;
	}

	Vector3F& operator /= ( float inValue )
	{
		mX /= inValue;
		mY /= inValue;
		mZ /= inValue;

		return *this;
	}

	Vector3F operator -()
	{
		Vector3F Result;
		Result.mX = -mX;
		Result.mY = -mY;
		Result.mZ = -mZ;

		return Result;
	}

	float GetLength( void )
	{
		float Result = sqrtf( mX*mX + mY*mY + mZ*mZ );
		return Result;
	}
};

#endif