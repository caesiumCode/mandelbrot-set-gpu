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

#include "Mandelbrot.hpp"

namespace mv {
/**
 \brief Enumeration of the different type of mode
 */
enum ViewerMode {Default = 0, Debug = 1};

/**
 \brief Defines an object that compute the Mandelbrot Set
 */
class RenderMandelbrot : public Mandelbrot {
public:
    RenderMandelbrot();
    RenderMandelbrot(int, int);
    
    void        setWindowDimensions(int, int);
    
    void        setMode(ViewerMode);
    ViewerMode  getViewerMode();
        
    void        draw(sf::RenderWindow &);
    
private:
    // Constants
    int WINDOW_WIDTH, WINDOW_HEIGHT;
    
    // Mode
    mv::ViewerMode mode;
    
    // Drawing variables/methods
    sf::Sprite sprite;
    
    void draw_default(sf::RenderWindow &);
    void draw_debug(sf::RenderWindow &);
    sf::Vector2f complex_to_window(sf::Vector2f);
    sf::CircleShape point_to_shape(sf::Vector2f);
};
}

#endif /* MandelbrotViewer_hpp */
