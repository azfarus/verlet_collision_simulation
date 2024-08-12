#pragma once
#include "Macros.h"
#include <sstream>
#include<iostream>



class Shape: public sf::CircleShape
{

private:
	static int x ;
	int id;

	std::vector<Shape*> neighbors;

public:
	sf::Vector2<float> cur_pos;
	sf::Vector2<float> cur_vel;
	sf::Vector2<float> cur_acc;

	sf::Vector2<float> new_pos;
	sf::Vector2<float> new_vel;


	float mass;
	
    Shape(float radius) : sf::CircleShape(radius) {


		id = x++;
		this->mass = randomFloatInRange(1,3);
		this->cur_pos = this->cur_vel = this->cur_acc = sf::Vector2f(0, 0);
		this->setOrigin(radius , radius );
		this->setPosition(cur_pos);
		this->setOutlineColor(sf::Color::Blue);
		this->setOutlineThickness(-1.0f);

	}

	Shape operator=(sf::Vector2f& pos) {
		cur_pos = pos;
		new_pos = pos;
		return *this;
	}
	bool operator==(Shape& other) {
		return other.id == this->id;
	}
	bool operator!=(Shape& other) {
		return other.id != this->id;
	}
	operator std::string() const {

		std::stringstream ss;
		ss << id << ": " << cur_pos.x << " " << cur_pos.y << " ";
		return ss.str();
	}

	void collisionCheck(std::vector<Shape*> balls) {


		sf::Vector2f norm ;

		float offset_x = getRadius() + BOX_WIDTH;
		float offset_y = getRadius() + BOX_HEIGHT;

		if (cur_pos.x < offset_x) {
			norm = sf::Vector2f(1, 0);
			new_vel = reflect(cur_vel, norm)*EFFICIENCY;
		}
		else if (cur_pos.x > (SCREEN_WIDTH - offset_x)) {
			norm = sf::Vector2f(-1, 0);
			new_vel = reflect(cur_vel, norm) * EFFICIENCY;
		}
		else if (cur_pos.y > (SCREEN_HEIGHT - getRadius())) {
			norm = sf::Vector2f(0, -1);
			new_vel = reflect(cur_vel, norm) * EFFICIENCY;
		}
		else if (cur_pos.y < offset_y) {
			norm = sf::Vector2f(0, 1);
			new_vel = reflect(cur_vel, norm) * EFFICIENCY;
		}
		else {
			new_vel = cur_vel;
		}

		new_pos.x = clamp(new_pos.x, offset_x, SCREEN_WIDTH - offset_x);
		new_pos.y = clamp(new_pos.y, offset_y, SCREEN_WIDTH - getRadius());
		
		for (auto ball : balls) {
			
			if (*ball != *this) {
				float dist = distance(ball->new_pos, this->new_pos);
				if (dist < (ball->getRadius() + this->getRadius())) {
					neighbors.push_back(ball);
				}
			}
		}


		for (auto ball : neighbors) {
			handleCollision(ball,neighbors.size());
		}
		

		neighbors.clear();



	}
	void wallCheck() {
		sf::Vector2f norm;

		float offset_x = getRadius() + BOX_WIDTH;
		float offset_y = getRadius() + BOX_HEIGHT;

		if (cur_pos.x < offset_x) {
			norm = sf::Vector2f(1, 0);
			new_vel = reflect(cur_vel, norm) * EFFICIENCY;
		}
		else if (cur_pos.x > (SCREEN_WIDTH - offset_x)) {
			norm = sf::Vector2f(-1, 0);
			new_vel = reflect(cur_vel, norm) * EFFICIENCY;
		}
		else if (cur_pos.y > (SCREEN_HEIGHT - getRadius())) {
			norm = sf::Vector2f(0, -1);
			new_vel = reflect(cur_vel, norm) * EFFICIENCY;
		}
		else if (cur_pos.y < offset_y) {
			norm = sf::Vector2f(0, 1);
			new_vel = reflect(cur_vel, norm) * EFFICIENCY;
		}
		else {
			new_vel = cur_vel;
		}

		new_pos.x = clamp(new_pos.x, offset_x, SCREEN_WIDTH - offset_x);
		new_pos.y = clamp(new_pos.y, offset_y, SCREEN_WIDTH - getRadius());
	}
	void handleCollision(Shape* other,size_t neighborcnt ) {
		sf::Vector2f norm = (this->cur_pos - other->cur_pos);
		

		float dist = magnitude(norm);
		norm = normalize(norm);
		float intersect_dist = std::fabs((this->getRadius() + other->getRadius()) - dist) / (2);


		new_pos += intersect_dist * norm;
		sf::Vector2f relativeVel = componentAlongNormal((this->new_vel - other->cur_vel), -norm);

		new_vel -= (EFFICIENCY) * (2 * this->mass / (this->mass + other->mass)) * relativeVel ;

		new_vel.x = clamp(new_vel.x, -MAX_VEL_COMPONENT, MAX_VEL_COMPONENT);
		new_vel.y = clamp(new_vel.y, -MAX_VEL_COMPONENT, MAX_VEL_COMPONENT);

	}

	void calculate(float delta) {
		new_pos += new_vel * delta + 0.5f * cur_acc * delta * delta;
		new_vel += (cur_acc * delta - new_vel*DRAG);
	}
	void update() {
		cur_pos = new_pos;
		cur_vel = new_vel;
	 }


	
};




