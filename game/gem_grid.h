#ifndef GEM_GRID_H_
#define GEM_GRID_H_

#include <vector>
#include <map>
#include <string>
#include <set>
#include <queue>

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

    /// \brief Generate initial gem grid
    void generate_assets(void);

    /// \brief Draw scene - calculate textures pozition,
    void draw( void ) const; //TODO const/not const??


    void mouse(int mouse_x, int mouse_y );

private:
    /// \brief Create new gem - on grid init.
    void new_gem(size_t idx);

    /// \brief Check current line for win
    unsigned check_line(int i, int j, int i_inc, int j_inc, 
        unsigned mask, std::set<int>* win_idx) const;

    /// \brief Find win lines
    bool find_win_lines(int i, int j, unsigned mask, 
        std::set<int>* win_gems) const;

    /// \brief Private default consturctor
    /// \warning Is triggered by get_instance() 
    GemGrid(void);

    /// \brief Disabled Cope constructor and operator =
    GemGrid(const GemGrid&) {}
    GemGrid& operator =(const GemGrid&) {return *this;}

private:
// Private members

    struct Gem;
    std::vector<Gem*>     gems_       ; // Array of all grid gems
    std::vector<unsigned> gem_masks_  ; // Gem mask/type

    std::queue<size_t>      moving_gems_; // Indexes of moving gems
    std::pair<int, int>     last_touch_;

    Texture               tex_loader_;

// Internal Structures
    struct Gem
    {
        GLfloat x_new_ ;
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
