//
//  Program.hpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 03/07/2021.
//

#ifndef Program_hpp
#define Program_hpp

#include <SFML/Graphics.hpp>

class Program {
public:
    Program(unsigned = 1000, unsigned = 700);
    
    void run();
    
private:
    // Constants
    const unsigned WINDOW_WIDTH, WINDOW_HEIGHT;
    const unsigned TEXTURE_WIDTH, TEXTURE_HEIGHT;
    const float OFFSET_DELTA = 0.05;
    
    // shader variables
    float offset_x, offset_y, scale;
    
    // SFML variables
    sf::RenderWindow window;
    sf::RenderTexture MS_texture;
    sf::Shader MS_shader;
    sf::Sprite MS_sprite;
    
    /**
     Deal with the user inputs
     
     \param event input to handle
     */
    void handleEvent(const sf::Event& event);
    
    /**
     Re-calculate the mandelbrot set texture
     */
    void update();
};

#endif /* Program_hpp */
