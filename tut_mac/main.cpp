#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <vector>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
//#define __gl_h_
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif


// Consts

const float SCREEN_W = 800; //800 XXX
const float SCREEN_H = 600;
const int SCREEN_FPS = 60;
//Color modes
const int COLOR_MODE_CYAN = 0;
const int COLOR_MODE_MULTI = 1;


// Globals

//The current color rendering mode
int gColorMode = COLOR_MODE_CYAN;
//The projection scale
GLfloat gProjectionScale = 1.f;

// Call-back-s

void main_loop( int val );
bool init( void );
void update( void );
void display( void );
void keyboard( unsigned char key, int x, int y );
void mouse( int btn, int state, int x, int y );


int main( int argc, char* args[] )
{
	glutInit( &argc, args );

	//TUT //Create Double Buffered Window
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);// | GLUT_STENCIL);
    glutInitWindowSize( SCREEN_W, SCREEN_H );
    glutCreateWindow( "krosh_king" ); //argv[0]
    
	// assign rendering call-backs
	glutDisplayFunc( display );
    //glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    glutKeyboardFunc( keyboard );

    // init library
    if( !init() )
    {
        printf( "Unable to init\n" );
        return 1;
    }
    
	// register a timer callback to be triggered in a 1000 / SCREEN_FPS msec
	glutTimerFunc( 1000 / SCREEN_FPS, main_loop, 0 );

	// enters the GLUT event processing loop
	glutMainLoop();

	return EXIT_SUCCESS;
}



/* TUT
 Pre Condition:
 -Initialized freeGLUT
 Post Condition:
 -Calls the main loop functions and sets itself to be called back in 1000 / SCREEN_FPS milliseconds
 Side Effects:
 -Sets glutTimerFunc
 */
void main_loop( int val)
//TODO check if this function is needed or only one function can be used instead
{
    //TUT Frame logic
    update();
    display();
    
    glutTimerFunc( 1000 / SCREEN_FPS, main_loop, val);
}



bool init(void)
{
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_W, SCREEN_H, 0.0, 1.0, -1.0 );
    
    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );
    
    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }
    
    return true;
}

void update(void)
{
    
}

void display(void)
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );
    
    //Reset modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    //Move to center of the screeт
    glTranslatef( SCREEN_W / 2.f, SCREEN_H / 2.f, 0.f );
    
    //Render quad
    if( gColorMode == COLOR_MODE_CYAN )
    {
        //Solid Cyan
        glBegin( GL_QUADS );
        glColor3f( 0.f, 1.f, 1.f );
        glVertex2f( -50.f, -50.f );
        glVertex2f(  50.f, -50.f );
        glVertex2f(  50.f,  50.f );
        glVertex2f( -50.f,  50.f );
        glEnd();
    }
    else
    {
        //RYGB Mix
        glBegin( GL_QUADS );
        glColor3f( 1.f, 0.f, 0.f ); glVertex2f( -50.f, -50.f );
        glColor3f( 1.f, 1.f, 0.f ); glVertex2f(  50.f, -50.f );
        glColor3f( 0.f, 1.f, 0.f ); glVertex2f(  50.f,  50.f );
        glColor3f( 0.f, 0.f, 1.f ); glVertex2f( -50.f,  50.f );
        glEnd();
    }
    
    //Update screen
    glutSwapBuffers();
}



void mouse(int btn, int state, int x, int y)

{
 //   //_btn in [GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON]
 //   //_state in [GLUT_UP, GLUT_DOWN]
 //   if (_state == GLUT_DOWN && (_btn == GLUT_LEFT_BUTTON || _btn == GLUT_RIGHT_BUTTON))
 //   {
 //       move_loc[0] = static_cast<GLfloat>(_x) * 2.0 / static_cast<GLfloat>(SCREEN_X) - 1.0;
 //       move_loc[1] = static_cast<GLfloat>(_y) * (-2.0) / static_cast<GLfloat>(SCREEN_Y) + 1.0;
 //   }
}


void keyboard( unsigned char key, int /*_x*/, int /*_y*/ )
{
    //If the user presses q
    if( key == 'a' )
    {
        //Toggle color mode
        if( gColorMode == COLOR_MODE_CYAN )
        {
            gColorMode = COLOR_MODE_MULTI;
        }
        else
        {
            gColorMode = COLOR_MODE_CYAN;
        }
    }
    else if( key == 'e' )
    {
        //Cycle through projection scales
        if( gProjectionScale == 1.f )
        {
            //Zoom out
            gProjectionScale = 2.f;
        }
        else if( gProjectionScale == 2.f )
        {
            //Zoom in
            gProjectionScale = 0.5f;
        }
        else if( gProjectionScale == 0.5f )
        {
            //Regular zoom
            gProjectionScale = 1.f;
        }
        
        //Update projection matrix
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho( 0.0, SCREEN_W * gProjectionScale, SCREEN_H * gProjectionScale, 0.0, 1.0, -1.0 );
    }
    else if( key == 'q' )
    {
        exit(EXIT_SUCCESS);
    }
}



















/*#include <errno.h>
 #include <math.h>
 #include <stdio.h>
 #include <string>
 #include <algorithm>
 #include <vector>
 #include <stdlib.h>
 
 #ifdef __APPLE__
 #include <OpenGL/gl3.h>
 //#define __gl_h_
 #include <OpenGL/gl.h>
 #include <OpenGL/glu.h>
 #include <GLUT/glut.h>
 #else
 #include <GL/gl.h>
 #include <GL/glu.h>
 #include <GL/glut.h>
 #endif
 
 
 GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile)
 {
 GLuint shader = glCreateShader(eShaderType);
 if (shader == 0)
 {
 //TODO - proizoshlo adskoe zlo!!! return
 }
 const char *chShader = strShaderFile.c_str();
 glShaderSource(shader, 1, &chShader, NULL);
 
 glCompileShader(shader);
 
 GLint status;
 glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
 if (status == GL_FALSE)
 {
 GLint infoLogLength;
 glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
 
 GLchar *strInfoLog = new GLchar[infoLogLength + 1];
 glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
 
 const char *strShaderType = NULL;
 switch(eShaderType)
 {
 case GL_VERTEX_SHADER: {strShaderType = "vertex"; break;}
 case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
 case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
 }
 
 fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
 delete[] strInfoLog;
 }
 
 return shader;
 }
 
 GLuint CreateProgram(const std::vector<GLuint> &shaderList)
 {
 GLuint program = glCreateProgram();
 
 for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
 glAttachShader(program, shaderList[iLoop]);
 
 glLinkProgram(program);
 
 GLint status;
 glGetProgramiv (program, GL_LINK_STATUS, &status);
 if (status == GL_FALSE)
 {
 GLint infoLogLength;
 glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
 
 GLchar *strInfoLog = new GLchar[infoLogLength + 1];
 glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
 fprintf(stderr, "Linker failure: %s\n", strInfoLog);
 delete[] strInfoLog;
 }
 
 for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
 glDetachShader(program, shaderList[iLoop]);
 
 return program;
 }
 
 GLuint theProgram;
 
 const std::string strVertexShader(
 //"#version 100\n"
 "uniform vec4 move;\n"
 "void main()\n"
 "{\n"
 "   gl_Position = gl_Vertex + move;\n"
 //"   gl_Position.x /= 2.0;\n"
 "}\n"
 );
 
 const std::string strFragmentShader(
 //"#version 100\n"
 "void main()\n"
 "{\n"
 "   gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);\n"
 "}\n"
 );
 
 void InitializeProgram()
 {
 std::vector<GLuint> shaderList;
 
 shaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
 shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, strFragmentShader));
 
 theProgram = CreateProgram(shaderList);
 
 std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
 }
 
 const float SCREEN_X = 600; //800 XXX
 const float SCREEN_Y = 600;
 
 
const float vertexPositions[12] = {
    0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.0f, 0.0f, 1.0f,
    0.0f, -0.5f, 0.0f, 1.0f,
};

GLuint positionBufferObject;
GLuint vao;


void InitializeVertexBuffer()
{
    glGenBuffers(1, &positionBufferObject);
    
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
    printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
    
    // Set background color - black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    InitializeProgram();
    InitializeVertexBuffer();
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

GLfloat move_loc[4] = {0,0,0,0};

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void display()
{
    // See glClearColor values
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(theProgram);
    
    GLuint move_idx = glGetUniformLocation(theProgram, "move");
    printf("\n move_idx = %d \n", move_idx);
    
    glUniform4fv(move_idx, 1, move_loc);
    
    //Enable vertex arrays
    glEnableClientState( GL_VERTEX_ARRAY );
    
    //Set vertex data
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glVertexPointer( 4, GL_FLOAT, 0, NULL);
    
    GLfloat data[12];
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPositions), (void*)data);
    for (int i = 0; i < 12; ++i)
    {
        printf("xxx data[%d] = %f\n", i, data[i]);
    }
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glUseProgram(0);
    
    glutSwapBuffers();
    
    //TODO calculate sleep time - estimate the previos code and add desired time
    glutPostRedisplay();
}

//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshape (int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

//Called whenever a key on the keyboard was pressed.
//The key is given by the ''key'' parameter, which is in ASCII.
//It's often a good idea to have the escape key (ASCII value 27) call glutLeaveMainLoop() to
//exit the program.
void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
        case 27:
        {
            //TODO - do exit    glutLeaveMainLoop();
            exit(0);
            return;
        }
    }
}

void mouse(int _btn, int _state, int _x, int _y)
{
    //_btn in [GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON]
    //_state in [GLUT_UP, GLUT_DOWN]
    if (_state == GLUT_DOWN && (_btn == GLUT_LEFT_BUTTON || _btn == GLUT_RIGHT_BUTTON))
    {
        move_loc[0] = static_cast<GLfloat>(_x) * 2.0 / static_cast<GLfloat>(SCREEN_X) - 1.0;
        move_loc[1] = static_cast<GLfloat>(_y) * (-2.0) / static_cast<GLfloat>(SCREEN_Y) + 1.0;
    }
}

unsigned int defaults(unsigned int displayMode, int &width, int &height) {return displayMode;}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);// | GLUT_STENCIL);
    glutInitWindowSize(SCREEN_X, SCREEN_Y);
    glutInitWindowPosition(200, 100);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    init();
    glutMainLoop();
    return 0;
}

*/
