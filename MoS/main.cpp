// experimental branch

#include "sgct.h"
#include <sstream>
#include <iostream>
#include <iomanip>

sgct::Engine * gEngine;

//-----------------------
// function declarations 
//-----------------------
void myInitOGLFun();
void myPreSyncFun();
void myDrawFun();

//for syncing variables across a cluster
void myEncodeFun();
void myDecodeFun();



//-----------------------
// variable declarations 
//-----------------------

sgct::SharedDouble curr_time(0.0);



int main( int argc, char* argv[] )
{
	// Allocate
	gEngine = new sgct::Engine( argc, argv );

	// Bind your functions
	gEngine->setInitOGLFunction( myInitOGLFun );
	gEngine->setPreSyncFunction( myPreSyncFun );
	gEngine->setDrawFunction( myDrawFun );

	// Init the engine
	if( !gEngine->init() )
	{
		delete gEngine;
		return EXIT_FAILURE;
	}

	sgct::SharedData::instance()->setEncodeFunction( myEncodeFun );
	sgct::SharedData::instance()->setDecodeFunction( myDecodeFun );

	// Main loop
	gEngine->render();

	// Clean up
	delete gEngine;

	// Exit program
	exit( EXIT_SUCCESS );
}

void myInitOGLFun()
{


	/*
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_COLOR_MATERIAL );
	glDisable( GL_LIGHTING );
	glEnable( GL_CULL_FACE );
	glEnable( GL_TEXTURE_2D );

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW); //our polygon winding is counter clockwise
	*/


	/*
	if( gEngine->isMaster() )
	{

	}
	*/
}

/*
	This callback is called once per render loop iteration.
*/
void myPreSyncFun()
{

	
	if( gEngine->isMaster() )
	{
		//get the time in seconds
		curr_time.setVal(sgct::Engine::getTime());
	}
}

/*
	This callback can be called several times per render loop iteration.
	Using a single viewport in stereo (3D) usually results in refresh rate of 120 Hz.
*/
void myDrawFun()
{
  
}

void myEncodeFun()
{
	//sgct::SharedData::instance()->writeVector( &sharedTransforms );

}

void myDecodeFun()
{
	//sgct::SharedData::instance()->readVector( &sharedTransforms );

}
