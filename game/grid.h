#ifndef GRID_H_
#define GRID_H_

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
    Grid(int min_x,
         int min_y,
         int len_x,
         int len_y,
         int num_x,
         int num_y);
    
    
    // Get face index by input coordinates (face index is used as gem index)
    // x,y - mouse coordinates
    // return - index value - on success, -1 - error/did not find index (i.e. input coords are outside of the grid)
    int get_id(int x, int y) const;

    // Get grid indexes by face index
    // id - face/gem index
    // i - index of input face (x-direction)
    // j - index of input face (y-direction)
    // return -1 on error
    int get_idxs(int idx, int& i, int& j) const;
    
    /// \brief Get x-coordinate by face/gem index
    /// \param idx index of face
    /// \return x-coordinate
    int get_x(int idx) const;
 
    /// \brief Get y-coordinate by face/gem index
    /// \param idx index of face
    /// \return y-coordinate
    int get_y(int idx) const;
    
protected:

    // Initialize grid
    // return -1 on error
    int init( int min_x,
              int min_y,
              int len_x,
              int len_y,
              int num_x,
              int num_y );


protected:

    int min_x_; // Origin coordinates
    int min_y_;
    int max_x_; // Max coordinates (calculated values)
    int max_y_;
    int len_x_; // x,y sizes of one grid face
    int len_y_;
    int num_x_; // Number of faces in each direction
    int num_y_;
   
};
#endif // GRID_H_
