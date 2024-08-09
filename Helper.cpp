#include "Helper.h"


sf::Vector2f reflect(const sf::Vector2f& vec, const sf::Vector2f& norm) {
    float dotProduct = vec.x * norm.x + vec.y * norm.y;
    sf::Vector2f reflectedVec = vec - 2 * dotProduct * norm;
    return reflectedVec;
}


float fastInvSqrt(float number) {
    const float threehalfs = 1.5F;

    float x2 = number * 0.5F;
    float y = number;
    long i = *reinterpret_cast<long*>(&y);   // Evil floating-point bit level hacking
    i = 0x5f3759df - (i >> 1);               // What the magic number does
    y = *reinterpret_cast<float*>(&i);
    y = y * (threehalfs - (x2 * y * y));     // 1st iteration
    y = y * (threehalfs - (x2 * y * y));  // 2nd iteration, can be used for more accuracy

    return y;
}

// Function to normalize a vector using fast inverse square root
sf::Vector2f normalize(const sf::Vector2f& vec) {
    float lengthSquared = vec.x * vec.x + vec.y * vec.y;
    float invLength = fastInvSqrt(lengthSquared);
    return sf::Vector2f(vec.x * invLength, vec.y * invLength);
}

float distance(sf::Vector2f& a, sf::Vector2f& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

float dot(const sf::Vector2f& vec1, const sf::Vector2f& vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

float magnitude(sf::Vector2f v) {
    return std::sqrt(dot(v, v));
}

sf::Vector2f componentAlongNormal(const sf::Vector2f& vec, const sf::Vector2f& norm) {
    sf::Vector2f normalizedNorm = normalize(norm);
    float projectionLength = std::max(dot(vec, normalizedNorm),0.0f);
    return projectionLength * normalizedNorm;
}

float clamp(float value, float min, float max) {
    return std::max(min, std::min(value, max));
}