#include "gem_grid.h"
#include "global_opengl.h"
#include "global_consts.h"
#include <iostream>
#include <unistd.h>

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
    const GLuint w = this->tex_loader_.get_bg_width ();
    const GLuint h = this->tex_loader_.get_bg_height();
    GLfloat x = ( SCREEN_WIDTH  - w ) / 2.f;
    GLfloat y = ( SCREEN_HEIGHT - h ) / 2.f;
    this->tex_loader_.draw(x, y);

    for (int i = 0; i < (this->num_x_ * this->num_y_); ++i)
    {
        x = (GLfloat)this->get_x(i);
        y = (GLfloat)this->get_y(i);
        std::cout << std::endl << "!!! >>> x = " << x << ", y = " << y << std::endl;
        size_t tex_idx = this->gems_[i]->tex_idx_;
        this->gems_[i]->draw_ptr_(x, y, tex_idx, this->tex_loader_); // Calls texture draw or nothing
    }
}

//-----------------------------------------------------------------------------
void GemGrid::Gem::draw_null  (GLfloat , GLfloat, size_t, const Texture&) { return; }

//-----------------------------------------------------------------------------
void GemGrid::Gem::draw_static(GLfloat x, GLfloat y, size_t tex_idx, const Texture& tex_loader)
{
    tex_loader.draw(x, y, tex_idx);
    return;
}
void GemGrid::Gem::draw_moving(GLfloat , GLfloat, size_t, const Texture&) { return; }

//-----------------------------------------------------------------------------
void  GemGrid::mouse(int mouse_x, int mouse_y )
{
    int gem_idx = this->get_idx(mouse_x, mouse_y);
    if (gem_idx == -1)
    {
        std::cout << "Mouse did not touch gems" << std::endl;
        return;
    }

    // Initial mouse touch
    if (this->moving_gems_.size() == 0)
    {
        this->last_touch_.first  = mouse_x;
        this->last_touch_.second = mouse_y;
        this->moving_gems_.push(static_cast<size_t>(gem_idx));
    }
    // Mouse was already pressed at least once before
    else
    {
        int prev_gem_idx = this->moving_gems_.front();
        std::set<int> ad_gems;
        this->get_ad(prev_gem_idx, ad_gems);
        std::set<int>::iterator it = ad_gems.find(gem_idx);
        if (it != ad_gems.end())
        {
            //neighbour was found!!
            // calculate if there is any win
            // and if there IS -  swap items and process win
            // swap shoudl be done in update function
            
        }

    }

    //TODO


}

//-----------------------------------------------------------------------------
unsigned GemGrid::check_line(const int i, const int j, const int i_inc, const int j_inc,
                            const unsigned mask, std::set<int>* win_idx) const
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
        std::cout << std::endl;
        for_each (win_idx->begin(), win_idx->end(), [](int n) {std::cout << n << '\t';});
        std::cout << std::endl;
        //TODO CHECK IT
    }
    return res;
}

//-----------------------------------------------------------------------------
bool GemGrid::find_win_lines(const int i, const int j, const unsigned mask, std::set<int>* win_gems) const
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







