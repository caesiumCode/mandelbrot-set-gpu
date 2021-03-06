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
    
    sf::Glsl::Vec2 directions[8];
    directions[0] = sf::Glsl::Vec2(-1.0,   0.0);
    directions[1] = sf::Glsl::Vec2(-1.0,   1.0);
    directions[2] = sf::Glsl::Vec2(0.0,    1.0);
    directions[3] = sf::Glsl::Vec2(1.0,    1.0);
    directions[4] = sf::Glsl::Vec2(1.0,    0.0);
    directions[5] = sf::Glsl::Vec2(1.0,    -1.0);
    directions[6] = sf::Glsl::Vec2(0.0,    -1.0);
    directions[7] = sf::Glsl::Vec2(-1.0,   -1.0);
    shader.setUniformArray("directions", directions, 8);
    shader.setUniform("nb_directions", 8);
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
    
    refresh();
}

void mv::RenderMandelbrot::update_scale(float factor) {
    scale *= factor;
    shader.setUniform("zoom", sf::Glsl::Vec3(offset_x, offset_y, scale));
    shader.setUniform("previous_state_position", sf::Glsl::Vec4((float) (1.f - 1.f/factor) * TEXTURE_WIDTH / 2.f,
                                                                (float) (1.f - 1.f/factor) * TEXTURE_HEIGHT / 2.f,
                                                                (float) TEXTURE_WIDTH / factor,
                                                                (float) TEXTURE_HEIGHT / factor));
    
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

sf::Vector3f mv::RenderMandelbrot::getLocalisation() {
    return sf::Vector3f(offset_x, offset_y, scale);
}

int mv::RenderMandelbrot::getLimit() {
    return limit;
}

void mv::RenderMandelbrot::refresh() {
    // Find reference point
    find_reference_point();
    
    // Save previous state
    previous_state.update(texture.getTexture());
    shader.setUniform("previous_state", previous_state);
    
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
    
    sf::Vector2f z = sf::math::zero;    // z
    sf::Vector2f dz = sf::math::zero;   // z'
    
    sf::Vector2f gx = sf::math::zero;   // gradient of Re(z)
    sf::Vector2f gy = sf::math::zero;   // gradient of Im(z)
    
    sf::Vector2f gdx = sf::math::zero;  // gradient of Re(z')
    sf::Vector2f gdy = sf::math::zero;  // gradient of Im(z')
    
    int n = 0;
    while (sf::math::dot(z,z) <= 1.0e+10 && n < limit) {
        sf::Vector2f temp_gx = gx;
        sf::Vector2f temp_gdx = gdx;
        
        gdx = 2.f*dz.x*gx - 2.f*dz.y*gy + 2.f*z.x*gdx - 2.f*z.y*gdy;
        gdy = 2.f*dz.x*gy + 2.f*dz.y*gx + 2.f*z.x*gdy + 2.f*z.y*temp_gdx;
        
        gx = 2.f*z.x*gx - 2.f*z.y*gy        + sf::math::real_unit;
        gy = 2.f*z.x*gy + 2.f*z.y*temp_gx   + sf::math::imag_unit;
        
        dz = 2.f * sf::math::cmul(z, dz)    + sf::math::real_unit;
        
        z = sf::math::cmul(z, z) + c;
        
        n++;
    }
    
    if (n == limit) {
        return {0.f, sf::math::zero};
    } else {
        float norm2 = sf::math::dot(z,z), dnorm2 = sf::math::dot(dz,dz);
        float norm = sqrt(norm2), dnorm = sqrt(dnorm2);
        float log_norm = log(norm);
        
        float distance = norm*log_norm/dnorm;
        
        sf::Vector2f gradient = (1.f + log_norm)/(norm*dnorm)*(z.x*gx + z.y*gy) - distance/dnorm2*(dz.x*gdx + dz.y*gdy);
        
        return {distance, -gradient/sf::math::norm(gradient)};
    }
}


