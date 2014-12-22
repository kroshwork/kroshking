/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 002

#include "LTexture.h"
#include "lodepng.h"
#include <iostream>
#include <cstdio>

LTexture::LTexture()
{
    //Initialize texture ID
    mTextureID = 0;

    //Initialize texture dimensions
    mTextureWidth = 0;
    mTextureHeight = 0;
    
    set_mov(0,0);
    base[0] = 0;
    base[1] = 0;
    
    
 ///   njInit();

}

LTexture::~LTexture()
{
  ///  njDone();
    //Free texture data if needed
    freeTexture();
}

bool LTexture::loadTextureFromFile( std::string path )
{
    //Texture loading success
    bool textureLoaded = false;
    
//THIS IS WORKING CODE FOR LADING PNG - create util_img.h OLGA
    std::vector<unsigned char> image; //the raw pixels
    unsigned width, height;
    
    //decode
    unsigned error = lodepng::decode(image, width, height, path);
   
    
    //if there's an error, display it
    if(error)
    {
        std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
    else
    {
        textureLoaded = loadTextureFromPixels32( (GLuint*)&image[0], (GLuint)width, (GLuint)height);
    }
//OLGA

    //Report error
    if( !textureLoaded )
    {
        printf( "Unable to load %s\n", path.c_str() );
    }

    return textureLoaded;
}

bool LTexture::loadTextureFromPixels32( GLuint* pixels, GLuint width, GLuint height )
{
    //Free texture if it exists
    freeTexture();

    //Get texture dimensions
    mTextureWidth = width;
    mTextureHeight = height;

    //Generate texture ID
    glGenTextures( 1, &mTextureID );

    //Bind texture ID
    glBindTexture( GL_TEXTURE_2D, mTextureID );

    //Generate texture
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    //Set texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    //Unbind texture
    glBindTexture( GL_TEXTURE_2D, NULL );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error loading texture from %p pixels! %s\n", pixels, gluErrorString( error ) );
        return false;
    }

    return true;
}


void LTexture::freeTexture()
{
    //Delete texture
    if( mTextureID != 0 )
    {
        glDeleteTextures( 1, &mTextureID );
        mTextureID = 0;
    }

    mTextureWidth = 0;
    mTextureHeight = 0;
}

void LTexture::render( GLfloat x, GLfloat y )
{
    //If the texture exists
    if( mTextureID != 0 )
    {
        //Remove any previous transformations
        glLoadIdentity();

        //Move to rendering point
        glTranslatef( 0/*x*/, 0/*y*/, 0.f );

        //Set texture ID
        glBindTexture( GL_TEXTURE_2D, mTextureID );

        //Render textured quad
        printf("mTextureWidth = %d\n", mTextureWidth );
        printf("mTextureHeight = %d\n", mTextureHeight );
        printf("base[0] = %d\n", base[0]);
        printf("base[1] = %d\n", base[1]);
        
        glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(           0.f - base[0],            0.f  - base[1] );
            glTexCoord2f( 1.f, 0.f ); glVertex2f( mTextureWidth - base[0],            0.f  - base[1] );
            glTexCoord2f( 1.f, 1.f ); glVertex2f( mTextureWidth - base[0], mTextureHeight  - base[1] );
            glTexCoord2f( 0.f, 1.f ); glVertex2f(           0.f - base[0], mTextureHeight  - base[1] );
        glEnd();
        /*
        //Render textured quad
        glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(           0.f + mTextureWidth*1.5,            0.f );
            glTexCoord2f( 1.f, 0.f ); glVertex2f( mTextureWidth + mTextureWidth*1.5,            0.f );
            glTexCoord2f( 1.f, 1.f ); glVertex2f( mTextureWidth + mTextureWidth*1.5, mTextureHeight );
            glTexCoord2f( 0.f, 1.f ); glVertex2f(           0.f + mTextureWidth*1.5, mTextureHeight);
        glEnd();
*/
    }
}

GLuint LTexture::getTextureID()
{
    return mTextureID;
}

GLuint LTexture::textureWidth()
{
    return mTextureWidth;
}

GLuint LTexture::textureHeight()
{
    return mTextureHeight;
}


