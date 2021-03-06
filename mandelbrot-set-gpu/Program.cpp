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
    renderMandelbrot.set_rendering_settings(WINDOW_WIDTH, WINDOW_HEIGHT);
    updateTitle();
    
    mouse_flag = false;
}

// Main loop of the program
void Program::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            handleEvent(event);
        
        window.clear();
        
        switch (renderMandelbrot.getViewerMode()) {
            case mv::ViewerMode::Default:
                drawDefaultMode();
                break;
                
            case mv::ViewerMode::Debug:
                drawDebugMode();
                break;
                
            default:
                break;
        }
        
        window.display();
    }
}

void Program::drawDefaultMode() {
    renderMandelbrot.draw(window);
}

void Program::drawDebugMode() {
    renderMandelbrot.draw(window);
}

// Handle any kind of expected input
void Program::handleEvent(const sf::Event& event) {
    exitEvent(event);
    modeEvent(event);
    scaleEvent(event);
    offsetEvent(event);
    limitEvent(event);
    screenshotEvent(event);
    
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSlash)
        renderMandelbrot.refresh();
    
    updateTitle();
}

void Program::exitEvent(const sf::Event & event) {
    if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
        window.close();
}

void Program::modeEvent(const sf::Event & event) {
    if (event.type == sf::Event::KeyPressed) {
        
        switch (event.key.code) {
            case sf::Keyboard::Q:
                renderMandelbrot.set_mode(mv::ViewerMode::Default);
                break;
                
            case sf::Keyboard::S:
                renderMandelbrot.set_mode(mv::ViewerMode::Debug);
                break;
                
            default:
                break;
        }
        
    }
}

void Program::scaleEvent(const sf::Event & event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        sf::Event::MouseWheelScrollEvent mouse_parameters = event.mouseWheelScroll;
        
        float update_factor = exp(-0.1*mouse_parameters.delta);
        
        if (mouse_parameters.delta != 0.f) {
            renderMandelbrot.set_previous_state_optimization(true);
            renderMandelbrot.update_scale(update_factor);
            renderMandelbrot.set_previous_state_optimization(false);
        }
    }
}

void Program::offsetEvent(const sf::Event & event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        cursor.loadFromSystem(sf::Cursor::Hand);
        window.setMouseCursor(cursor);
        
        mouse_previous_position = sf::Mouse::getPosition();
        
        mouse_flag = true;
        
        renderMandelbrot.set_previous_state_optimization(true);
    }
    
    if (mouse_flag) {
        sf::Vector2i mouse_offset = sf::Mouse::getPosition() - mouse_previous_position;
        
        renderMandelbrot.update_offset(mouse_offset.x, -mouse_offset.y);
        
        mouse_previous_position = sf::Mouse::getPosition();
    }
    
    if (event.type == sf::Event::MouseButtonReleased) {
        cursor.loadFromSystem(sf::Cursor::Arrow);
        window.setMouseCursor(cursor);
        
        mouse_flag = false;
        
        renderMandelbrot.set_previous_state_optimization(false);
    }
}

void Program::limitEvent(const sf::Event & event) {
    if (event.type == sf::Event::KeyPressed) {
        
        if      (event.key.code == sf::Keyboard::Up)    renderMandelbrot.increase_limit();
        else if (event.key.code == sf::Keyboard::Down)  renderMandelbrot.decrease_limit();
    }
}

void Program::screenshotEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return) {
        
        std::string filename = "mandelbrot-set";
        if (renderMandelbrot.getViewerMode() == mv::ViewerMode::Default)
            filename += "[default]";
        else
            filename += "[debug]";
        
        sf::Vector3f loc = renderMandelbrot.getLocalisation();
        int limit = renderMandelbrot.getLimit();
        
        filename += "[" + std::to_string(loc.x) + ":" + std::to_string(loc.y) + ":" + std::to_string(loc.z) + "]";
        filename += "[" + std::to_string(limit) + "]";
        filename += ".png";
        
        std::string filepath = PATH + "/" + filename;
        
        sf::Texture texture;
        texture.create(window.getSize().x, window.getSize().y);
        texture.update(window);
        if (texture.copyToImage().saveToFile(filepath))
        {
            std::cout << "screenshot saved to " << filepath << std::endl;
        }
    }
}


void Program::updateTitle() {
    std::string title = WINDOW_TITLE + " ";
    
    if (renderMandelbrot.getViewerMode() == mv::ViewerMode::Default)
        title += "[default]";
    else
        title += "[debug]";
    
    sf::Vector3f loc = renderMandelbrot.getLocalisation();
    int limit = renderMandelbrot.getLimit();
    
    title += "[" + std::to_string(loc.x) + ":" + std::to_string(loc.y) + ":" + std::to_string(loc.z) + "]";
    title += "[" + std::to_string(limit) + "]";
    
    window.setTitle(title);
}
