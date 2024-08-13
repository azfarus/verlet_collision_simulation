#pragma once
#include <random>
#include  <chrono>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include<sstream>
#include<boost/geometry.hpp>
#include<boost/geometry/index/rtree.hpp>
#include "Helper.h"

class Shape;

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef std::pair<box, Shape *> rtree_value;


double getRandomdouble(double min, double max);


#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 1200
#define BOX_WIDTH 280.0f
#define BOX_HEIGHT 200.0f
#define g_VALUE sf::Vector2f(0,-800)
#define BALL_COUNT 5000
#define BALL_RADIUS 5
#define FRAME_COUNT 1500
#define FPS 60.0
#define FRAME_TIME (1.0/FPS)
#define MAX_VEL_COMPONENT 1000.0f
#define SUBFRAMES 25.0
#define EFFICIENCY .95f
#define SPACING .5
#define OFFSET 100.0
#define COLLISION_OFFSET .8f
#define SINGLE_SPAWN_FRAME_INTERVAL 1
#define DRAG 0.001f
#define FILENAME "texture/amongus2.jpg"



#define RANDOM_VEC2F(min,max) \
    sf::Vector2f( getRandomdouble(min,max) , getRandomdouble(min,max) )
