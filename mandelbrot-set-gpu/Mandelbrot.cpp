//
//  Mandelbrot.cpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 22/07/2021.
//

#include "Mandelbrot.hpp"

mv::Mandelbrot::Mandelbrot() {
    TEXTURE_WIDTH = 2000;
    TEXTURE_HEIGHT = 1500;
    
    // Computation constrains
    center = sf::math::zero;
    scale = 5.f;
    limit = 100;
    
    // Optimisation variables
    ps_flag = false;
    ps_position = sf::Vector2i(0, 0);
    ps_size = sf::Vector2i(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    ps_texture.create(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    
    // Render variables
    render_texture.create(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    render_shader.setTextureDimensions(&TEXTURE_WIDTH, &TEXTURE_HEIGHT);
    render_shader.setComputationtConstrains(&center, &scale, &limit);
    render_shader.setOptimisationVariables(&ps_flag, &ps_position, &ps_size, &ps_texture);
        
    // Perturbation theory variables
    gradient_path.clear();
    reference_point = sf::math::zero;
}

void mv::Mandelbrot::set_texture_dimensions(int w, int h) {
    TEXTURE_WIDTH = w;
    TEXTURE_HEIGHT = h;
    
    // Computation constrains
    center = sf::math::zero;
    scale = 5.f;
    limit = 100;
    
    // Optimisation variables
    ps_flag = false;
    ps_position = sf::Vector2i(0, 0);
    ps_size = sf::Vector2i(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    ps_texture.create(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    
    // Render variables
    render_texture.create(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    render_shader.setTextureDimensions(&TEXTURE_WIDTH, &TEXTURE_HEIGHT);
    render_shader.setComputationtConstrains(&center, &scale, &limit);
    render_shader.setOptimisationVariables(&ps_flag, &ps_position, &ps_size, &ps_texture);
        
    // Perturbation theory variables
    gradient_path.clear();
    reference_point = sf::math::zero;
    
    render();
}

void mv::Mandelbrot::update_center(sf::Vector2i dpixel) {
    center -= scale * sf::Vector2f((float) dpixel.x/TEXTURE_WIDTH,
                                   (float) dpixel.y/TEXTURE_WIDTH);
    
    ps_position = dpixel;
    ps_size = sf::Vector2i(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    
    render_shader.update_shader(mv::Parameter::zoom);
    render_shader.update_shader(mv::Parameter::previous_state_position);
    
    ps_flag = true;
    render_shader.update_shader(mv::Parameter::previous_state_flag);
    
    render();
}

void mv::Mandelbrot::update_center(sf::Vector2f dposition) {
    center -= dposition;
    
    ps_position = sf::Vector2i((float) dposition.x/scale*TEXTURE_WIDTH,
                               (float) dposition.y/scale*TEXTURE_WIDTH);
    ps_size = sf::Vector2i(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    
    render_shader.update_shader(mv::Parameter::zoom);
    render_shader.update_shader(mv::Parameter::previous_state_position);
    
    ps_flag = true;
    render_shader.update_shader(mv::Parameter::previous_state_flag);
    
    render();
}

void mv::Mandelbrot::update_scale(float factor) {
    scale *= factor;
    ps_position = sf::Vector2i((float) (1.f - 1.f/factor) * TEXTURE_WIDTH / 2.f,
                               (float) (1.f - 1.f/factor) * TEXTURE_HEIGHT / 2.f);
    ps_size = sf::Vector2i((float) TEXTURE_WIDTH / factor,
                           (float) TEXTURE_HEIGHT / factor);
    
    render_shader.update_shader(mv::Parameter::zoom);
    render_shader.update_shader(mv::Parameter::previous_state_position);
    
    ps_flag = true;
    render_shader.update_shader(mv::Parameter::previous_state_flag);
    
    render();
}

void mv::Mandelbrot::increase_limit() {
    if (limit < 10)
        limit++;
    else
        limit *= 1.1;
    
    render_shader.update_shader(mv::Parameter::limit);
    
    ps_flag = false;
    render_shader.update_shader(mv::Parameter::previous_state_flag);
    
    render();
}

void mv::Mandelbrot::decrease_limit() {
    if (limit > 0) {
        if (limit < 10)
            limit--;
        else
            limit *= 0.9;
        
        render_shader.update_shader(mv::Parameter::limit);
    }
    
    ps_flag = false;
    render_shader.update_shader(mv::Parameter::previous_state_flag);
    
    render();
}

void mv::Mandelbrot::render() {
    // Find reference point
    find_reference_point();
    
    // Save previous state
    ps_texture.update(render_texture.getTexture());
    render_shader.update_shader(mv::Parameter::previous_state_texture);
    
    // Update texture
    sf::Sprite sprite;
    sprite.setTexture(ps_texture);
    
    render_texture.clear();
    render_texture.draw(sprite, render_shader.getShaderPtr());
    render_texture.display();
}

void mv::Mandelbrot::refresh() {
    bool tmp_ps_flag = ps_flag;
    
    ps_flag = false;
    render_shader.update_shader(mv::Parameter::previous_state_flag);
    
    render();
    
    ps_flag = tmp_ps_flag;
}

const float& mv::Mandelbrot::getScale() {
    return scale;
}

const sf::Vector2f& mv::Mandelbrot::getCenter() {
    return center;
}

const sf::Vector2f&  mv::Mandelbrot::getReferencePoint() {
    return reference_point;
}

const std::vector<sf::Vector2f>& mv::Mandelbrot::getGradientPath() {
    return gradient_path;
}

const sf::Texture& mv::Mandelbrot::getTexture() {
    return render_texture.getTexture();
}

void mv::Mandelbrot::find_reference_point() {
    sf::Vector2f point = center;
    
    sf::math::value_gradient distance = distance_estimation(point);
    
    
    gradient_path.clear();
    gradient_path.push_back(point);
    
    
    int keeper = 0;
    while (distance.first > (float) scale/TEXTURE_WIDTH && keeper < 100) {
        point.x += distance.second.x*distance.first;
        point.y += distance.second.y*distance.first;
        
        gradient_path.push_back(point);
                
        distance = distance_estimation(point);
        keeper++;
    }
        
    reference_point = point;
}


sf::math::value_gradient mv::Mandelbrot::distance_estimation(sf::Vector2f c) {
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
