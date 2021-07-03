//
//  main.cpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 03/07/2021.
//

#include <SFML/Graphics.hpp>

int main(int argc, const char * argv[]) {
    const std::string PATH = "/Users/stephen/Documents/GitHub/mandelbrot-set-gpu/mandelbrot-set-gpu/";
    
    // Init window
    const int WIDTH = 1500;
    const int HEIGHT = 1000;
    
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set with GPU");
    window.setFramerateLimit(30);
    
    
    // Init texture
    const int TEXTURE_WIDTH = 1000;
    const int TEXTURE_HEIGHT = (float) HEIGHT/WIDTH*TEXTURE_WIDTH;
    
    sf::RenderTexture MS_texture;
    MS_texture.create(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    MS_texture.clear(sf::Color::Black);
    
    
    // Init shader
    sf::Shader MS_shader;
    MS_shader.loadFromFile(PATH + "mandelbrot.frag", sf::Shader::Fragment);
    MS_shader.setUniform("resolution", sf::Glsl::Vec2(TEXTURE_WIDTH, TEXTURE_HEIGHT));
    

    // Init program
    sf::Sprite sprite;
    sprite.setScale((float) WIDTH/TEXTURE_WIDTH, (float) HEIGHT/TEXTURE_HEIGHT);
    sprite.setTexture(MS_texture.getTexture());
    MS_texture.draw(sprite, &MS_shader);
    MS_texture.display();
    sprite.setTexture(MS_texture.getTexture());
    
    
    // Main loop of the program
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }
        
        window.clear();
        
        window.draw(sprite);
        
        window.display();
    }
    
    return 0;
}
