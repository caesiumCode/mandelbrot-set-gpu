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
TEXTURE_WIDTH(WINDOW_WIDTH),
TEXTURE_HEIGHT((float) WINDOW_HEIGHT/WINDOW_WIDTH*TEXTURE_WIDTH)
{
    // Setup cursor
    cursor.loadFromSystem(sf::Cursor::Arrow);
    
    // Setup window
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
    window.setFramerateLimit(30);
    window.setMouseCursor(cursor);
    
    // Setup texture
    MS_texture.create(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    MS_texture.clear(sf::Color::Black);
    
    // Setup shader
    MS_shader.loadFromFile("mandelbrot.frag", sf::Shader::Fragment);
    
    offset_x = 0.;
    offset_y = 0.;
    scale = 5.;
    limit = 100;
    previous_state.create(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    MS_shader.setUniform("resolution", sf::Glsl::Vec2(TEXTURE_WIDTH, TEXTURE_HEIGHT));
    MS_shader.setUniform("zoom", sf::Glsl::Vec3(offset_x, offset_y, scale));
    MS_shader.setUniform("limit", limit);
    MS_shader.setUniform("previous_state", previous_state);
    MS_shader.setUniform("previous_state_position", sf::Glsl::Vec4(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT));
    MS_shader.setUniform("previous_state_flag", false);
    
    // Setup sprite
    MS_sprite.setScale((float) WINDOW_WIDTH/TEXTURE_WIDTH, (float) WINDOW_HEIGHT/TEXTURE_HEIGHT);
    MS_sprite.setTexture(MS_texture.getTexture());
    
    // Setup mandelbrot set
    update();
    
    // Setup Temporary variables
    mouse_flag = false;
    window.setTitle(WINDOW_TITLE + " | limit=" + std::to_string((int)limit));
}

void Program::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            handleEvent(event);
        
        window.clear();
        
        window.draw(MS_sprite);
        
        window.display();
    }
}

void Program::handleEvent(const sf::Event& event) {
    exitEvent(event);
    scaleEvent(event);
    offsetEvent(event);
    limitEvent(event);
}

void Program::exitEvent(const sf::Event & event) {
    if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
        window.close();
}

void Program::scaleEvent(const sf::Event & event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        sf::Event::MouseWheelScrollEvent mouse_parameters = event.mouseWheelScroll;
        
        scale *= exp(-0.1*mouse_parameters.delta);
        
        MS_shader.setUniform("zoom", sf::Glsl::Vec3(offset_x, offset_y, scale));
        update();
    }
}

void Program::offsetEvent(const sf::Event & event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        cursor.loadFromSystem(sf::Cursor::Hand);
        window.setMouseCursor(cursor);
        
        mouse_previous_position = sf::Mouse::getPosition();
        
        mouse_flag = true;
        MS_shader.setUniform("previous_state_flag", true);
    }
    
    if (mouse_flag) {
        previous_state.update(MS_texture.getTexture());
        MS_shader.setUniform("previous_state", previous_state);
        
        
        sf::Vector2i mouse_offset = sf::Mouse::getPosition() - mouse_previous_position;
        float temp_offset_x = (float)mouse_offset.x/WINDOW_WIDTH*scale;
        float temp_offset_y = (float)mouse_offset.y/WINDOW_WIDTH*scale;
        
        MS_shader.setUniform("zoom", sf::Glsl::Vec3(offset_x - temp_offset_x, offset_y + temp_offset_y, scale));
        MS_shader.setUniform("previous_state_position", sf::Glsl::Vec4(mouse_offset.x, -mouse_offset.y, TEXTURE_WIDTH, TEXTURE_HEIGHT));
        
        offset_x -= temp_offset_x;
        offset_y += temp_offset_y;
        update();
        mouse_previous_position = sf::Mouse::getPosition();
    }
    
    if (event.type == sf::Event::MouseButtonReleased) {
        cursor.loadFromSystem(sf::Cursor::Arrow);
        window.setMouseCursor(cursor);
        
        sf::Vector2i mouse_offset = sf::Mouse::getPosition() - mouse_previous_position;
        float temp_offset_x = (float)mouse_offset.x/WINDOW_WIDTH*scale;
        float temp_offset_y = (float)mouse_offset.y/WINDOW_WIDTH*scale;
        
        offset_x -= temp_offset_x;
        offset_y += temp_offset_y;
        
        mouse_flag = false;
        
        MS_shader.setUniform("previous_state_flag", false);
    }
}

void Program::limitEvent(const sf::Event & event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::A:
                if (limit < 10)
                    limit++;
                else
                    limit *= 1.1;
                break;
                
            case sf::Keyboard::Z:
                if (limit > 0)
                    limit *= 0.9;
                break;
                
            default:
                break;
        }
        limit = round(limit);        
        
        window.setTitle(WINDOW_TITLE + " | limit=" + std::to_string((int)limit));
        MS_shader.setUniform("limit", limit);
        update();
    }
}




void Program::update() {
    MS_texture.clear();
    MS_texture.draw(MS_sprite, &MS_shader);
    MS_texture.display();
    MS_sprite.setTexture(MS_texture.getTexture());
}
