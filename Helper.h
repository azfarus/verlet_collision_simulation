#pragma once
#include "Macros.h"


sf::Vector2f reflect(const sf::Vector2f& vec, const sf::Vector2f& norm);
float fastInvSqrt(float number);
sf::Vector2f normalize(const sf::Vector2f& vec);
float distance(sf::Vector2f& a, sf::Vector2f& b);
float dot(const sf::Vector2f& vec1, const sf::Vector2f& vec2);
sf::Vector2f componentAlongNormal(const sf::Vector2f& vec, const sf::Vector2f& norm);
float clamp(float value, float min, float max);
float magnitude(sf::Vector2f v);
float randomFloatInRange(float min_value, float max_value);