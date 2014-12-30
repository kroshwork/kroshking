#include "grid.h"
#include <iostream>

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
Grid::Grid(int  min_x,
           int  min_y,
           int  len_x,
           int  len_y,
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
int Grid::get_id(int x, int y) const
{
    int result = -1;
    
    // Check if coords are inside of the grid
    if (x < this-> min_x_ || x > this->max_x_ || y < this-> min_y_ || y > this->max_y_)
    {
        std::cerr << "Grid :: input coordinates are outside of the grid" << std::endl;
        return result;
    }

    // Calc ID
    result = (x - this->min_x_) / this->len_x_ + ((y - this->min_y_) / this->len_y_) * this->num_x_;
    return result;
}

//-----------------------------------------------------------------------------
int Grid::get_idxs(int id, int& i, int& j) const
{
    // Check input ID
    if (id < 0 || id >= this->num_x_ * this->num_y_)
    {
        std::cerr << "Grid :: wrong input ID" << std::endl;
        return -1;
    } 

    // Calc indexes
    i = id % this->num_x_;
    j = id / this->num_x_;

    return EXIT_SUCCESS;
}


// Private methods

//-----------------------------------------------------------------------------
int Grid::init( int min_x,
                int min_y,
                int len_x,
                int len_y,
                int num_x,
                int num_y )
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

