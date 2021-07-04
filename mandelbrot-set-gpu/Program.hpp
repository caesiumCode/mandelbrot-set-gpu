//
//  Program.hpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 03/07/2021.
//

#ifndef Program_hpp
#define Program_hpp

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

class Program {
public:
    Program(unsigned = 1000, unsigned = 700);
    
    void run();
    
private:
    // Constants
    const std::string   WINDOW_TITLE = "Mandelbrot Set with GPU";
    const unsigned      WINDOW_WIDTH, WINDOW_HEIGHT;
    const unsigned      TEXTURE_WIDTH, TEXTURE_HEIGHT;
    const float         OFFSET_DELTA = 0.01;
    
    // shader variables
    float offset_x, offset_y, scale;
    int limit;
    
    // SFML variables
    sf::Cursor          cursor;
    sf::RenderWindow    window;
    sf::RenderTexture   MS_texture;
    sf::Shader          MS_shader;
    sf::Sprite          MS_sprite;
    
    // Temporary variables
    sf::Vector2i mouse_initial_position;
    bool mouse_flag;
    
    
    
    
    // Event related methods
    void handleEvent(const sf::Event&);
    
    void exitEvent(const sf::Event&);
    void scaleEvent(const sf::Event&);
    void offsetEvent(const sf::Event&);
    void limitEvent(const sf::Event&);
    
    // update mandelbrot set frame
    void update();
};

#endif /* Program_hpp */
