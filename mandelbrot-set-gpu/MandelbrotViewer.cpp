//
//  MandelbrotViewer.cpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 08/07/2021.
//

#include "MandelbrotViewer.hpp"

mv::RenderMandelbrot::RenderMandelbrot() {
    WINDOW_WIDTH = 1;
    WINDOW_HEIGHT = 1;
    
    Mandelbrot();
    
    mode = mv::ViewerMode::Default;
        
    sprite.setTexture(getTexture());
}

void mv::RenderMandelbrot::setWindowDimensions(int w, int h) {
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;
    
    Mandelbrot();
    
    set_texture_dimensions(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void mv::RenderMandelbrot::setMode(ViewerMode m) {
    mode = m;
}

mv::ViewerMode mv::RenderMandelbrot::getViewerMode() {
    return mode;
}

void mv::RenderMandelbrot::draw(sf::RenderWindow & window) {
    switch (mode) {
        case mv::ViewerMode::Default:
            draw_default(window);
            break;
            
        case mv::ViewerMode::Debug:
            draw_debug(window);
            
        default:
            break;
    }
}

void mv::RenderMandelbrot::draw_default(sf::RenderWindow & window) {
    sprite.setTexture(getTexture());
    window.draw(sprite);
}

void mv::RenderMandelbrot::draw_debug(sf::RenderWindow & window) {
    window.draw(sprite);
    
    std::vector<sf::Vector2f> gradient_path = getGradientPath();
    for (int i = 0; i < gradient_path.size(); i++) {
        window.draw(point_to_shape(gradient_path[i]));
        
        if (i > 0) {
            sf::Vertex line[] = {
                sf::Vertex(complex_to_window(gradient_path[i-1])),
                sf::Vertex(complex_to_window(gradient_path[i]))
            };
            
            line[0].color = sf::Color::Red;
            line[1].color = sf::Color::Red;
            
            window.draw(line, 2, sf::Lines);
        }
    }
}

sf::Vector2f mv::RenderMandelbrot::complex_to_window(sf::Vector2f p) {
    sf::Vector2f center = getCenter();
    return sf::Vector2f((float) (p.x-center.x)/getScale()*WINDOW_WIDTH + WINDOW_WIDTH/2,
                        (float) -(p.y-center.y)/getScale()*WINDOW_WIDTH + WINDOW_HEIGHT/2);
}

sf::CircleShape mv::RenderMandelbrot::point_to_shape(sf::Vector2f p) {
    const int SHAPE_RADIUS = 10;
    sf::CircleShape shape;
    shape.setRadius(SHAPE_RADIUS);
    shape.setOrigin(SHAPE_RADIUS, SHAPE_RADIUS);
    shape.setPosition(complex_to_window(p));
    shape.setFillColor(sf::Color::Red);
    
    return shape;
}
