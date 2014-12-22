#include "gem_grid.h"



const int 
//-----------------------------------------------------------------------------
GemGrid::GemGrid(int min_x,
                 int min_y;
                 int len_x,
                 int len_y;
                 int num_x,
                 int num_y)
{
    // Explicitly call parent constructor
    Grid::Grid ( min_x,    
                 min_y,  
                 len_x,  
                 len_y,  
                 num_x,  
                 num_y );

    const size_t all_gems = static_cast<size_t>(num_x * num_y);

    // Resize the container so that it contains num_x * num_y 'gems'
    this->gems_.resize(all_gems, NULL);   
    // Set all textures to NONE
    this->gem_masks_.resize(all_gems, TM_NONE);

    // Request that the vector capacity be at least enough to contain num_x * num_y elements
    // We do not know yet what how many gems will be displayed - so just reserve
    this->active_gems_.reserve(all_gems);   
    this->moving_gems_.reserve(all_gems);   
}

//template <int N>
//unsigned foo_x(int i, int j, unsigned mask , int inc
//{
//    return foo_x<N-1>(i, j, this->get_masks[i + inc + this->num_x_ * j], inc + inc);
//}
//
//template <>
//unsigned foo_x<0>(int i, int j, unsigned mask , int inc
//{
//    return mask;
//}


//-----------------------------------------------------------------------------
bool GemGrid::check_x_for_win(int i, int j, unsigned mask) const
{
    unsigned result_left = 0; 
    if (i >= (WIN_LINE_SIZE - 1)) 
    {
        result_left = mask & this->gem_masks[i - 1 + this->num_x_ * j] & this->gem_masks[i - 2 + this->num_x_ * j]; // TODO fix mask calc to generic
    }
    unsigned result_right = 0; 
    if (i <=  (this->num_x_ - WIN_LINE_SIZE)) 
    {
        result_right = mask & this->gem_masks[i + 1 + this->num_x_ * j] & this->gem_masks[i + 2 + this->num_x_ * j];  // TODO fix mask calc to generic    
    }
    return (result_left || result_right);
}


//-----------------------------------------------------------------------------
//TODO add pointer to vector - if it's null dont add, if it's not null collect win lines
bool GemGrid::check_y_for_win(int i, int j, unsigned mask) const
{
    unsigned result_up = 0; 
    if (j >= (WIN_LINE_SIZE - 1)) 
    {
        result_up = mask & this->gem_masks[i + this->num_x_ * (j - 1)] & this->gem_masks[i + this->num_x_ *(j - 2)]; // TODO fix mask calc to generic
    }
    unsigned result_down = 0; 
    if (i <=  (this->num_x_ - WIN_LINE_SIZE)) 
    {
        result_down = mask & this->gem_masks[i + this->num_x_ * (j + 1)] & this->gem_masks[i + this->num_x_ * (j + 2)];  // TODO fix mask calc to generic    
    }
    return (result_up || result_down);
}



//-----------------------------------------------------------------------------
void new_gem(size_t id, const TexLoader& texture)
{
    int i = 0, j = 0;
    this->get_idxs(id, i, j);

    GemMask gem_mask = texture.get_random();
    bool found_in_x = check_x_for_win(i, gem_mask);
    bool found_in_y = check_y_for_win(j, gem_mask);

    while ((gem_mask != GM_NONE) && (found_in_x || found_in_y))
    {
        gem_mask = texture.get_next();
        found_in_x = check_x_for_win(i, j, static_cast<unsigned>(gem_mask));
        found_in_y = check_y_for_win(i, j, static_cast<unsigned>(gem_mask));
    }

    // If good texture was found, create new gem and update mask in mask array
    if (gem_mask != GM_NONE)
    {
        if (this->gems_[id] != NULL)
        {
            delete this->gems_[id];
        }
        this->gems_[id] = new Gem(texture.get_current());
        this->gem_masks_[id] = gem_mask;
    }
    else
    {
        std::cerr << "GemGrid :: failed to create a new gem with ID = " << id << std::endl;
    }
}

    
//-----------------------------------------------------------------------------
void GemGrid::generate(const TexLoader& texture)
{
    const size_t all_gems = this->gems_.size();
    for (size_t i = 0; i < all_gems; ++i)
    {
        this->new_gem(i, texture);
        this->active_gems_.push_back(i);
    }
}







