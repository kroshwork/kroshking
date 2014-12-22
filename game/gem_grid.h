#ifndef GEM_GRID_H_
#define GEM_GRID_H_

#include "grid.h"
#include <vector>

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

    
    
 



private:

    std::vector<Gem*>       gems_       ; // Array of all grid gems
    std::vector<unsigned>   tex_masks_  ; // Texture mask/ID assigned to gems

    std::vector<size_t>     active_gems_; // Indexes of active gems
    std::vector<size_t>     moving_gems_; // Indexes of moving gems
};

#endif // GEM_GRID_H_
