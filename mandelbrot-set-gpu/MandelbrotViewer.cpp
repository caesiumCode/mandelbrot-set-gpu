//
//  MandelbrotViewer.cpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 08/07/2021.
//

#include "MandelbrotViewer.hpp"

mv::RenderMandelbrot::RenderMandelbrot() {
    TEXTURE_WIDTH = 1;
    TEXTURE_HEIGHT = 1;
    
    mode = mv::ViewerMode::Default;
    
    // Optimization parameters
    previous_state_flag = false;
    
    // Computation constrains
    buffer_pixel_offset_x = 0.;
    buffer_pixel_offset_y = 0.;
    offset_x = 0.;
    offset_y = 0.;
    scale = 5.;
    limit = 100;
    
    // Rendering variables
    texture.create(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    
    previous_state.create(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    
    shader.loadFromFile("mandelbrot.frag", sf::Shader::Fragment);
    shader.setUniform("resolution", sf::Glsl::Vec2(TEXTURE_WIDTH, TEXTURE_HEIGHT));
    shader.setUniform("zoom", sf::Glsl::Vec3(offset_x, offset_y, scale));
    shader.setUniform("limit", limit);
    shader.setUniform("previous_state", previous_state);
    shader.setUniform("previous_state_position", sf::Glsl::Vec4(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT));
    shader.setUniform("previous_state_flag", previous_state_flag);
    shader.setUniform("mode", mode);
}

void mv::RenderMandelbrot::set_rendering_settings(unsigned int w, unsigned int h) {
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;
    
    TEXTURE_WIDTH = w;
    TEXTURE_HEIGHT = h;
    
    WINDOW_TO_TEXTURE = (float) TEXTURE_WIDTH/WINDOW_WIDTH;
    
    // Update Textures
    texture.create(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    previous_state.create(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    
    sprite.setTexture(texture.getTexture());
    sprite.setScale((float) WINDOW_WIDTH/TEXTURE_WIDTH, (float)WINDOW_HEIGHT/TEXTURE_HEIGHT);
    
    // Update shader variables
    shader.setUniform("resolution", sf::Glsl::Vec2(TEXTURE_WIDTH, TEXTURE_HEIGHT));
    shader.setUniform("previous_state_position", sf::Glsl::Vec4(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT));
    shader.setUniform("previous_state", previous_state);
    
    refresh();
}

void mv::RenderMandelbrot::set_previous_state_optimization(bool flag) {
    previous_state_flag = flag;
    
    shader.setUniform("previous_state_flag", previous_state_flag);
}

void mv::RenderMandelbrot::set_mode(ViewerMode m) {
    mode = m;
    shader.setUniform("mode", mode);
    
    refresh();
}

void mv::RenderMandelbrot::update_offset(int pixel_dx, int pixel_dy) {
    buffer_pixel_offset_x += pixel_dx;
    buffer_pixel_offset_y += pixel_dy;
    
    int texture_pixel_offset_x = (float) buffer_pixel_offset_x * WINDOW_TO_TEXTURE;
    int texture_pixel_offset_y = (float) buffer_pixel_offset_y * WINDOW_TO_TEXTURE;
    
    // Update offset
    if (abs(texture_pixel_offset_x) >= 1) {
        
        offset_x -= (float) texture_pixel_offset_x/TEXTURE_WIDTH*scale;
        buffer_pixel_offset_x -= (float) texture_pixel_offset_x / WINDOW_TO_TEXTURE;
        
    } if (abs(texture_pixel_offset_y) >= 1) {
        
        offset_y -= (float) texture_pixel_offset_y/TEXTURE_WIDTH*scale;;
        buffer_pixel_offset_y -= (float) texture_pixel_offset_y / WINDOW_TO_TEXTURE;;
    }
    
    // Update shader variables
    shader.setUniform("zoom", sf::Glsl::Vec3(offset_x, offset_y, scale));
    shader.setUniform("previous_state_position", sf::Glsl::Vec4(texture_pixel_offset_x,
                                                                texture_pixel_offset_y,
                                                                TEXTURE_WIDTH, TEXTURE_HEIGHT));
    if (previous_state_flag) shader.setUniform("previous_state", previous_state);
    
    refresh();
}

void mv::RenderMandelbrot::update_scale(float factor) {
    scale *= factor;
    shader.setUniform("zoom", sf::Glsl::Vec3(offset_x, offset_y, scale));
    
    refresh();
}

void mv::RenderMandelbrot::increase_limit() {
    if (limit < 10)
        limit++;
    else
        limit *= 1.1;
    
    shader.setUniform("limit", limit);
    
    refresh();
}

void mv::RenderMandelbrot::decrease_limit() {
    if (limit > 0) {
        if (limit < 10)
            limit--;
        else
            limit *= 0.9;
        
        shader.setUniform("limit", limit);
    }
    
    refresh();
}

mv::ViewerMode mv::RenderMandelbrot::getViewerMode() {
    return mode;
}

void mv::RenderMandelbrot::refresh() {
    // Find reference point
    find_reference_point();
    
    // Save previous state
    previous_state.update(texture.getTexture());
    
    // Update texture
    texture.clear();
    texture.draw(sprite, &shader);
    texture.display();
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
    window.draw(sprite);
}

void mv::RenderMandelbrot::draw_debug(sf::RenderWindow & window) {
    window.draw(sprite);
    
    for (int i = 0; i < gradient_descent.size(); i++) {
        window.draw(point_to_shape(gradient_descent[i]));
        
        if (i > 0) {
            sf::Vertex line[] = {
                sf::Vertex(complex_to_window(gradient_descent[i-1])),
                sf::Vertex(complex_to_window(gradient_descent[i]))
            };
            
            line[0].color = sf::Color::Red;
            line[1].color = sf::Color::Red;
            
            window.draw(line, 2, sf::Lines);
        }
    }
}

sf::Vector2f mv::RenderMandelbrot::complex_to_window(sf::Vector2f p) {
    return sf::Vector2f((float) (p.x-offset_x)/scale*WINDOW_WIDTH + WINDOW_WIDTH/2,
                        (float) -(p.y-offset_y)/scale*WINDOW_WIDTH + WINDOW_HEIGHT/2);
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

void mv::RenderMandelbrot::find_reference_point() {
    sf::Vector2f point = sf::Vector2f(offset_x, offset_y);
    
    value_gradient distance = distance_estimation(point);
    
    
    gradient_descent.clear();
    gradient_descent.push_back(point);
    
    
    int keeper = 0;
    while (distance.first > (float) scale/WINDOW_WIDTH && keeper < 100) {
        point.x += distance.second.x*distance.first;
        point.y += distance.second.y*distance.first;
        
        gradient_descent.push_back(point);
                
        distance = distance_estimation(point);
        keeper++;
    }
        
    reference_point = point;
}


mv::value_gradient mv::RenderMandelbrot::distance_estimation(sf::Vector2f c) {
    // Derivatives are in respect to c
    
    sf::Vector2f z = sf::Vector2f(0.f, 0.f);    // z
    sf::Vector2f dz = sf::Vector2f(0.f, 0.f);   // z'
    
    sf::Vector2f gx = sf::Vector2f(0.f, 0.f);   // gradient of Re(z)
    sf::Vector2f gy = sf::Vector2f(0.f, 0.f);   // gradient of Im(z)
    
    sf::Vector2f gdx = sf::Vector2f(0.f, 0.f);  // gradient of Re(z')
    sf::Vector2f gdy = sf::Vector2f(0.f, 0.f);  // gradient of Im(z')
    
    int n = 0;
    while (z.x*z.x + z.y*z.y <= 1.0e+10 && n < limit) {
        sf::Vector2f temp_gx = gx;
        sf::Vector2f temp_gdx = gdx;
        
        gdx = sf::Vector2f(2.f*dz.x*gx.x - 2.f*dz.y*gy.x + 2.f*z.x*gdx.x - 2.f*z.y*gdy.x,
                           2.f*dz.x*gx.y - 2.f*dz.y*gy.y + 2.f*z.x*gdx.y - 2.f*z.y*gdy.y);
        gdy = sf::Vector2f(2.f*dz.x*gy.x + 2.f*dz.y*gx.x + 2.f*z.x*gdy.x + 2.f*z.y*temp_gdx.x,
                           2.f*dz.x*gy.y + 2.f*dz.y*gx.y + 2.f*z.x*gdy.y + 2.f*z.y*temp_gdx.y);
        
        gx = sf::Vector2f(2.f*z.x*gx.x - 2.f*z.y*gy.x + 1.f, 2.f*z.x*gx.y - 2.f*z.y*gy.y);
        gy = sf::Vector2f(2.f*z.x*gy.x + 2.f*z.y*temp_gx.x, 2.f*z.x*gy.y + 2.f*z.y*temp_gx.y + 1.f);
        
        dz = sf::Vector2f(2.f*z.x*dz.x - 2.f*z.y*dz.y + 1.f, 2.f*z.x*dz.y + 2.f*z.y*dz.x);
        
        z = sf::Vector2f(z.x*z.x - z.y*z.y + c.x, 2.f*z.x*z.y + c.y);
        
        n++;
    }
    
    if (n == limit) {
        return {0.f, sf::Vector2f(0.f, 0.f)};
    } else {
        float norm2 = z.x*z.x + z.y*z.y, dnorm2 = dz.x*dz.x + dz.y*dz.y;
        float norm = sqrt(norm2), dnorm = sqrt(dnorm2);
        float log_norm = log(norm);
        
        float distance = norm*log_norm/dnorm;
        
        sf::Vector2f gradient = (1.f + log_norm)/(norm*dnorm)*(z.x*gx + z.y*gy) - distance/dnorm2*(dz.x*gdx + dz.y*gdy);
        
        return {distance, -gradient/sqrt(gradient.x*gradient.x + gradient.y*gradient.y)};
    }
}


