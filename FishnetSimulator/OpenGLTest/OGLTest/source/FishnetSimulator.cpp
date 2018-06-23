//******************************************************************************//
//	FishnetSimulator.cpp														//
//																				//
//	Fishnet Simulation program													//
//																				//
//	Maksim	Skorokhod	2011													//	
//																				//
//******************************************************************************//
#include	"FishnetSimulator.h"

////////////////////////////////////////////////////////////////////////////////
//	Constructor
//
FishnetSimulator::FishnetSimulator( 
						Vector3F inGridPos,
						int inNumOfColumns,			// Number of columns.
						int inNumOfRows,			// Number of rows in fishnet.
						float inNodeMass,			// Mass of every node.
						float inLinkLength,			// Length of link, which connect two node's.		
						float inLinkForce,			// Force which occurs when link stretching.
						float inLinkFrictionForce,	// Link friction force.
						Vector3F inGravity,			// Earth gravity force.
						float inAirFrictionForce	// Air friction force.
						)
{
	mGridPos = inGridPos;
	mNumOfRows = inNumOfRows;
	mNumOfColumns = inNumOfColumns;
	mNodeMass = inNodeMass;
	mLinkLength = inLinkLength;								
	mLinkForce = inLinkForce;
	mLinkFrictionForce = inLinkFrictionForce;
	mGravity = inGravity;
	mAirFrictionForce = inAirFrictionForce;
	CreateNodes();
	CreateLinks();
}		

////////////////////////////////////////////////////////////////////////////////
// Destructor.
//
FishnetSimulator::~FishnetSimulator( void )
{
	vector<NodeWithMass*>::iterator NodeI = mMassArray.begin();
	vector<NodeWithMass*>::iterator NodeEnd = mMassArray.end();
	for( ; NodeI != NodeEnd; ++NodeI )							// Freed memory from nodes.
	{
		delete *NodeI;
		*NodeI = NULL;
	}

	vector<Link*>::iterator LinkI = mLinkArray.begin();
	vector<Link*>::iterator LinkEnd = mLinkArray.end();
	for( ; LinkI < LinkEnd; ++LinkI )							// Freed memory from links.
	{
		delete *LinkI;
		*LinkI = NULL;
	}
}		


////////////////////////////////////////////////////////////////////////////////
// Add forces to every node.
//
void	FishnetSimulator::AddForces( void )		
{
	vector<Link*>::iterator LinkI = mLinkArray.begin();
	vector<Link*>::iterator LinkEnd = mLinkArray.end();

	for ( ; LinkI != LinkEnd; ++LinkI )
	{
		(*LinkI)->AddLinkForce();									// Add every link force to node whit was connected to this link 
	}

	vector<NodeWithMass*>::iterator NodeI = mMassArray.begin();
	vector<NodeWithMass*>::iterator NodeEnd = mMassArray.end();
	for( ; NodeI != NodeEnd; ++NodeI )
	{																// Add global forces to every node.
		(*NodeI)->AddForce( mGravity*(*NodeI)->mMass );					
		(*NodeI)->AddForce( -(*NodeI)->mVelocity*mAirFrictionForce );
	}
}		

////////////////////////////////////////////////////////////////////////////////
// Do force effect to velocity.
//
void	FishnetSimulator::ApplyForces( float inDTime )
{
	vector<NodeWithMass*>::iterator i = mMassArray.begin();
	vector<NodeWithMass*>::iterator end = mMassArray.end();
	for( ; i != end; ++i )											// Set previously added forces to zero.
	{
		(*i)->ResetForce();
	}
	AddForces();													// Add all forces to every object.

	i = mMassArray.begin();
	end = mMassArray.end();
			
	for( ; i != end; ++i )											// Do effect forces to nodes and links.
	{
		(*i)->ApplyForce( inDTime );
	}
}

////////////////////////////////////////////////////////////////////////////////
// Create fishnet node
//
void  FishnetSimulator::CreateNodes( void )
{
		Vector3F Vel;
		float x;
		float y;
		y = mGridPos.mY;
		for( int i = 0; i < mNumOfRows; ++i )
		{
			x = mGridPos.mX;
			for( int j = 0; j < mNumOfColumns; ++j )
			{
				mMassArray.push_back( new NodeWithMass( x, y, 0.0f, Vel, mNodeMass ) );
				x += mLinkLength; 
			}
			y += mLinkLength;  
		}
		mMassArray[0]->mMass = 0.0f;
		mMassArray[mNumOfColumns-1]->mMass = 0.0f;
}

////////////////////////////////////////////////////////////////////////////////
// Create links between nodes.
//
void FishnetSimulator::CreateLinks( void )
{
	for( int i = 0; i < mNumOfRows; ++i )
	{
		for( int j = 0; j < mNumOfColumns - 1; ++j )						// Create links between columns.
		{
			mLinkArray.push_back( new Link( 
										mMassArray[i*mNumOfColumns+j],
										mMassArray[i*mNumOfColumns+j+1],
										mLinkLength,
										mLinkFrictionForce,
										mLinkForce ) );
		}
	}
	for( int i = 1; i < mNumOfRows; ++i )									// Create links between rows.
	{
		for( int j = 0; j < mNumOfColumns; ++j )
		{
			mLinkArray.push_back( new Link( 
								mMassArray[(i-1)*mNumOfColumns+j],
								mMassArray[i*mNumOfColumns+j],
								mLinkLength,
								mLinkFrictionForce,
								mLinkForce ) );
					
		}
	}
	
}
