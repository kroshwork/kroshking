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
    
    
    // Get face ID by input coordinates (face ID is used as gem ID)
    // x,y - mouse coordinates
    // return - ID value - on success, -1 - error/did not find ID (i.e. input coords are outside of the grid)
    int get_id(int x, int y) const;

    // Get grid indexes by face ID
    // id - face/gem ID
    // i - index of input face (x-direction)
    // j - index of input face (y-direction)
    // return -1 on error
    int get_idxs(int id, int& i, int& j) const;

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
