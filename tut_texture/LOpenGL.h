
#ifndef LOPENGL_H
#define LOPENGL_H

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

#endif
