//
//  MandelbrotViewer.hpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 08/07/2021.
//

#ifndef MandelbrotViewer_hpp
#define MandelbrotViewer_hpp

#include <SFML/Graphics.hpp>

class MandelbrotViewer {
public:
    MandelbrotViewer();
    
    void set_rendering_settings(unsigned, unsigned);
    void set_previous_state_optimization(bool);
    
    void update_offset(int, int);
    void update_scale(float);
    void increase_limit();
    void decrease_limit();
    
    void refresh();
    
    void draw(sf::RenderWindow &);
    
private:
    // Constants
    unsigned WINDOW_WIDTH, WINDOW_HEIGHT;
    unsigned TEXTURE_WIDTH, TEXTURE_HEIGHT;
    float WINDOW_TO_TEXTURE;
    
    // Optimization parameters
    bool previous_state_flag;
    
    // Computation constrains
    float buffer_pixel_offset_x, buffer_pixel_offset_y;
    float offset_x, offset_y, scale;
    int limit;
    
    // Rendering variables
    sf::RenderTexture   texture;
    sf::Shader          shader;
    sf::Sprite          sprite;
    
    sf::Texture previous_state;
    
    // Private methods
    void update_texture();
};

#endif /* MandelbrotViewer_hpp */
