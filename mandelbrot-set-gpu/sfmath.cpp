//
//  sfmath.cpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 15/07/2021.
//

#include "sfmath.hpp"

float sf::math::dot(const sf::Vector2f & u, const sf::Vector2f & v) {
    return u.x * v.x + u.y * v.y;
}

float sf::math::norm(const sf::Vector2f & u) {
    return sqrt(dot(u, u));
}

sf::Vector2f sf::math::cmul(const sf::Vector2f & u, const sf::Vector2f & v) {
    return sf::Vector2f(u.x * v.x - u.y * v.y, u.x * v.y + u.y * v.x);
}
