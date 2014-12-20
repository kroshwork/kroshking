CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
#krosh - Kroshkina, Olga
#king - King
PROJECT(kroshking)

ADD_DEFINITIONS(-O2 -funsigned-char -Wall -W -Wshadow -Wstrict-prototypes -Wpointer-arith -Winline)

#linux
#ADD_DEFINITIONS(-O2 -funsigned-char -Wall -W -Wshadow -Wpointer-arith -Winline) # -DGL_GLEXT_PROTOTYPES)
#DGL_GLEXT_PROTOTYPES - needs for linux - TODO check it on mac and win

FIND_PACKAGE(GLUT)# REQUIRED)
FIND_PACKAGE(OpenGL)# REQUIRED)
SET(GL_LIBS ${GLUT_LIBRARY} ${OPENGL_LIBRARY})

SET(SRC main.cpp)
ADD_EXECUTABLE(main ${SRC})
TARGET_LINK_LIBRARIES(main {GL_LIBS} m)




#CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
#PROJECT(NEWBIE)
#
#ADD_DEFINITIONS(-O2 -funsigned-char -Wall -W -Wshadow -Wstrict-prototypes -Wpointer-arith -Winline)
#
#FIND_PACKAGE(GLUT)
#FIND_PACKAGE(OpenGL)
#SET(GL_LIBS ${GLUT_LIBRARY} ${OPENGL_LIBRARY})
#
#SET(SRC main.cpp)
#ADD_EXECUTABLE(main ${SRC})
#TARGET_LINK_LIBRARIES(main ${GL_LIBS} m)

