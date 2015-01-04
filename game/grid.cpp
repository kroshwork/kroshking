#include "grid.h"
#include <iostream>
#include <cmath>

// Public methods

//-----------------------------------------------------------------------------
Grid::Grid(void)
{
    (void)this->init(1,1,1,1,1,1);
}

//-----------------------------------------------------------------------------
Grid::Grid(const Grid& obj)
{
    (void)this->init( obj.min_x_,
                      obj.min_y_,
                      obj.len_x_,
                      obj.len_y_,
                      obj.num_x_,
                      obj.num_y_ );
}

//-----------------------------------------------------------------------------
Grid& Grid::operator=(const Grid& obj)
{
    (void)this->init( obj.min_x_,
                      obj.min_y_,
                      obj.len_x_,
                      obj.len_y_,
                      obj.num_x_,
                      obj.num_y_ );
    return *this;
}

//-----------------------------------------------------------------------------
Grid::~Grid(void)
{ }

//-----------------------------------------------------------------------------
Grid::Grid(float  min_x,
           float  min_y,
           float  len_x,
           float  len_y,
           int  num_x,
           int  num_y)
{
    (void)this->init( min_x,
                      min_y,
                      len_x,
                      len_y,
                      num_x,
                      num_y);
}

//-----------------------------------------------------------------------------
int Grid::get_idx(float x, float y) const
{
    int result = -1;

    // Check if coords are inside of the grid
    if (x < this-> min_x_ || x > this->max_x_ || y < this-> min_y_ || y > this->max_y_)
    {
        std::cerr << "Grid :: input coordinates are outside of the grid" << std::endl;
        return result;
    }

    // Calc index
    std::cout << "min x = " << this->min_x_ << " ; min y = " << this->min_y_;
    std::cout << " ; len x = " << this->len_x_ << " ; len y = " << this->len_y_;
    std::cout << " ; num x = " << num_x_;
    std::cout << " ; x = " << x << " ; y = " << y << std::endl;
    result = std::floor( (x - this->min_x_) / this->len_x_ )
    +  std::floor((y - this->min_y_) / this->len_y_) * this->num_x_ ;
    return result;
}

//-----------------------------------------------------------------------------
int Grid::get_ij(int idx, int& i, int& j) const
{
    // Check input index
    if (idx < 0 || idx >= this->num_x_ * this->num_y_)
    {
        std::cerr << "Grid :: wrong input index" << std::endl;
        return -1;
    }

    // Calc indexes
    i = idx % this->num_x_;
    j = idx / this->num_x_;

    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
float Grid::get_x(int idx) const
{
    // Check input index
    if (idx < 0 || idx >= this->num_x_ * this->num_y_)
    {
        std::cerr << "Grid :: wrong input index (get_x)" << std::endl;
        return -1;
    }

    // Calc x coord
    float x_coord = this->min_x_ + (idx % this->num_x_) * this->len_x_;
    return x_coord;
}

//-----------------------------------------------------------------------------
float Grid::get_y(int idx) const
{
    // Check input index
    if (idx < 0 || idx >= this->num_x_ * this->num_y_)
    {
        std::cerr << "Grid :: wrong input index (get_y)" << std::endl;
        return -1;
    }

    // Calc y coord
    float y_coord = this->min_y_ + (idx / this->num_x_) * this->len_y_ ;
    return y_coord;
}

//-----------------------------------------------------------------------------
int Grid::get_ad(int idx, std::set<int>& ad) const
{
    int result = -1;
    int i = -1, j = -1;
    ad.clear();
    if (this->get_ij(idx, i, j) != -1)
    {
	if (i > 0)
	{
	    ad.insert((i - 1) + j * this->num_x_);
	}
	if (i < (this->num_x_ - 1))
	{
	    ad.insert((i + 1) + j * this->num_x_);
	}
	if (j > 0)
	{
	    ad.insert(i + (j - 1) * this->num_x_);
	}
	if (j < (this->num_y_ - 1))
	{
	    ad.insert(i + (j + 1) * this->num_x_);
	}
    }

    if (ad.size() > 0)
    {
        result = static_cast<int>(ad.size());
    }
    return result;
}

// Private methods

//-----------------------------------------------------------------------------
int Grid::init( float min_x,
                float min_y,
                float len_x,
                float len_y,
                int   num_x,
                int   num_y )
{

    // Check input parameters
    if (num_x < 1 || num_y < 1 || len_x <= 0 || len_y <= 0)
    {
        std::cerr << "Grid :: wrong input init parameters" << std::endl;
        return -1;
    }

    this->min_x_ = min_x;
    this->min_y_ = min_y;
    this->len_x_ = len_x;
    this->len_y_ = len_y;
    this->num_x_ = num_x;
    this->num_y_ = num_y;

    // Calc max grid coordinates
    this->max_x_ = min_x + len_x * num_x;
    this->max_y_ = min_y + len_y * num_y;

    return EXIT_SUCCESS;
}

