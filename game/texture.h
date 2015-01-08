#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>
#include <vector>

#include "global_opengl.h"

class Texture
{

static const size_t BACKGROUND_IDX = 0;

public:

    /// \brief Init member variables
    Texture();

    /// \brief Destructor
    ~Texture();

    /// \brief Add a new texture, loads it form pgn-file, sets type/mask -- TODO 
    /// \param mask - texture type identifier
    /// \param png_file - png image full file name
    /// \return index or -1 - on error
    int add( unsigned mask, const std::string& png_file );

    /// \brief draw textures
    /// \param x - x centered offset
    /// \param y - y centered offset
    /// \param tex_idx texture index (-1 means - do not draw)
    void draw( GLfloat x, GLfloat y, int tex_idx = static_cast<int>(BACKGROUND_IDX)) const;

    /// \brief Deletes loaded textures, 0-s parameters
    void clear( void );


    // Getters

    /// \brief Get random texture from existing texture array
    unsigned get_random( void ) const;
    
    /// \brief Loop through existing textures
    unsigned get_next( void ) const;

    /// \brief Get current texture index from the array
    /// \return index of the std::vector (loaded gem textures)
    size_t get_current( void ) const;

    /// \brief Get background ID (generated by OpenGL)
    /// \return ID value or 0 - if ID was not generated
    GLuint   get_bg_id    ( void ) const;
    GLuint   get_bg_width ( void ) const;
    GLuint   get_bg_height( void ) const;
    
    GLuint   get_id    ( size_t idx ) const;
    GLuint   get_width ( size_t idx ) const;
    GLuint   get_height( size_t idx ) const;
    unsigned get_mask  ( size_t idx ) const;

private:
    // TODO - write descr - Loads texture from array, OpenGL generates tex. ID
    GLuint load_texture( GLuint* img, GLuint w, GLuint h );

private:

    mutable size_t current_tex_;

    // Textures
    struct TexDescr;
    std::vector<TexDescr> descr_;



    // Structure "describing" each texture
    struct TexDescr
    {
        GLuint   id_    ; // Texture ID - generated by OpenGL
        GLuint   width_ ; // Geom. parameters
        GLuint   height_;
        unsigned mask_  ; // Type mask

        TexDescr( void ) : id_(0), width_(0), height_(0), mask_(0) { }
        TexDescr(GLuint id, GLuint width, GLuint height, unsigned mask ) : 
            id_(id), width_(width), height_(height), mask_(mask) { }
        void clear( void );
        void draw( GLfloat x, GLfloat y ) const;
    };


};


#endif //TEXTURE_H_

