#ifndef GEM_GRID_H_
#define GEM_GRID_H_

#include <vector>

#include "grid.h"
#include "tex_loader.h"

// Class - gems manager/container
// (inherited from regular 2D grid)
class GemGrid : public Grid
{
//TODO OLGA - write all essentials

public:

    // Delete data
    virtual ~GemGrid(void);

    // Parameterized constructor
    GemGrid(int min_x,
            int min_y;
            int len_x,
            int len_y;
            int num_x,
            int num_y);


    load_media

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



#endif // GEM_GRID_H_
