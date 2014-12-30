#include <iostream>
#include "global_consts.h"
#include "global_opengl.h"


void main_loop( int v )
{
    update();
    display();
    glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, val );
}



int main( int argc, char* args[] )
{
    /* TODO
///////////
    char the_path[256];
    
    getcwd(the_path, 255);
    strcat(the_path, "/");
    strcat(the_path, args[0]);
    
    printf("%s\n", the_path);
//////////
    */
    
    // 1. GL initialization
    
    glutInit( &argc, args );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT ); //TODO fullscreen? may be?
    glutCreateWindow( "kroshking" );

    if( !init() )
    {
        std::cout << "Failed to initialize GL" << std::endl;
        return EXIT_FAILURE;
    }

    
    // 2. Load game assets
    // I.e load textures, init game engine - GemGrid
    
    if( !load_assets() )
    {
        std::cout << "Failed to load game assets" <<std::endl;
        return EXIT_FAILURE; // TODO - exit codes
    }

    
    // 3. Assign call-back-s
    
    glutDisplayFunc( display );
    glutMouseFunc( mouse );
    glutKeyboardFunc( keyboard );

    
    // 4. Set and start main loop
    
    glutTimerFunc( 1000 / SCREEN_FPS, main_loop, 0 );
    glutMainLoop();

    return EXIT_SUCCESS;
}

