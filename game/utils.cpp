#include "utils.h"
#include "gem_grid.h"
#include "global_opengl.h"
#include "global_consts.h"

//-----------------------------------------------------------------------------
bool init( void )
{
    // Set the viewport
    glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );

    // Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

    // Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 0.f );

    // Enable texturing
    glEnable( GL_TEXTURE_2D );

    // Check for error
    GLenum err = glGetError();
    bool result = (err == GL_NO_ERROR);
    
    if (!result)
    {
        std::cerr << "utils :: failed to initialize OpenGL, error : ";
        std::cerr << gluErrorString( err ) << std::endl;
    }
    
    return result;
}

//-----------------------------------------------------------------------------
bool load_assets( void )
{

    if ( !GEM_GRID.load_bg_tex( IMG_PATH + std::string("BackGround.png") ) )
    {
        std::cerr << "load_assets :: Failed to load background texture" << std::endl;
        return false;
    }

//    std::map<unsigned, std::string> tex_map = {  );
//    sif ( !GEM_GRID.load_gems_tex(
//                                    {{ GM_BLUE   , { IMG_PATH + std::string( "Blue.png"   ) }},
//                                    {{ GM_GREEN  , { IMG_PATH + std::string( "Green.png"  ) }},
//                                    {{ GM_PURPLE , { IMG_PATH + std::string( "Purple.png" ) }},
//                                    {{ GM_RED    , { IMG_PATH + std::string( "Red.png"    ) }},
//                                    {{ GM_YELLOW , { IMG_PATH + std::string( "Yellow.png" ) }}  ))
//    {
 
    if ( !GEM_GRID.load_gems_tex( {
                                    { GM_BLUE   , IMG_PATH + std::string( "Blue.png"   )},
                                    { GM_GREEN  , IMG_PATH + std::string( "Green.png"  )},
                                    { GM_PURPLE , IMG_PATH + std::string( "Purple.png" )},
                                    { GM_RED    , IMG_PATH + std::string( "Red.png"    )},
                                    { GM_YELLOW , IMG_PATH + std::string( "Yellow.png" )} 
                                    }
                                    ))
    {
        std::cerr << "load_assets :: Failed to load gem textures" << std::endl;
        return false;
    }

    return true;
}

//TODO rescale screen after downloading background - or do something else - just think about it

//-----------------------------------------------------------------------------
void display( void )
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );
    
    //Render texture
    GEM_GRID.draw();
    
    //Update screen
    glutSwapBuffers();
}

//-----------------------------------------------------------------------------
void update( void )
{
    //TODO WRITE ME    
}

//-----------------------------------------------------------------------------
void mouse(int /*btn*/, int /*state*/, int /*x*/, int /*y*/)
{
    //TODO WRITE ME
    
}

//-----------------------------------------------------------------------------
void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
        case 27:
        case 'a':
        {
        //TODO - do exit    glutLeaveMainLoop();
            exit(EXIT_SUCCESS);
            return;
        }
    }
}

// #include <complex>
// int gPreviosLoc[2] = {0,0};
// static bool gJustMoved = false;
// static bool gWasLocationInit = false;
// 
// void mouse(int btn, int state, int x, int y)
// {
//     if (!gWasLocationInit) {
//         gWasLocationInit = true;
//         gPreviosLoc[0] = x;
//         gPreviosLoc[1] = y;
//     }
//     printf("gJustMoved = %d\n", gJustMoved);
//     //_btn in [GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON]
//     //_state in [GLUT_UP, GLUT_DOWN]
//     if ((state == GLUT_DOWN || state == GLUT_UP) && (btn == GLUT_LEFT_BUTTON || btn == GLUT_RIGHT_BUTTON))
//     {
//         // Detect moving direction - in our case it may be x or y
//         int delta_x = gPreviosLoc[0] - x; //OLGA THIS CODE IS NEEDED FOR THE TEST
//         int delta_y = gPreviosLoc[1] - y;
//         int abs_delta_x = std::abs(delta_x);
//         int abs_delta_y = std::abs(delta_y);
//         printf("delta_x = %d\n", delta_x);
//         printf("delta_y = %d\n", delta_y);
//         if (abs_delta_x > abs_delta_y)
//         {
//             // move in horizontally
//             //if (abs_delta_x >= gLoadedTexture.get_min_mov() && abs_delta_x <= gLoadedTexture.get_max_mov())
//             {
//                 gLoadedTexture.set_mov(delta_x,0);
//                 gJustMoved = true;
//                 gPreviosLoc[0] = x;
//                 gPreviosLoc[1] = y;
//             }
//             //else
//             //{
//             //    gJustMoved = false;
//             //}
//         }
//         else if (abs_delta_y > abs_delta_x)
//         {
//             // move in horizontally
//             //if (abs_delta_y >= gLoadedTexture.get_min_mov() && abs_delta_y <= gLoadedTexture.get_max_mov())
//             {
//                 gLoadedTexture.set_mov(0,delta_y);
//                 gJustMoved = true;
//                 gPreviosLoc[0] = x;
//                 gPreviosLoc[1] = y;
//             }
//             //else
//             //{
//             //    gJustMoved = false;
//             //}
//    
//         }
//         else
//         {
//             gJustMoved = false;
//         }
//         
//     }
// }
// 
