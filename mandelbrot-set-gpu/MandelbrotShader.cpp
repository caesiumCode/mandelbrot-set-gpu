//
//  MandelbrotShader.cpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 23/07/2021.
//

#include "MandelbrotShader.hpp"

std::string mv::getParameterString(mv::Parameter parameter) {
    switch (parameter) {
        case mv::Parameter::resolution:
            return "resolution";
            break;
            
        case mv::Parameter::zoom:
            return "zoom";
            break;
            
        case mv::Parameter::limit:
            return "limit";
            break;
            
        case mv::Parameter::previous_state_flag:
            return "previous_state_flag";
            break;
            
        case mv::Parameter::previous_state_position:
            return "previous_state_position";
            break;
            
        case mv::Parameter::previous_state_texture:
            return "previous_state_texture";
            break;
            
        case mv::Parameter::directions:
            return "directions";
            break;
            
        default:
            break;
    }
}

mv::MandelbrotShader::MandelbrotShader() {
    // Shader
    shader.loadFromFile("mandelbrot.frag", sf::Shader::Fragment);
    
    // Texture dimensions
    texture_width = nullptr;
    texture_height = nullptr;
    
    // Computation constrains
    center = nullptr;
    scale = nullptr;
    limit = nullptr;
    
    // Render optimisation variables
    ps_flag = nullptr;
    ps_position = nullptr;
    ps_size = nullptr;
    ps_texture = nullptr;
    
    update_shader(mv::Parameter::directions);
}

void mv::MandelbrotShader::setTextureDimensions(int * w, int * h) {
    texture_width = w;
    texture_height = h;
    
    update_shader(mv::Parameter::resolution);
}
void mv::MandelbrotShader::setComputationtConstrains(sf::Vector2f * c, float * s, int * l) {
    center = c;
    scale = s;
    limit = l;
    
    update_shader(mv::Parameter::zoom);
    update_shader(mv::Parameter::limit);
}
void mv::MandelbrotShader::setOptimisationVariables(bool * f, sf::Vector2i * p, sf::Vector2i * s, sf::Texture * t) {
    ps_flag = f;
    ps_position = p;
    ps_size = s;
    ps_texture = t;
    
    update_shader(mv::Parameter::previous_state_flag);
    update_shader(mv::Parameter::previous_state_position);
    update_shader(mv::Parameter::previous_state_texture);
}

sf::Shader* mv::MandelbrotShader::getShaderPtr() {
    return &shader;
}

void mv::MandelbrotShader::update_shader(mv::Parameter parameter) {
    switch (parameter) {
        case mv::Parameter::resolution:
            shader.setUniform(mv::getParameterString(parameter), sf::Glsl::Vec2(*texture_width, *texture_height));
            break;
            
        case mv::Parameter::zoom:
            shader.setUniform(mv::getParameterString(parameter), sf::Glsl::Vec3(center->x, center->x, *scale));
            break;
            
        case mv::Parameter::limit:
            shader.setUniform(mv::getParameterString(parameter), *limit);
            break;
            
        case mv::Parameter::previous_state_texture:
            shader.setUniform(mv::getParameterString(parameter), *ps_texture);
            break;
            
        case mv::Parameter::previous_state_position:
            shader.setUniform(mv::getParameterString(parameter),
                                     sf::Glsl::Vec4(ps_position->x, ps_position->y, ps_size->x, ps_size->y));
            break;
            
        case mv::Parameter::previous_state_flag:
            shader.setUniform(mv::getParameterString(parameter), *ps_flag);
            break;
            
        case mv::Parameter::directions:
            const std::vector<sf::Glsl::Vec2> directions = {
                sf::Glsl::Vec2(-1.0,   0.0),
                sf::Glsl::Vec2(-1.0,   1.0),
                sf::Glsl::Vec2(0.0,    1.0),
                sf::Glsl::Vec2(1.0,    1.0),
                sf::Glsl::Vec2(1.0,    0.0),
                sf::Glsl::Vec2(1.0,    -1.0),
                sf::Glsl::Vec2(0.0,    -1.0),
                sf::Glsl::Vec2(-1.0,   -1.0)
            };
            int directions_size = static_cast<int>(directions.size());
            
            shader.setUniformArray(mv::getParameterString(parameter), &directions[0], directions_size);
            shader.setUniform("nb_directions", directions_size);
            break;
    }
}
