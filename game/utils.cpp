/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 006

#include "LUtil.h"
#include "LTexture.h"

//File loaded texture
LTexture gLoadedTexture;

bool initGL()
{
    //Set the viewport
    glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );

    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Enable texturing
    glEnable( GL_TEXTURE_2D );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    
    return true;
}

bool loadMedia()
{
    //Load texture
    if( !gLoadedTexture.loadTextureFromFile( "/Users/vasya/king_test/kroshking/tut_loading_tex/img/BackGround.png" ) )
    {
        printf( "Unable to load file texture!\n" );
        return false;
    }

    return true;
}

void update()
{
    //TODO here we determin which object is touched
    //and update the displaying data

}

#include <complex>
int gPreviosLoc[2] = {0,0};
static bool gJustMoved = false;
static bool gWasLocationInit = false;

void mouse(int btn, int state, int x, int y)
{
    if (!gWasLocationInit) {
        gWasLocationInit = true;
        gPreviosLoc[0] = x;
        gPreviosLoc[1] = y;
    }
    printf("gJustMoved = %d\n", gJustMoved);
    //_btn in [GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON]
    //_state in [GLUT_UP, GLUT_DOWN]
    if ((state == GLUT_DOWN || state == GLUT_UP) && (btn == GLUT_LEFT_BUTTON || btn == GLUT_RIGHT_BUTTON))
    {
        // Detect moving direction - in our case it may be x or y
        int delta_x = gPreviosLoc[0] - x; //OLGA THIS CODE IS NEEDED FOR THE TEST
        int delta_y = gPreviosLoc[1] - y;
        int abs_delta_x = std::abs(delta_x);
        int abs_delta_y = std::abs(delta_y);
        printf("delta_x = %d\n", delta_x);
        printf("delta_y = %d\n", delta_y);
        if (abs_delta_x > abs_delta_y)
        {
            // move in horizontally
            //if (abs_delta_x >= gLoadedTexture.get_min_mov() && abs_delta_x <= gLoadedTexture.get_max_mov())
            {
                gLoadedTexture.set_mov(delta_x,0);
                gJustMoved = true;
                gPreviosLoc[0] = x;
                gPreviosLoc[1] = y;
            }
            //else
            //{
            //    gJustMoved = false;
            //}
        }
        else if (abs_delta_y > abs_delta_x)
        {
            // move in horizontally
            //if (abs_delta_y >= gLoadedTexture.get_min_mov() && abs_delta_y <= gLoadedTexture.get_max_mov())
            {
                gLoadedTexture.set_mov(0,delta_y);
                gJustMoved = true;
                gPreviosLoc[0] = x;
                gPreviosLoc[1] = y;
            }
            //else
            //{
            //    gJustMoved = false;
            //}
   
        }
        else
        {
            gJustMoved = false;
        }
        
    }
}


void render()
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    //Calculate centered offsets
    GLfloat x = ( SCREEN_WIDTH - gLoadedTexture.textureWidth() ) / 2.f;
    GLfloat y = ( SCREEN_HEIGHT - gLoadedTexture.textureHeight() ) / 2.f;

    //Render texture
    gLoadedTexture.render( x, y );

    //Update screen
    glutSwapBuffers();
}
