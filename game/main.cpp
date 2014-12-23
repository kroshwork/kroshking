#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "global_consts.h"


void main_loop( int val );


int main( int argc, char* args[] )
{
    char the_path[256];
    
    getcwd(the_path, 255);
    strcat(the_path, "/");
    strcat(the_path, args[0]);
    
    printf("%s\n", the_path);

    
    
    //Initialize FreeGLUT
    glutInit( &argc, args );

    //Create Double Buffered Window
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    
    glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
    glutCreateWindow( "OpenGL" );

    //Do post window/context creation initialization
    if( !initGL() )
    {
        printf( "Unable to initialize graphics library!\n" );
        return 1;
    }

    //Load media
    if( !loadMedia() )
    {
        printf( "Unable to load media!\n" );
        return 2;
    }

    //Set rendering function
    glutDisplayFunc( render );
    glutMouseFunc(mouse);

    //Set main loop
    glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, 0 );

    //Start GLUT main loop
    glutMainLoop();

    return 0;
}

void runMainLoop( int val )
{
    //Frame logic
    update();
    render();

    //Run frame one more time
    glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, val );
}
