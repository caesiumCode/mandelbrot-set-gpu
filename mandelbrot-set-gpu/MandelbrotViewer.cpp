//
//  MandelbrotViewer.cpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 08/07/2021.
//

#include "MandelbrotViewer.hpp"

MandelbrotViewer::MandelbrotViewer() {
    TEXTURE_WIDTH = 1;
    TEXTURE_HEIGHT = 1;
    
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
}

void MandelbrotViewer::set_rendering_settings(unsigned int w, unsigned int h) {
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
    
    update_texture();
}

void MandelbrotViewer::set_previous_state_optimization(bool flag) {
    previous_state_flag = flag;
    
    shader.setUniform("previous_state_flag", previous_state_flag);
}

void MandelbrotViewer::update_offset(int pixel_dx, int pixel_dy) {
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
    
    update_texture();
}

void MandelbrotViewer::update_scale(float factor) {
    scale *= factor;
    shader.setUniform("zoom", sf::Glsl::Vec3(offset_x, offset_y, scale));
    
    update_texture();
}

void MandelbrotViewer::increase_limit() {
    if (limit < 10)
        limit++;
    else
        limit *= 1.1;
    
    shader.setUniform("limit", limit);
    
    update_texture();
}

void MandelbrotViewer::decrease_limit() {
    if (limit > 0) {
        if (limit < 10)
            limit--;
        else
            limit *= 0.9;
        
        shader.setUniform("limit", limit);
    }
    
    update_texture();
}

void MandelbrotViewer::refresh() {
    update_texture();
}

void MandelbrotViewer::update_texture() {
    // Save previous state
    previous_state.update(texture.getTexture());
    
    // Update texture
    texture.clear();
    texture.draw(sprite, &shader);
    texture.display();
}

void MandelbrotViewer::draw(sf::RenderWindow & window) {
    window.draw(sprite);
}
