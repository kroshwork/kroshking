#ifndef UTILS_H_
#define UTILS_H_


//TODO cope-past the file with name - I forgot what it was about

/// \brief Initialize viewport, matrices, clear color, enable texturing
/// \return false - on error, true - otherwise
bool init( void );

/// \brief Load assets to enable the game engine (gem_grid)
/// \return false - on error, true - otherwise
bool load_assets( void );

// Call-backs

/// \brief Display scenes
void display( void );

/// \brief Update/per-frame logic
void update( void );

/// \brief Handles mouse events
/// \param btn TODO
/// \param state TODO
/// \param x TODO
/// \param y TODO
void mouse(int btn, int state, int x, int y);

//TODO keyboard
void keyboard(unsigned char key, int, int);

#endif
