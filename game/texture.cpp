#include "texture.h"
#include "lodepng.h"
#include <iostream>
#include <algorithm>
#include <cstdio>

//-----------------------------------------------------------------------------
Texture::Texture(void) : id_(0), width_(0), height_(0), mask_(GM_NONE)
{  }

//-----------------------------------------------------------------------------
Texture::~Texture(void)
{
    this->clear();
}

//-----------------------------------------------------------------------------
int Texture::add ( GemMask mask, const std::string& png_file )
{
    std::vector<unsigned char> image; //the raw pixels
    GLuint w = 0, h = 0;

    //decode
    unsigned error = lodepng::decode(image, width, height, path);

    //if there's an error, display it
    if(error)
    {
        std::cerr << "Texture :: decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
    else
    {
        // loading result
        bool result = load_texture( (GLuint*)&image[0], w, h);
        if (result)
        {
            this->descr_.push_back(TexDescr(mask, w, h))


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



}

//-----------------------------------------------------------------------------
void Texture::clear(void)
{
    glDeleteTextures(this->id_.size(), &this->id_);
    std::for_each(this->id_.begin(), this->id_.end(), [](int &n){ n = 0; });
    std::for_each(this->id_.begin(), this->id_.end(), [](TexDescr &descr){ descr.clear();});
}

//-----------------------------------------------------------------------------
GLuint  Texture::get_id    (size_t idx) const { return this->id_[idx]    ; }
GLuint  Texture::get_width (size_t idx) const { return this->descr_[idx].width_ ; }
GLuint  Texture::get_height(size_t idx) const { return this->descr_[idx].height_; }
GemMask Texture::get_mask  (size_t idx) const { return this->descr_[idx].mask_  ; }


