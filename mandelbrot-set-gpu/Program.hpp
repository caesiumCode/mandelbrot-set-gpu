//
//  Program.hpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 03/07/2021.
//

#ifndef Program_hpp
#define Program_hpp

#include <iostream>
#include <filesystem>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "MandelbrotViewer.hpp"

namespace fs = std::__fs::filesystem;

class Program {
public:
    Program(unsigned, unsigned);
    
    void run();
    
private:
    // Constants
    const std::string   PATH = fs::current_path().string();
    const std::string   WINDOW_TITLE = "Mandelbrot Set with GPU";
    const unsigned      WINDOW_WIDTH, WINDOW_HEIGHT;
    
    // SFML variables
    sf::Cursor          cursor;
    sf::RenderWindow    window;
    
    // Temporary variables
    sf::Vector2i mouse_previous_position;
    bool mouse_flag;
    
    // Mandelbrot Set related variables
    mv::RenderMandelbrot renderMandelbrot;
    
    // Get the right window title
    void updateTitle();
    
    // Drawing methods
    void drawDefaultMode();
    void drawDebugMode();
    
    // Event related methods
    void handleEvent(const sf::Event&);
    
    void exitEvent(const sf::Event&);
    void modeEvent(const sf::Event&);
    void scaleEvent(const sf::Event&);
    void offsetEvent(const sf::Event&);
    void limitEvent(const sf::Event&);
    void screenshotEvent(const sf::Event&);    
};

#endif /* Program_hpp */
