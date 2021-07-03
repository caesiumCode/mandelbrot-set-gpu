//
//  Program.cpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 03/07/2021.
//

#include "Program.hpp"

Program::Program(unsigned width, unsigned height):
WINDOW_WIDTH(width),
WINDOW_HEIGHT(height),
TEXTURE_WIDTH(1000),
TEXTURE_HEIGHT((float) WINDOW_HEIGHT/WINDOW_WIDTH*TEXTURE_WIDTH)
{
    // Setup window
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot Set with GPU");
    window.setFramerateLimit(30);
    
    // Setup texture
    MS_texture.create(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    MS_texture.clear(sf::Color::Black);
    
    // Setup shader
    MS_shader.loadFromFile("mandelbrot.frag", sf::Shader::Fragment);
    
    offset_x = 0.;
    offset_y = 0.;
    scale = 5.;
    MS_shader.setUniform("resolution", sf::Glsl::Vec2(TEXTURE_WIDTH, TEXTURE_HEIGHT));
    MS_shader.setUniform("zoom", sf::Glsl::Vec3(offset_x, offset_y, scale));
    
    // Setup sprite
    MS_sprite.setScale((float) WINDOW_WIDTH/TEXTURE_WIDTH, (float) WINDOW_HEIGHT/TEXTURE_HEIGHT);
    MS_sprite.setTexture(MS_texture.getTexture());
    
    // Setup mandelbrot set
    update();
}

void Program::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            handleEvent(event);
        }
        
        window.clear();
        
        window.draw(MS_sprite);
        
        window.display();
    }
}

void Program::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
        window.close();
    
    // Update viewer parameters
    if (event.KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Left:
                offset_x += scale*OFFSET_DELTA;
                break;
                
            case sf::Keyboard::Right:
                offset_x -= scale*OFFSET_DELTA;
                break;
                
            case sf::Keyboard::Down:
                offset_y += scale*OFFSET_DELTA;
                break;
                
            case sf::Keyboard::Up:
                offset_y -= scale*OFFSET_DELTA;
                break;
                                
            default:
                break;
        }
        
        MS_shader.setUniform("zoom", sf::Glsl::Vec3(offset_x, offset_y, scale));
        update();
    }
}

void Program::update() {
    MS_texture.draw(MS_sprite, &MS_shader);
    MS_texture.display();
    MS_sprite.setTexture(MS_texture.getTexture());
}
