//
//  sfmath.hpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 15/07/2021.
//

#ifndef sfmath_hpp
#define sfmath_hpp

#include <cmath>
#include <SFML/Graphics.hpp>

namespace sf {
namespace math {


using value_gradient = std::pair<float, sf::Vector2f>;

// constants
const sf::Vector2f real_unit = sf::Vector2f(1.f, 0.f);
const sf::Vector2f imag_unit = sf::Vector2f(0.f, 1.f);
const sf::Vector2f zero = sf::Vector2f(0.f, 0.f);

// Dot product
float           dot(const sf::Vector2f &, const sf::Vector2f &);

// Norm operator
float           norm(const sf::Vector2f &);

// Complex multiplication
sf::Vector2f    cmul(const sf::Vector2f &, const sf::Vector2f &);

}
}

#endif /* sfmath_hpp */
