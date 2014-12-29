#ifndef GEM_GRID_H_
#define GEM_GRID_H_

#include <vector>
#include <map>
#include <string>

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
    static GemGrid& get_instace(void);

    /// \brief Load background texture.
    ///        Pass filename to the texture loaded, load texture from the file,
    ///        assign ID, reports about errors
    /// \param fname backgroung file - path+name.png
    /// \return true - on success, false - on failure
    bool load_bg_tex( const std::string fname );
    
    /// \brief Load gem textures.
    /// \param tex_map gem textures map, where key is unsigned mask and value is file name
    /// \return true - on success, false - on failure
    bool load_gems_tex( const std::map<unsigned, std::string>& tex_map );

    /// \brief Draw scene - calculate textures pozition, 
    void draw( void );
    
    
    bool load_assets()
    
/*
    // Parameterized constructor
    GemGrid(int min_x,
            int min_y;
            int len_x,
            int len_y;
            int num_x,
            int num_y);

*/
    
    
    
    
      void GemGrid::generate_gems(void)




private:

    std::vector<Gem*>     gems_       ; // Array of all grid gems
    std::vector<GemMask>  gem_masks_  ; // Gem mask/type
    std::set<size_t>      moving_gems_; // Indexes of moving gems
   
    Texture tex_loader_;

    struct Gem
    {
        int x_;
        int y_;
        int x_new_; // Needed for moving gems only
        int y_new_;
        int w_;
        int h_;
        GLuint tex_id_;

        void (Gem::*draw_ptr_)(void);  

        void draw_null(void);
        void draw_static(void);
        void draw_moving(void);

        Gem(size_t tex_idx);
    };



private:
    // Check current line for win
    unsigned check_line(int i, int j, int i_inc, int j_inc, unsigned mask, std::set<int>* win_idx) const;
    
    // Find win lines
    bool find_win_lines(int i, int j, unsigned mask, std::set<size_t>* win_gems) const;

    // Add new gem to grid on init
    void new_gem(size_t id, const Texture& tex_loader);



};

#define GEM_GRID GemGrid::get_instance() 

#endif // GEM_GRID_H_
