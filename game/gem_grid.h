#ifndef GEM_GRID_H_
#define GEM_GRID_H_

#include <vector>
#include <map>
#include <string>
#include <set>
#include <queue>
#include <cassert>

#include "grid.h"
#include "texture.h"

struct Gem;


/// \brief Class - gems manager/container
///        (inherited from regular 2D grid)
/// \detailes GemGrid is Singleton object.
///           Since it is the game engine it must be only one,
///           we don't want to define an instance of it anywhere,
///           and it will be automatically destroyed on exit.
class GemGrid : public Grid
{
//TODO OLGA - write all essentials
    friend Gem;

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

    void update( void );
    
    void mouse(int mouse_x, int mouse_y );
    

private:
    /// \brief Create new gem - on grid init.
    void new_gem(size_t idx);

    /// \brief Check current line for win
    unsigned check_line(int i, int j, int i_inc, int j_inc, 
        unsigned mask, std::set<size_t>* win_idx) const;

    /// \brief Find win lines
    bool find_win_lines(int i, int j, unsigned mask, 
        std::set<size_t>* win_gems) const;

    /// \brief Private default consturctor
    /// \warning Is triggered by get_instance() 
    GemGrid(void);

    /// \brief Disabled Cope constructor and operator =
    GemGrid(const GemGrid&) {}
    GemGrid& operator =(const GemGrid&) {return *this;}

    //::draw_moving(size_t gem_idx, const GemGrid& grid, Gem& itself);
private:
// Private members

    Texture               tex_loader_ ;

    std::vector<Gem*>     gems_       ; // Array of all grid gems
    std::vector<unsigned> gem_masks_  ; // Gem mask/type

    std::set<size_t>      moving_gems_; // Indexes of moving gems
    std::set<size_t>      win_gems_   ;
    std::set<size_t> *    null_gems_  ;
    

};

// Internal Structure
struct Gem
{
    enum GemStatus
    {
        GS_INITIAL = 0,
        GS_MOVING  = 1,
        GS_DONE    = 2
    };
 
    
    GLfloat x_progress_ ;
    GLfloat y_progress_ ;
    GLfloat x_move_ ;
    GLfloat y_move_ ;
    int tex_idx_ ;
    int new_tex_idx_ ;

    void set_moving(GLfloat x_move, GLfloat y_move, int new_tex_idx)
    {
        x_progress_ = 0;
        y_progress_ = 0;
        x_move_ = x_move;
        y_move_ = y_move;
        new_tex_idx_ = new_tex_idx;
        assert(x_move_ * y_move_ == 0); // check that we mov only in one direction
    }
    
    void set_static(int tex_idx)
    {
        x_progress_ = 0;
        y_progress_ = 0;
        x_move_ = -1;
        y_move_ = -1;
        new_tex_idx_ = -1;
        tex_idx_ = tex_idx;
    }
    
    void set_null(void)
    {
        x_progress_ = 0;
        y_progress_ = 0;
        x_move_ = -1;
        y_move_ = -1;
        new_tex_idx_ = -1;
        tex_idx_ = -1;
    }
    
    Gem(size_t tex_idx)
    {
        this->set_static(tex_idx);
    }
    
    GemStatus update( void );

};

//extern std::set<Gem*>   gMovingGems; // Indexes of moving gems



#define GEM_GRID GemGrid::get_instance()

#endif // GEM_GRID_H_
