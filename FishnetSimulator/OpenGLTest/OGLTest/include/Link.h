//******************************************************************************//
//	Link.h																	//
//																				//
//	Fishnet Simulation program													//
//																				//
//	Maksim	Skorokhod	2011													//	
//																				//
//******************************************************************************//
#ifndef _LINK_
#define _LINK_

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

#include "Vector3F.h"
#include "NodeWithMass.h"

class Link
{
	// Methods.
	public:
		// Constructor and destructor.
		Link( 
				NodeWithMass* inStartNode,
				NodeWithMass* inEndNode,
				float inLength, 
				float inFrictionForce,
				float inSpringForce )		
		{
			mStartNode = inStartNode;
			mEndNode = inEndNode;
			mLength = inLength;
			mFrictionForce = inFrictionForce;
			mSpringForce = inSpringForce;
		}

		~Link( void )
		{

		}

		// Apply internal forces effect.
		void AddLinkForce( )
		{
			Vector3F LinkVector = mStartNode->mPosition - mEndNode->mPosition;

			float VectorLength = LinkVector.GetLength();										
			Vector3F Force;		

			if ( VectorLength != 0 )
			{
				Force += (LinkVector / VectorLength)*( VectorLength - mLength)*(-mSpringForce);
			}
			Force += -( mStartNode->mVelocity - mEndNode->mVelocity) * mFrictionForce;
			
			mStartNode->AddForce( Force );													
			mEndNode->AddForce( -Force );	
		}

		NodeWithMass* GetStartNode( void )
		{
			return mStartNode;
		}

		NodeWithMass* GetEndNode( void )
		{
			return mEndNode;
		}
	// Data.
	private:
		
		// Nodes.
		NodeWithMass*	mStartNode;	
		NodeWithMass*	mEndNode;

		// Link data.
		float  mLength;
		float  mSpringForce;
		float  mFrictionForce;

		// Time
		float mDt;
};

#endif