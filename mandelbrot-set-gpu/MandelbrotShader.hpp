//
//  MandelbrotShader.hpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 23/07/2021.
//

#ifndef MandelbrotShader_hpp
#define MandelbrotShader_hpp

#include <SFML/Graphics.hpp>

namespace mv {
enum Parameter {
    resolution,
    zoom,
    limit,
    previous_state_texture,
    previous_state_position,
    previous_state_flag,
    directions
};

std::string getParameterString(mv::Parameter);

class MandelbrotShader {
public:
    MandelbrotShader();
    
    void setTextureDimensions(int*, int*);
    void setComputationtConstrains(sf::Vector2f*, float*, int*);
    void setOptimisationVariables(bool*, sf::Vector2i*, sf::Vector2i*, sf::Texture*);
    
    sf::Shader* getShaderPtr();
    
    void update_shader(mv::Parameter);
    
private:
    // Shader
    sf::Shader shader;
    
    // Texture dimensions
    const int* texture_width;
    const int* texture_height;
    
    // Computation constrains
    sf::Vector2f* center;
    float* scale;
    int* limit;
    
    // Render optimisation variables
    bool* ps_flag;
    sf::Vector2i* ps_position;
    sf::Vector2i* ps_size;
    sf::Texture* ps_texture;
};
}
#endif /* MandelbrotShader_hpp */
