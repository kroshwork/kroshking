#include "texture.h"
#include "lodepng.h"

#include <iostream>
#include <algorithm>
#include <cstdio>
#include <stdlib.h>    
#include <time.h>       


//-----------------------------------------------------------------------------
Texture::Texture(void) : id_(0), width_(0), height_(0), mask_(0)
{  }

//-----------------------------------------------------------------------------
Texture::~Texture(void)
{
    this->clear();
}

//-----------------------------------------------------------------------------
int Texture::add ( unsigned mask, const std::string& png_file )
{
    int result = -1;
    std::vector<unsigned char> image; //the raw pixels
    GLuint w = 0, h = 0;

    // decode
    unsigned error = lodepng::decode(image, width, height, path);

    // if there's an error, display it
    if(error)
    {
        std::cerr << "Texture :: decoder error " << error << " : "
                  << lodepng_error_text(error) << std::endl;
    }
    else
    {
        // get Texture ID from OpenGl
        GLuint tex_id = load_texture( (GLuint*)&image[0], w, h );
        if (tex_id > 0)
        {
            this->descr_.push_back( TexDescr( mask, w, h ) );
            this->id_.push_back( tex_id );
            result = this->id_.size() - 1;
        }
        else
        {
            result = -1;
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
GLuint Texture::load_texture( GLuint* img, GLuint w, GLuint h )
{
    GLuint tex_id = 0;
    
    glGenTextures( 1, &tex_id );
    glBindTexture( GL_TEXTURE_2D, mTextureID );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glBindTexture( GL_TEXTURE_2D, NULL );
    
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        tex_id = 0;
        std::cerr << "Texture :: failed to load texture - " << gluErrorString( error ) << std::endl;
    }

    //Successeded
    return tex_id;
}

//-----------------------------------------------------------------------------
void Texture::clear(void)
{
    glDeleteTextures( this->id_.size(), &this->id_ );
    std::for_each( this->id_.begin(), this->id_.end(), [](int &n){ n = 0; } );
    std::for_each( this->id_.begin(), this->id_.end(), [](TexDescr &descr){ descr.clear();} );
}

//-----------------------------------------------------------------------------
unsigned Texture::get_random(void) const
{
    srand (time(NULL));
    // generate secret number between 0 and size of tex array - 1
    unsigned int random_idx = rand() % (this->id.size() - 1);

    std::cout << "Generated tex index = " << random_index << std::endl;

    this->current_tex = random_idx;
    return this->descr_[this->current_tex].mask_;
}

//-----------------------------------------------------------------------------
unsigned Texture::get_next(void) const
{
    this->current_tex = (this->current_tex + 1) % (this->id.size() - 1);
    return this->descr_[this->current_tex].mask_;
}

//-----------------------------------------------------------------------------
GLuint  Texture::get_id    (size_t idx) const { return this->id_[idx]           ; }
GLuint  Texture::get_width (size_t idx) const { return this->descr_[idx].width_ ; }
GLuint  Texture::get_height(size_t idx) const { return this->descr_[idx].height_; }
GemMask Texture::get_mask  (size_t idx) const { return this->descr_[idx].mask_  ; }


