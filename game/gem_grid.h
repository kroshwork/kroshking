#ifndef GEM_GRID_H_
#define GEM_GRID_H_

#include <vector>
#include <map>
#include <string>
#include <set>

#include "grid.h"
#include "texture.h"

/// \brief Class - gems manager/container
///        (inherited from regular 2D grid)
/// \detailes GemGrid is Singleton object.
///           Since it is the game engine it must be only one,
///           we don't want to define an instance of it anywhere,
///           and it will be automatically destroyed on exit.
class GemGrid : public Grid
{
//TODO OLGA - write all essentials

public:

    /// \brief Clean the data, delete the instance
    virtual ~GemGrid(void);

    /// \brief Init (if it was not initialized yet) and return instance of the grid
    static GemGrid& get_instance( void );

    /// \brief Load background texture.
    ///        Pass filename to the texture loaded, load texture from the file,
    ///        assign ID, reports about errors
    /// \param fname backgroung file - path+name.png
    /// \return true - on success, false - on failure
    bool load_bg_tex( const std::string& fname, int x_orig, int y_orig, int w, int h );

    /// \brief Load gem textures.
    /// \param tex_map gem textures map, where key is unsigned mask and value is file name
    /// \return true - on success, false - on failure
    bool load_gems_tex( const std::map<unsigned, std::string>& tex_map );

    /// \brief Draw scene - calculate textures pozition,
    void draw( void ); //TODO const/not const??


/*
    // Parameterized constructor
    GemGrid(int min_x,
            int min_y;
            int len_x,
            int len_y;
            int num_x,
            int num_y);

*/





void generate_assets(void);

void new_gem(size_t idx);



private:


    // Check current line for win
    unsigned check_line(const int i, const int j, const int i_inc, const int j_inc, unsigned mask, std::set<int>* win_idx) const;

    // Find win lines
    bool find_win_lines(const int i, const int j, unsigned mask, std::set<int>* win_gems) const;

    // Add new gem to grid on init
   // void new_gem(size_t id, const Texture& tex_loader);





private:

GemGrid(void);

    struct Gem;
    std::vector<Gem*>     gems_       ; // Array of all grid gems
    std::vector<unsigned> gem_masks_  ; // Gem mask/type
    std::set<size_t>      moving_gems_; // Indexes of moving gems

    Texture tex_loader_;

    struct Gem
    {
        GLfloat x_new_ ; // Needed for moving gems only
        GLfloat y_new_ ;
        size_t tex_idx_;

        void (*draw_ptr_)(GLfloat, GLfloat, size_t, const Texture&);

        static void draw_null  (GLfloat x, GLfloat y, size_t tex_idx, const Texture&);
        static void draw_static(GLfloat x, GLfloat y, size_t tex_idx, const Texture&);
        static void draw_moving(GLfloat x, GLfloat y, size_t tex_idx, const Texture&);

        Gem(size_t tex_idx) : x_new_(-1), y_new_(-1)
        {
            this->tex_idx_ = tex_idx;
            draw_ptr_ = &(GemGrid::Gem::draw_static);
        }
    };




};

#define GEM_GRID GemGrid::get_instance()

#endif // GEM_GRID_H_
