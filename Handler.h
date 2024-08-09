#pragma once
#include <omp.h>
#include"Shape.h"
#include<vector>
#include<algorithm>
#include<iostream>


class Handler
{
private:
	std::vector<Shape*> balls;
	double radius;
	int ballcount;

public:
	Handler(int ball_count, double radius) {

		this->radius = radius;
		this->ballcount = ball_count;

	}

	void rectSpawner(sf::Vector2f start, sf::Vector2f end) {

		for (int i = 0; i < this->ballcount; i++) {

			sf::Vector2f pos = sf::Vector2f(getRandomdouble(radius, SCREEN_WIDTH - radius), getRandomdouble(radius, SCREEN_HEIGHT - radius));

			singleSpawner(pos);
		}

		double x = start.x + radius;
		double y = start.y + radius;

		for (auto ball : balls) {

			sf::Vector2f pos = sf::Vector2f(x, y);
			*ball = pos;
			x += radius * 2 + 5;
			if (x > end.x) {
				x = start.x + radius;
				y += radius * 2 + 5;
			}
			std::cout << std::string(*ball) <<std::endl;
		}


	}
	void singleSpawner(sf::Vector2f position , sf::Vector2f acc= sf::Vector2f(0,0), sf::Vector2f vel = sf::Vector2f(0, 0)) {
		
		if (this->balls.size() >= this->ballcount) return;
		Shape* ball = new Shape(this->radius);

		*ball = position;
		ball->cur_acc = acc;
		ball->cur_vel = vel;
		this->balls.push_back(ball);
	}
	void setBallVel(sf::Vector2f vel) {
		for (auto x : balls) {
			x->cur_vel = vel;
		}
	}
	void setBallVel(int idx , sf::Vector2f vel) {
		balls[idx]->cur_vel = vel;
	}
	void setBallAcc(sf::Vector2f acc) {
		for (auto x : balls) {
			x->cur_acc = acc;
		}
	}
	void setBallAcc(int idx, sf::Vector2f acc) {
		balls[idx]->cur_acc = acc;
	}
	void draw(sf::RenderTexture& renderTexture) {
		renderTexture.clear();
		for (auto x : balls) {
			x->setPosition(x->cur_pos);
			renderTexture.draw(*x);
		}
	}
	void update(float delta) {
		for (auto ball : balls) {
			ball->collisionCheck(balls);
		}
		for (auto ball : balls) {
			ball->calculate(delta);
		}
		for (auto ball : balls) {
			ball->update();
		}
	}



	
};

