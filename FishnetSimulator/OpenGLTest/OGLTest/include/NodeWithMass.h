//******************************************************************************//
//	NodeWithMass.h																//
//																				//
//	Fishnet Simulation program													//
//																				//
//	Maksim	Skorokhod	2011													//	
//																				//
//******************************************************************************//
#ifndef _FISHNETNODE_
#define _FISHNETNODE_

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <assert.h>
#include "Vector3F.h"

class NodeWithMass

{
	// Methods
	public:
		
		// Create node. Set node coordinates and mass which represented in kg.
		NodeWithMass( 
					 float inX,
					 float inY,
					 float inZ,
					 Vector3F inVel,
					 float inMass = 1.0f )
		{
			mPosition.mX = inX;
			mPosition.mY = inY;
			mPosition.mZ = inZ;
			mVelocity = inVel;
			mMass = inMass;
		}

		virtual	~NodeWithMass( void )
		{

		}

		// Add force to current.
		void AddForce( Vector3F inForce )
		{
			mForce += inForce;
		}

		// Set force to zero.
		void ResetForce( void )
		{
			Vector3F T;
			mForce = T;
		}
		
		//Apply forces to change current position and 
		void ApplyForce( float inDTime )
		{
			if( mMass != 0 )
			{
				mVelocity += ( mForce / mMass) * inDTime;
			}	
			mPosition += mVelocity * inDTime;
		}
	//******************************************************************************
	// Data.
	public:

		// Coordinates.
		Vector3F mPosition;

		// Forces.
		Vector3F mForce;
	
		// Mass.
		float mMass;
		
		// Velocity
		Vector3F mVelocity;

};

#endif