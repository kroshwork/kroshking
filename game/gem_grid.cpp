#include "gem_grid.h"
#include "global_opengl.h"
#include "global_consts.h"
#include <iostream>
#include <unistd.h>
#include <cassert>

//-----------------------------------------------------------------------------
GemGrid::~GemGrid(void)
{
    // TODO

}

//-----------------------------------------------------------------------------
GemGrid::GemGrid(void) : Grid()
{
    this->num_x_ = GRID_X;
    this->num_y_ = GRID_Y;
}

//-----------------------------------------------------------------------------
GemGrid& GemGrid::get_instance( void )
{
    static GemGrid instance;
    return instance;
}

//-----------------------------------------------------------------------------
bool GemGrid::load_bg_tex( const std::string& fname, int x, int y, int w, int h )
{
    bool result = true;
    if (x <= 0)
    {
        std::cerr << "GemGrid :: wrong grid value 'x'" << std::endl;
        result = false;
    }
    if (y <= 0)
    {
        std::cerr << "GemGrid :: wrong grid value 'y'" << std::endl;
        result = false;
    }
    if (w <= 0)
    {
        std::cerr << "GemGrid :: wrong grid value 'w'" << std::endl;
        result = false;
    }
    if (h <= 0)
    {
        std::cerr << "GemGrid :: wrong grid value 'h'" << std::endl;
        result = false;
    }
    // Init gems grid
    this->init( x,
                y,
                floor((float)w / (float)this->num_x_),
                floor((float)h / (float)this->num_y_),
                this->num_x_,
                this->num_y_ );


    result = (this->tex_loader_.add(GM_NONE, fname) != -1);


    return result;
}

//-----------------------------------------------------------------------------
bool GemGrid::load_gems_tex( const std::map<unsigned, std::string>& tex_map )
{
    bool result = true;
    std::map<unsigned, std::string>::const_iterator it = tex_map.cbegin();
    std::map<unsigned, std::string>::const_iterator end = tex_map.cend();
    for (; it != end; ++it)
    {
        if (this->tex_loader_.add(it->first, it->second) < 0)
        {
            std::cerr << "GemGrid :: unable to load texture from " << it->second << std::endl;
            result = false;
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
void GemGrid::draw( void ) const
{
    // draw background
    const GLuint w = this->tex_loader_.get_bg_width ();
    const GLuint h = this->tex_loader_.get_bg_height();
    GLfloat x = ( SCREEN_WIDTH  - w ) / 2.f;
    GLfloat y = ( SCREEN_HEIGHT - h ) / 2.f;
    this->tex_loader_.draw(x, y);

    // draw gems
    const size_t all_gems = this->num_x_ * this->num_y_;
    for (size_t i = 0; i < all_gems; ++i)
    {
        this->gems_[i]->draw_ptr_(i, *this, *this->gems_[i]); // Call corresponding draw-method
    }
}

//-----------------------------------------------------------------------------
//void Gem::draw_null  (GLfloat , GLfloat, Gem&, const Texture&) { return; }
void Gem::draw_null( size_t gem_idx, const GemGrid& grid, Gem& itself ) { return; }

//-----------------------------------------------------------------------------
//void Gem::draw_static(GLfloat x, GLfloat y, Gem& gem, const Texture& tex_loader)
void Gem::draw_static( size_t gem_idx, const GemGrid& grid, Gem& itself )
{
    GLfloat x = (GLfloat)grid.get_x(gem_idx);
    GLfloat y = (GLfloat)grid.get_y(gem_idx);
    grid.tex_loader_.draw(x, y, itself.tex_idx_);
    return;
}

//-----------------------------------------------------------------------------
void Gem::draw_moving( size_t gem_idx, const GemGrid& grid, Gem& itself )
{
    static GLfloat step = 0;
    std::cout << "Hi from MOVING gems" << std::endl;
    // WARNING!! one of 'move' variables (x or y) is always 0!!
    step = (itself.y_move_ + itself.x_move_) / static_cast<GLfloat>(SCREEN_FPS * 0.25);
    
    if (fabs(itself.x_progress_ + itself.y_progress_) < fabs(itself.y_move_ + itself.x_move_))
    {
        if (itself.x_move_ > 0)
            itself.x_progress_ += step;
        else
            itself.y_progress_ += step;
        
        GLfloat x = (GLfloat)grid.get_x(gem_idx);
        GLfloat y = (GLfloat)grid.get_y(gem_idx);
        
        grid.tex_loader_.draw(x + itself.x_progress_, y + itself.y_progress_, itself.tex_idx_);
    }
    else
    {
        std::set<size_t>::const_iterator it = grid.moving_gems_.find(gem_idx);
        assert(it != grid.moving_gems_.end());
        itself.set_static(itself.new_tex_idx_);
    }
    return;
}

//-----------------------------------------------------------------------------
void  GemGrid::mouse(int mouse_x, int mouse_y )
{
    int gem_idx = this->get_idx(static_cast<float>(mouse_x), static_cast<float>(mouse_y));
    if (gem_idx == -1)
    {
        std::cout << "Mouse did not touch gems" << std::endl;
        return;
    }
    
    std::cout << "TOUCHED GEM # " << gem_idx << std::endl;

    // Initial mouse touch
    if (this->moving_gems_.size() == 0)
    {
        this->moving_gems_.insert(static_cast<size_t>(gem_idx));
    }
    // Mouse was already pressed at least once before
    else
    {
        size_t prev_gem_idx = *(this->moving_gems_.begin());
        if (static_cast<size_t>(gem_idx) == prev_gem_idx)
        {
            this->moving_gems_.clear();
            std::cout << "Two touches into the same gem (moving_gems_ vector cleared)" <<std::endl;
            return;
            
        }
        std::set<int> ad_gems;
        this->get_ad(prev_gem_idx, ad_gems);
        if (ad_gems.find(gem_idx) != ad_gems.end())
        {
            
            // grid indexes of the current touched gem
            int cur_i = -1, cur_j = -1;
            this->get_ij(gem_idx, cur_i, cur_j);
            unsigned cur_mask = this->gem_masks_[gem_idx];
            
            // grid indexes of the previous touched gem
            int prev_i = -1, prev_j = -1;
            this->get_ij(prev_gem_idx, prev_i, prev_j);
            unsigned prev_mask = this->gem_masks_[prev_gem_idx];
            
            
            bool win_found = false;
            this->win_gems_.clear();
            // will we win if current gem will be moved to the previous position?
            if ( this->find_win_lines( prev_i, prev_j, cur_mask, &this->win_gems_) )
            {
                win_found = true;
                std::cout << "Yeii we won some lines (CHECK 1)" << std::endl;
            }
            
            if ( this->find_win_lines( cur_i, cur_j, prev_mask, &this->win_gems_) )
            {
                win_found = true;
                std::cout << "Yeii we won some lines (CHECK 2)" << std::endl;
            }
            
            if (win_found)
            {
                // add the second gem to the moving queue, set them new coordinates
                this->moving_gems_.insert(static_cast<size_t>(gem_idx));
                
                // set gems as moving
                this->gems_[gem_idx]->set_moving( this->get_x(prev_gem_idx) - this->get_x(gem_idx),
                                                  this->get_y(prev_gem_idx) - this->get_y(gem_idx),
                                                  this->gems_[prev_gem_idx]->tex_idx_);
                
                this->gems_[prev_gem_idx]->set_moving( this->get_x(gem_idx) - this->get_x(prev_gem_idx),
                                                       this->get_y(gem_idx) - this->get_y(prev_gem_idx),
                                                       this->gems_[gem_idx]->tex_idx_);
            }
            else
            {
                this->moving_gems_.clear();
            }
        
        }

    }
    
}

//-----------------------------------------------------------------------------
unsigned GemGrid::check_line(const int i, const int j, const int i_inc, const int j_inc,
                            const unsigned mask, std::set<size_t>* win_idx) const
{
// TODO ? change to use recursion
    unsigned res = mask;

    // Create look-up indexes array
    static std::vector<int> idx_vec;
    idx_vec.reserve(WIN_LINE_SIZE);
    idx_vec.clear();
    for (int n = 1; n < WIN_LINE_SIZE; ++n)
    {
        idx_vec.push_back(i + i_inc * n + this->num_x_ * (j + j_inc * n));
    }
    // Get masks
    for (size_t n = 0; n < idx_vec.size(); ++n)
    {
        res &= static_cast<unsigned>(this->gem_masks_[idx_vec[n]]);
    }

    //TODO CHECK IT
//    std::cout << std::endl << ">>> CHECK POINT <<< GemGrid :: check_line result = " << res << std::endl;

    if (res > 0 && win_idx != NULL)
    {
        for_each (idx_vec.begin(), idx_vec.end(), [&](int n)
                    {
                        win_idx->insert(n);
                        return res;
                    });
        std::cout << "WON idxs:";
        std::cout << std::endl;
        for_each (win_idx->begin(), win_idx->end(), [](int n) {std::cout << n << '\t';});
        std::cout << std::endl;
        //TODO CHECK IT
    }
    return res;
}

//-----------------------------------------------------------------------------
bool GemGrid::find_win_lines(const int i, const int j, const unsigned mask, std::set<size_t>* win_gems) const
{
    if (win_gems != NULL)
    {
        win_gems->clear();
    }

    // Check left horizontal line
    unsigned res_left = 0;
    if (i >= (WIN_LINE_SIZE - 1))
    {
        res_left = this->check_line(i, j, -1, 0, mask, win_gems);
    }

    // Check right horizontal line
    unsigned res_right = 0;
    if (i <=  (this->num_x_ - WIN_LINE_SIZE))
    {
        res_right = this->check_line(i, j, 1, 0, mask, win_gems);
    }

    // Check top vertical
    unsigned res_up = 0;
    if (j >= (WIN_LINE_SIZE - 1))
    {
        res_up = this->check_line(i, j, 0, -1, mask, win_gems);
    }

    // Check bottom vertical
    unsigned res_down = 0;
    if (i <=  (this->num_x_ - WIN_LINE_SIZE))
    {
        res_down = this->check_line(i, j, 0, 1, mask, win_gems);
    }

    return res_up | res_down | res_left | res_right;
}

//-----------------------------------------------------------------------------
void GemGrid::new_gem(size_t idx)
{
    int i = 0, j = 0;
    this->get_ij(idx, i, j);
   
    unsigned gem_mask = this->tex_loader_.get_random();
    bool lines_found = this->find_win_lines(i, j, gem_mask, NULL);

    while (lines_found) // TODO - additional stop criteria might be needed
    {
        gem_mask = this->tex_loader_.get_next();
        lines_found = this->find_win_lines(i, j, gem_mask, NULL);
    }

    // If fine texture was found, create new gem and update mask in mask array
    if (gem_mask != 0)
    {
        if (this->gems_[idx] != NULL)
        {
            delete this->gems_[idx];
        }
        this->gems_[idx] = new Gem(this->tex_loader_.get_current());
        this->gem_masks_[idx] = gem_mask;
    }
    else
    {
        std::cerr << "GemGrid :: failed to create a new gem with index = " << idx << std::endl;
    }
}

//-----------------------------------------------------------------------------
void GemGrid::generate_assets(void)
{
    const size_t all_gems = static_cast<size_t>(this->num_x_ * this->num_y_);

    // Resize the container so that it contains num_x * num_y 'gems'
    this->gems_.resize(all_gems, NULL);
    // Set all textures to -1 -> means texture is not set
    this->gem_masks_.resize(all_gems, 0);

    for (size_t i = 0; i < all_gems; ++i)
    {
        this->new_gem(i);
    }
}







