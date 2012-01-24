/**
 * GLUT main app for things.
 */

#include "tools.h"
#include "gameDraw.h"
#include "glDebug.h"

// Global status things.
int key[256];
int specialKey[256];

// Global defines. Could also be extern vars eventually.
#define screenWidth 768.0
#define screenHeight 480.0

// #define screenWidth 1366.0
// #define screenHeight 768.0

// Key press handler, normal keys.
void handleKeypress( unsigned char k, int x, int y ) {
	key[ k ] = 1;
	
	// Some special cases.
	switch( k ) {
		case 27: // Escape
			exit( 0 );
		case 's': // Neat for debugging
			sleep( 1 );
		break;
	}
}
void handleKeyUp( unsigned char k, int x, int y ) {
	key[ k ] = 0;
}

// Key press handler, special keys.
void handleSpecialKeypress( int k, int x, int y ) {
	specialKey[ k ] = 1;
}
void handleSpecialUp( int k, int x, int y ) {
	specialKey[ k ] = 0;
}


// Resize handler.
void handleResize(int w, int h) {
	glutReshapeWindow((int)screenWidth,(int)screenHeight);
	glViewport(0, 0, (int)screenWidth, (int)screenHeight);
	gameResize((int)screenWidth, (int)screenHeight);
}

// Scene drawing
void drawScene() {
	gameDraw();
	glutSwapBuffers();
}

// Called every 10 milliseconds to update things on screen
void update(int value) {
	value=value;
	if( gameUpdate() == 0 ) {
		gameCleanup();
		exit( 1 );
	}

	glutPostRedisplay();
	glutTimerFunc( 10, update, 0 );
}

// Initialization functions.
void initOGL(int argc, char** argv) {
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	
	glutInitContextVersion(3, 2);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	// Build modestring.
	char tmpString[255];
	char modeString[255];
	modeString[0] = 0;
	itoa( (int)screenWidth, tmpString, 10 );
	strcat( modeString, tmpString );
	strcat( modeString, "x" );
	itoa( (int)screenHeight, tmpString, 10 );
	strcat( modeString, tmpString );
	strcat( modeString, ":24" );
	glutGameModeString( modeString );

// 	glutEnterGameMode();

	// Alternative: Windowed.
	glutInitWindowSize((int)screenWidth,(int)screenHeight);
	glutCreateWindow("Stallman Box");
	
	glutSetCursor(GLUT_CURSOR_NONE);

	glewExperimental = GL_TRUE;
	glewInit();

	// Only where supported.
	// attachGLDebug();
}

void initFunctions() {
	memset( key, 0, 256 );
	memset( specialKey, 0, 256 );
	
	glutDisplayFunc( drawScene );
	glutKeyboardFunc( handleKeypress );
	glutKeyboardUpFunc( handleKeyUp );
	glutSpecialFunc( handleSpecialKeypress );
	glutSpecialUpFunc( handleSpecialUp );
	glutReshapeFunc( handleResize );
	glutTimerFunc( 10, update, 0 );
}

int main(int argc, char** argv) {
	initOGL( argc, argv );
	initFunctions();
	
	gameInit();
	gameResize((int)screenWidth, (int)screenHeight);
	
	glutMainLoop();
	
	return 0;
}
