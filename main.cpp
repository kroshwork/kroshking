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

//Constants
#define SCREEN_X 800
#define SCREEN_Y 600
GLfloat move_loc[4] = {0,0,0,0};


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

const float vertexPositions[12] = {
    0.75f, 0.75f, 0.0f, 1.0f,
    0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f,
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
    
    printf("OpenGL version : %s \n", glGetString(GL_VERSION));
    glutMainLoop();
    

    return 0;
}

