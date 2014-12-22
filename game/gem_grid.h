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







private:

    std::vector<Gem*>     gems_       ; // Array of all grid gems
    std::vector<GemMask>  gem_masks_  ; // Gem mask/type
    std::set<size_t>      moving_gems_; // Indexes of moving gems
};



struct Gem
{
    unsigned tex_mask_;
    size_t   tex_idx_;


#endif // GEM_GRID_H_
