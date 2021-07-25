//
//  Mandelbrot.hpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 22/07/2021.
//

#ifndef Mandelbrot_hpp
#define Mandelbrot_hpp

#include <SFML/Graphics.hpp>
#include "sfmath.hpp"
#include <iostream>
#include "MandelbrotShader.hpp"

namespace mv {
class Mandelbrot {
public:
    Mandelbrot();
    
    void set_texture_dimensions(int, int);
        
    void update_center(sf::Vector2i);
    void update_center(sf::Vector2f);
    void update_scale(float);
    void increase_limit();
    void decrease_limit();
    
    const float&           getScale();
    const sf::Vector2f&    getCenter();
    const sf::Vector2f&    getReferencePoint();
    
    const std::vector<sf::Vector2f>& getGradientPath();
    const sf::Texture&               getTexture();
    
    void refresh();
    
private:
    // Constants
    int TEXTURE_WIDTH, TEXTURE_HEIGHT;
    
    // Computation constrains
    sf::Vector2f center;
    float scale;
    int limit;
    
    // Render optimisation variables
    bool ps_flag;
    sf::Vector2i ps_position, ps_size;
    sf::Texture ps_texture;
    
    // Rendering Mandelbrot Set
    sf::RenderTexture       render_texture;
    mv::MandelbrotShader    render_shader;
    
    // Perturbation theory variables
    std::vector<sf::Vector2f> gradient_path;
    sf::Vector2f reference_point;
        
    // Helper private methods
    void render();
    
    // Complex plane calculations    
    void find_reference_point();
    sf::math::value_gradient distance_estimation(sf::Vector2f);
};
}

#endif /* Mandelbrot_hpp */
