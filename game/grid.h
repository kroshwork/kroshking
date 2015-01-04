#ifndef GRID_H_
#define GRID_H_

#include <set>

// Regular 2D grid
class Grid
{
public:

    //Class essentials

    Grid(void);
    Grid(const Grid& obj);
    Grid& operator=(const Grid& obj);
    virtual ~Grid(void);

    // Parameterized constructor
    Grid(float min_x,
         float min_y,
         float len_x,
         float len_y,
         int   num_x,
         int   num_y);
    
    
    // Get face index by input coordinates (face index is used as gem index)
    // x,y - mouse coordinates
    // return - index value - on success, -1 - error/did not find index (i.e. input coords are outside of the grid)
    int get_idx(int x, int y) const;

    // Get grid indexes by face index
    // id - face/gem index
    // i - index of input face (x-direction)
    // j - index of input face (y-direction)
    // return -1 on error
    int get_ij(int idx, int& i, int& j) const;
    
    /// \brief Get x-coordinate by face/gem index
    /// \param idx index of face
    /// \return x-coordinate
    int get_x(int idx) const;
 
    /// \brief Get y-coordinate by face/gem index
    /// \param idx index of face
    /// \return y-coordinate
    int get_y(int idx) const;

    /// \brief Get adjacent faces
    /// \param idx index of face
    /// \param[out] ad_faces array of adjacent faces (diagonals are not included)
    /// \return number of neighbour faces
    int get_ad(int idx, std::set<int>& ad_faces) const;
    
protected:

    // Initialize grid
    // return -1 on error
    int init( float min_x,
              float min_y,
              float len_x,
              float len_y,
              int   num_x,
              int   num_y );


protected:

    float min_x_; // Origin coordinates
    float min_y_;
    float max_x_; // Max coordinates (calculated values)
    float max_y_;
    float len_x_; // x,y sizes of one grid face
    float len_y_;
    int   num_x_; // Number of faces in each direction
    int   num_y_;
   
};
#endif // GRID_H_
