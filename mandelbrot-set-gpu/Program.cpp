//
//  Program.cpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 03/07/2021.
//

#include "Program.hpp"

Program::Program(unsigned width, unsigned height):
WINDOW_WIDTH(width),
WINDOW_HEIGHT(height)
{
    // Setup cursor
    cursor.loadFromSystem(sf::Cursor::Arrow);
    
    // Setup window
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
    window.setFramerateLimit(30);
    window.setMouseCursor(cursor);
    
    // Setup variables
    mandelbrotViewer.set_rendering_settings(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    mouse_flag = false;
}

// Main loop of the program
void Program::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            handleEvent(event);
        
        window.clear();
        
        mandelbrotViewer.draw(window);
        
        window.display();
    }
}

// Handle any kind of expected input
void Program::handleEvent(const sf::Event& event) {
    exitEvent(event);
    scaleEvent(event);
    offsetEvent(event);
    limitEvent(event);
    
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
        mandelbrotViewer.refresh();
}

void Program::exitEvent(const sf::Event & event) {
    if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
        window.close();
}

void Program::scaleEvent(const sf::Event & event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        sf::Event::MouseWheelScrollEvent mouse_parameters = event.mouseWheelScroll;
        
        float update_factor = exp(-0.1*mouse_parameters.delta);
        
        mandelbrotViewer.update_scale(update_factor);
    }
}

void Program::offsetEvent(const sf::Event & event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        cursor.loadFromSystem(sf::Cursor::Hand);
        window.setMouseCursor(cursor);
        
        mouse_previous_position = sf::Mouse::getPosition();
        
        mouse_flag = true;
        
        mandelbrotViewer.set_previous_state_optimization(true);
    }
    
    if (mouse_flag) {
        sf::Vector2i mouse_offset = sf::Mouse::getPosition() - mouse_previous_position;
        
        mandelbrotViewer.update_offset(mouse_offset.x, -mouse_offset.y);
        
        mouse_previous_position = sf::Mouse::getPosition();
    }
    
    if (event.type == sf::Event::MouseButtonReleased) {
        cursor.loadFromSystem(sf::Cursor::Arrow);
        window.setMouseCursor(cursor);
        
        mouse_flag = false;
        
        mandelbrotViewer.set_previous_state_optimization(false);
    }
}

void Program::limitEvent(const sf::Event & event) {
    if (event.type == sf::Event::KeyPressed) {
        
        if      (event.key.code == sf::Keyboard::A) mandelbrotViewer.increase_limit();
        else if (event.key.code == sf::Keyboard::Z) mandelbrotViewer.decrease_limit();
        
    }
}
