//******************************************************************************//
//	FishnetSimulator.h															//
//																				//
//	Fishnet Simulation program													//
//																				//
//	Maksim	Skorokhod	2011													//	
//																				//
//******************************************************************************//
#ifndef _FISHNETSIMULATOR_
#define _FISHNETSIMULATOR_

#include <vector>
#include "NodeWithMass.h"
#include "Link.h"

using std::vector;

class	FishnetSimulator
{
		
	public:

		//----------------------------------------------------------------------
		FishnetSimulator( 
							Vector3F inGridPos,
							int inNumOfColumns,			// Number of columns.
							int inNumOfRows,			// Number of rows in fishnet.
							float inNodeMass,			// Mass of every node.
							float inLinkLength,			// Length of link, which connect two node's.		
							float inLinkForce,			// Force which occurs when link stretching.
							float inLinkFrictionForce,	// Link friction force.
							Vector3F inGravity,			// Earth gravity force.
							float inAirFrictionForce	// Air friction force.
						);
		
		//----------------------------------------------------------------------
		~FishnetSimulator( void );
	
		//----------------------------------------------------------------------
		void	AddForces( void );
	
		//----------------------------------------------------------------------
		void	ApplyForces( float inDTime );
		
	private:

		//----------------------------------------------------------------------
		void  CreateNodes( void );
		
		//----------------------------------------------------------------------
		void  CreateLinks( void );

		//----------------------------------------------------------------------
		// Data.
	public:
		int		mNumOfRows;
		int		mNumOfColumns;
		float	mNodeMass;
		float	mLinkLength;								
		float	mLinkForce;
		float	mLinkFrictionForce;
		Vector3F
				mGravity;
		float	mAirFrictionForce;
		vector<NodeWithMass*> mMassArray;
		vector<Link*>		  mLinkArray;
		Vector3F	mGridPos;
		
};

#endif