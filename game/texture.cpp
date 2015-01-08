#include "texture.h"
#include "lodepng.h"

#include <iostream>
#include <algorithm>
#include <cstdio>
#include <stdlib.h>    
#include <time.h>       
//urandom
#include <fcntl.h>
#include <cassert>

//-----------------------------------------------------------------------------
Texture::Texture(void) : current_tex_ (0), descr_(std::vector<TexDescr>())
{

}

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
    unsigned error = lodepng::decode(image, w, h, png_file);

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
            this->descr_.push_back( TexDescr(tex_id, w, h, mask) );
            result = this->descr_.size() - 1;
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
    glBindTexture( GL_TEXTURE_2D, tex_id );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glBindTexture( GL_TEXTURE_2D, 0 );
    
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
    std::for_each( this->descr_.begin(), this->descr_.end(), [](TexDescr &descr){ descr.clear();} );
}

//-----------------------------------------------------------------------------
unsigned Texture::get_random(void) const
{

    unsigned number = 0;
/*
    int fd = ::open("/dev/urandom", O_RDONLY);
    if (fd != -1)
    {
        if (::fread(fd, &number, sizeof(number)) == -1)
        { 
            std::cerr << "Texture :: failed to get random number from /dev/urandom";
            std::cerr << std::endl; 
            number = rand() % (this->descr_.size() - 1) + 1;
        }
        else
        {
            // Successfully received random number - range by [1;size - 1]
            number = number % (this->descr_.size() - 1) + 1;
        }

        if (::close(fd) == -1)
        { 
            std::cerr << "Texture :: failed to close /dev/urandom" << std::endl; 
        }
    }
    else*///TODO FIXME
    {
        std::cerr << "Texture :: failed to open /dev/urandom";
        number = rand() % (this->descr_.size() - 1) + 1;
    }

    std::cout << "Generated tex index = " << number << std::endl;
    assert(number > 0 && number < 6);

    this->current_tex_ = number;
    return this->descr_[number].mask_;
}

//-----------------------------------------------------------------------------
unsigned Texture::get_next(void) const
{
    this->current_tex_ = (this->current_tex_ + 1) % (this->descr_.size());
    this->current_tex_ = (this->current_tex_ == BACKGROUND_IDX) ? 1 : this->current_tex_;
    return this->descr_[this->current_tex_].mask_;
}

// Getters
//-----------------------------------------------------------------------------
GLuint   Texture::get_id    (size_t idx) const { return this->descr_[idx].id_    ; }
GLuint   Texture::get_width (size_t idx) const { return this->descr_[idx].width_ ; }
GLuint   Texture::get_height(size_t idx) const { return this->descr_[idx].height_; }
unsigned Texture::get_mask  (size_t idx) const { return this->descr_[idx].mask_  ; }

GLuint   Texture::get_bg_id    ( void ) const { return this->descr_[BACKGROUND_IDX].id_    ; }
GLuint   Texture::get_bg_width ( void ) const { return this->descr_[BACKGROUND_IDX].width_ ; }
GLuint   Texture::get_bg_height( void ) const { return this->descr_[BACKGROUND_IDX].height_; }

size_t   Texture::get_current( void ) const { return this->current_tex_; }

// TexDescr definitions
//-----------------------------------------------------------------------------
void Texture::TexDescr::clear( void ) 
{   
    glDeleteTextures( 1, &this->id_);
    id_ = 0; width_ = 0; height_ = 0; mask_ = 0; 
}

//-----------------------------------------------------------------------------
void Texture::draw( GLfloat x, GLfloat y, int tex_idx) const
{
    if (tex_idx >= 0 && tex_idx < static_cast<int>(this->descr_.size()))
    {
        ////std::cout << "Drawing index = " << tex_idx << std::endl;
        this->descr_[tex_idx].draw(x, y);
    }
}

//-----------------------------------------------------------------------------
void Texture::TexDescr::draw( GLfloat x, GLfloat y ) const
{
    if (this->id_ != 0)
    {
        glLoadIdentity();
 
        glTranslatef( x, y, 0.f );

        //Set texture ID
        glBindTexture( GL_TEXTURE_2D, this->id_ );

        //Render textured quad
        ////std::cout << "Width  = " << this->width_  << std::endl;
        ////std::cout << "Height = " << this->height_ << std::endl;
        
        glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(         0.f ,           0.f );
            glTexCoord2f( 1.f, 0.f ); glVertex2f( this->width_,           0.f );
            glTexCoord2f( 1.f, 1.f ); glVertex2f( this->width_, this->height_ );
            glTexCoord2f( 0.f, 1.f ); glVertex2f(         0.f , this->height_ );
        glEnd();
    }
}


