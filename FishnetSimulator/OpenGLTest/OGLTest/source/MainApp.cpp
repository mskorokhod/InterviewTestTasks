//******************************************************************************//
//	MainApp.cpp																	//
//																				//
//	Fishnet Simulation program													//
//																				//
//	Maksim	Skorokhod	2011													//	
//																				//
//******************************************************************************//
#include "MainApp.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor. inFlag init SDL subsystems.
//
MainApp::MainApp( Uint32 inFlag )
{
	if( SDL_Init( inFlag) < 0 )
		throw new SDLError();
	mSimulator = NULL;
	mFramesPerSecond = 0.0f;
	SDL_WM_SetCaption( "Fishnet Simulation <FPS:0.0>", NULL );
	atexit( SDL_Quit );

}

////////////////////////////////////////////////////////////////////////////////
// Destructor
//
MainApp::~MainApp( void )
{
	if( mSimulator )
	{
		delete mSimulator;
	}
	SDL_Quit();
}

////////////////////////////////////////////////////////////////////////////
// Init simulator.
// 
void	MainApp::InitSimulator( 
	Vector3F inGridPos,
	int inNumOfColumns,			// Number of columns.
	int inNumOfRows,			// Number of rows in fishnet.
	float inNodeMass,			// Mass of every node.
	float inLinkLength,			// Length of link, which connect two node's.		
	float inLinkForce,			// Force which occurs when link stretching.
	float inLinkFrictionForce,	// Link friction force.
	Vector3F inGravity,			// Earth gravity force.
	float inAirFrictionForce )	// Air friction force.
{
		mSimulator = new FishnetSimulator(
								inGridPos,
								inNumOfColumns,
								inNumOfRows,
								inNodeMass,
								inLinkLength,
								inLinkForce,
								inLinkFrictionForce,
								inGravity,
								inAirFrictionForce );

}

////////////////////////////////////////////////////////////////////////////
// Initialization of OpenGL and SDL video subsystem.
//
void MainApp::Init(
				int inWidth,				// Resolution in SDL or OpenGL window.
				int inHeight,				// Width and Height.
				Uint32 inFlags )			// Flags, which init some SDL video mode.
{
	mGLminX = mGLminY = 0.0f;			// Set minimum and maximum coordinates for OpenGL scene.
	mGLmaxX = 2.0f; 
	mGLmaxY = 2.0f;

	// Set base attributes for OpenGL.
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );		// Size of every RGB component in bit's. 
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );	//Set color bit depth.
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	// Init video context for drawing and visualization.
	pScreen = SDL_SetVideoMode( inWidth, inHeight, 24, SDL_OPENGL|inFlags ); // Create video window.
	if( pScreen == NULL )													 // Throw exception if some 
		throw SDLError();													 // video init error has been detected.
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );									 // OpenGL clear color.
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);					 // Clear OpenGL screen.
	glMatrixMode(GL_MODELVIEW);											     // Set matrix mode to GL_MODELVIEW.
	glLoadIdentity();														 // Load empty matrix.
	glOrtho( mGLminX, mGLmaxX, mGLminY, mGLmaxY, -1.0, 1.0 );

}

////////////////////////////////////////////////////////////////////////////////
// Main scene loop. The main part of program.
//
void	MainApp::SceneLoop( void )
{
	mLastTime = SDL_GetTicks();
	char Caption[] = "Fishnet Simulation <FPS:0.0>";

	// Stats for mouse draggy.
	mPixelX = 1.0f/float(pScreen->w)*2.0f;
	mPixelY = 1.0f/float(pScreen->h)*2.0f;
	mAccX = 8.0f*mPixelX;
	mAccY = 8.0f*mPixelY;

	while( 1 )
	{
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear OpenGL screen.
		SDL_PollEvent( &mEvent );
		switch( mEvent.type )
		{
		case SDL_QUIT:	exit(0); break;	
		}
		Uint32 CurentTime = SDL_GetTicks();
		DoSimulate( CurentTime - mLastTime );
		// Frames counter.		
		if( CurentTime - mLastFPSTick >= 1000 )							// If one second left then show new FPS.
		{
			sprintf( Caption,"Fishnet Simulation <FPS:%.1f>", mFramesPerSecond );
			mLastFPSTick = SDL_GetTicks();
			mFramesPerSecond = 0.0f;
			SDL_WM_SetCaption( Caption, NULL );
		}

		mLastTime = CurentTime;
		DrawScene();
		SDL_GL_SwapBuffers();											// Put all scene to screen.
		mFramesPerSecond += 1.0f;
	}
}

////////////////////////////////////////////////////////////////////////////
// This method launch the simulation of FishnetSimulator and update it after
// every call. I used Euler method to simulate process in time.
// inTime means a time in ms between last time counter update and current time .
//
void	MainApp::DoSimulate( Uint32 inTime )
{
	if( mEvent.button.button == 1 )
	{
		int x, y;
		float	glX, glY;
		Vector3F* Pos = NULL;
		SDL_GetMouseState( &x, &y );
		glX = float(x)/float(pScreen->w)*2.0f;
		glY = float(y)/float(pScreen->h)*2.0f;
		vector<NodeWithMass*>::iterator i = mSimulator->mMassArray.begin();
		vector<NodeWithMass*>::iterator end = mSimulator->mMassArray.end();

		for( ; i != end; ++i )
		{
			Pos = &(*i)->mPosition;
			if( (fabs( Pos->mX - glX) < mAccX ) && 
				(fabs( Pos->mY - glY) < mAccY) )
			{
				if( (*i)->mMass > 0.0f )
				{
					(*i)->mVelocity.mX += (glX - Pos->mX)/(*i)->mMass*10.0f;			// Increase the speed of mouse touched node.
					(*i)->mVelocity.mY += (glY - Pos->mY)/(*i)->mMass*10.0f;
					glColor3f( 1.0f, 0.0f, 0.0f );
					glPointSize( 6.0f );
					glBegin( GL_POINTS );
					glVertex3f( Pos->mX, Pos->mY, 0.0f );
					glEnd();
				}
			}
		}
	}

	float DTime = inTime / 1000.0f;			// Convert time presented in ms to seconds.

	float MaxDTime = 0.002f;				// Maximum possible value of DTime

	int Iterations = (int)( DTime / MaxDTime) + 1;
	if( Iterations )
		DTime = DTime / Iterations;
	for( int i = 0; i < Iterations; ++i )
		mSimulator->ApplyForces( DTime );	// Apply forces to simulator.
}

////////////////////////////////////////////////////////////////////////////
// Drawing method. Call it after DoSimulate
//
void	MainApp::DrawScene( void )
{
	glMatrixMode(GL_MODELVIEW);									    // Set matrix mode to GL_MODELVIEW.
	glLoadIdentity();												// Load empty matrix.
	glOrtho( mGLminX, mGLmaxX, mGLmaxY, mGLminY, -1.0, 1.0 );
	vector<Link*>::iterator i = mSimulator->mLinkArray.begin();		// Draw every link and Node from simulator.
	vector<Link*>::iterator end = mSimulator->mLinkArray.end();

	for( ; i != end; ++i )
	{
		NodeWithMass* StartNode = (*i)->GetStartNode();
		Vector3F* StartPos = &StartNode->mPosition;

		NodeWithMass* EndNode = (*i)->GetEndNode();
		Vector3F* EndPos = &EndNode->mPosition;

		glColor3ub( 47, 196, 106 );						// Set color for links lines. I've chose with MS Paint :).
		glLineWidth( 1.0f );
		glBegin( GL_LINES );
		glVertex3f(	StartPos->mX, StartPos->mY, StartPos->mZ);
		glVertex3f( EndPos->mX, EndPos->mY, EndPos->mZ);
		glEnd();

		glColor3ub( 128, 128, 0 );						// Same as upper, but for nodes.
		glPointSize( 4.0f );
		glBegin( GL_POINTS );
		glVertex3f(	StartPos->mX, StartPos->mY, StartPos->mZ );
		glVertex3f( EndPos->mX, EndPos->mY, EndPos->mZ );
		glEnd();
	}
}
