//
//  MandelbrotViewer.hpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 08/07/2021.
//

#ifndef MandelbrotViewer_hpp
#define MandelbrotViewer_hpp

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "sfmath.hpp"

namespace mv {
using value_gradient = std::pair<float, sf::Vector2f>;

/**
 \brief Enumeration of the different type of mode
 */
enum ViewerMode {Default = 0, Debug = 1};

/**
 \brief Defines an object that compute the Mandelbrot Set
 */
class RenderMandelbrot {
public:
    RenderMandelbrot();
    
    void set_rendering_settings(unsigned, unsigned);
    void set_previous_state_optimization(bool);
    void set_mode(ViewerMode);
    
    void update_offset(int, int);
    void update_scale(float);
    void increase_limit();
    void decrease_limit();
    
    ViewerMode getViewerMode();
    
    void refresh();
    
    void draw(sf::RenderWindow &);
    
private:
    // Constants
    unsigned WINDOW_WIDTH, WINDOW_HEIGHT;
    unsigned TEXTURE_WIDTH, TEXTURE_HEIGHT;
    float WINDOW_TO_TEXTURE;
    
    // Mode
    ViewerMode mode;
    
    // Optimization parameters
    bool previous_state_flag;
    
    // Computation constrains
    float buffer_pixel_offset_x, buffer_pixel_offset_y;
    float offset_x, offset_y, scale;
    int limit;
    
    // Perturbation theory variables
    std::vector<sf::Vector2f> gradient_descent;
    sf::Vector2f reference_point;
    
    // Rendering variables
    sf::RenderTexture   texture;
    sf::Shader          shader;
    sf::Sprite          sprite;
    
    sf::Texture previous_state;
    
    // Private methods
    void draw_default(sf::RenderWindow &);
    void draw_debug(sf::RenderWindow &);
    
    
    void find_reference_point();
    value_gradient distance_estimation(sf::Vector2f);
    
    sf::Vector2f complex_to_window(sf::Vector2f);
    sf::CircleShape point_to_shape(sf::Vector2f);
};
}

#endif /* MandelbrotViewer_hpp */
