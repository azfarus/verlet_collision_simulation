#pragma once
#include <random>
#include  <chrono>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include<sstream>
#include "Helper.h"

double getRandomdouble(double min, double max);


#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 1200
#define BOX_WIDTH 280.0f
#define BOX_HEIGHT 200.0f
#define g_VALUE sf::Vector2f(0,-400)
#define BALL_COUNT 1024
#define BALL_RADIUS 10
#define FRAME_COUNT 1500
#define FPS 30.0
#define FRAME_TIME (1.0/FPS)
#define MAX_VEL_COMPONENT 1000.0f
#define SUBFRAMES 50.0
#define EFFICIENCY 0.65f
#define SPACING .5
#define OFFSET 100.0
#define COLLISION_OFFSET .8f
#define SINGLE_SPAWN_FRAME_INTERVAL 4
#define DRAG 0.001f




#define RANDOM_VEC2F(min,max) \
    sf::Vector2f( getRandomdouble(min,max) , getRandomdouble(min,max) )
