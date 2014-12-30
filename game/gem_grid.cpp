#include "gem_grid.h"
#include "global_opengl.h"
#include <iostream>

//-----------------------------------------------------------------------------
GemGrid::~GemGrid(void)
{
    // TODO
    
}

//-----------------------------------------------------------------------------
GemGrid& GemGrid::get_instace(void)
{
    static GemGrid instance;
    return instance;
}

//-----------------------------------------------------------------------------
bool GemGrid::load_bg_tex( const std::string& fname )
{
    return this->tex_loader_.load_bg(fname);
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
void GemGrid::draw( void )
{
    if (this->tex_loader_.get_bg_id() != 0)
    {
        glLoadIdentity();
 
        // Centered offsets
        GLfloat x = ( SCREEN_WIDTH  - this->tex_loader_.get_bg_width ()) / 2.f;
        GLfloat y = ( SCREEN_HEIGHT - this->tex_loader_.get_bg_height()) / 2.f;
 
        glTranslatef( x, y, 0.f );

        //Set texture ID
        glBindTexture( GL_TEXTURE_2D, this->tex_loader_.get_bg_id() );

        //Render textured quad
        printf("BG Width  = %d\n", this->tex_loader_.get_bg_width()  );
        printf("BG Height = %d\n", this->tex_loader_.get_bg_height() );
        
        glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(           0.f ,            0.f);
            glTexCoord2f( 1.f, 0.f ); glVertex2f( mTextureWidth ,            0.f);
            glTexCoord2f( 1.f, 1.f ); glVertex2f( mTextureWidth , mTextureHeight);
            glTexCoord2f( 0.f, 1.f ); glVertex2f(           0.f , mTextureHeight);
        glEnd();
        /*
        //Render textured quad
        glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(           0.f + mTextureWidth*1.5,            0.f );
            glTexCoord2f( 1.f, 0.f ); glVertex2f( mTextureWidth + mTextureWidth*1.5,            0.f );
            glTexCoord2f( 1.f, 1.f ); glVertex2f( mTextureWidth + mTextureWidth*1.5, mTextureHeight );
            glTexCoord2f( 0.f, 1.f ); glVertex2f(           0.f + mTextureWidth*1.5, mTextureHeight);
        glEnd();
*/
        //TODO
    }
}



 
//-----------------------------------------------------------------------------
GemGrid::GemGrid( void )
{


}
 

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
    // Set all textures to -1 -> means texture is not set
    this->gem_masks_.resize(all_gems, -1);

    // Request that the vector capacity be at least enough to contain num_x * num_y elements
    // We do not know yet what how many gems will be displayed - so just reserve
    this->moving_gems_.reserve(all_gems);
}

//-----------------------------------------------------------------------------
unsigned GemGrid::check_line(int i, int j, int i_inc, int j_inc, unsigned mask, std::set<int>* win_idx) const
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
    for (auto n: idx_vec)
    {
        res &= static_cast<unsigned>(this->gem_masks_[n]);
    }
    if (res > 0 && win_idx != NULL)
    {
        for (auto n: idx_vec)
        {
            win_idx->insert(n);
        }
    }
    return res;
}

//-----------------------------------------------------------------------------
bool GemGrid::find_win_lines(int i, int j, unsigned mask, std::set<size_t>* win_gems) const
{
    if (win_gems != NULL)
    {
        win_gems->clear();
    }

    // Check left horizontal line
    unsigned res_left = 0;
    if (i >= (WIN_LINE_SIZE - 1))
    {
        res_left = this->check_line(i, j, -1, 0, win_gems);
    }

    // Check right horizontal line
    unsigned res_right = 0;
    if (i <=  (this->num_x_ - WIN_LINE_SIZE))
    {
        res_left = this->check_line(i, j, 1, 0, win_gems);
    }

    // Check top vertical
    unsigned res_up = 0;
    if (j >= (WIN_LINE_SIZE - 1))
    {
        res_up = this->check_line(i, j, 0, -1, win_gems);
    }

    // Check bottom vertical
    unsigned res_down = 0;
    if (i <=  (this->num_x_ - WIN_LINE_SIZE))
    {
        res_up = this->check_line(i, j, 0, 1, win_gems);
    }

    return (res_up || res_down || res_left || res_right);
}

//-----------------------------------------------------------------------------
void GemGrid::new_gem(size_t idx, const Texture& tex_loader)
{
    int i = 0, j = 0;
    this->get_idxs(idx, i, j);

    unsigned gem_mask = tex_loader.get_random();
    bool lines_found = this->find_win_lines(i, j, gem_mask);

    while (found_in_x || found_in_y) // TODO - additional stop criteria might be needed
    {
        gem_mask = tex_loader.get_next();
        lines_found = this->find_win_lines(i, j, static_cast<unsigned>(gem_mask), NULL);
    }

    // If fine texture was found, create new gem and update mask in mask array
    if (gem_mask != 0)
    {
        if (this->gems_[id] != NULL)
        {
            delete this->gems_[idx];
        }
        this->gems_[id] = new Gem(tex_loader.get_current());
        this->gem_masks_[id] = gem_mask;
    }
    else
    {
        std::cerr << "GemGrid :: failed to create a new gem with ID = " << id << std::endl;
    }
}

//-----------------------------------------------------------------------------
void GemGrid::generate_gems(void)
{
    const size_t all_gems = this->gems_.size();
    for (size_t i = 0; i < all_gems; ++i)
    {
        this->new_gem(i, this->tex_loader_);
    }
}







