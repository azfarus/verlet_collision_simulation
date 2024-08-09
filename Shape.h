#pragma once
#include "Macros.h"



class Shape: public sf::CircleShape
{

private:
	static int x ;
	int id;

	sf::Vector2<float> new_pos;
	sf::Vector2<float> new_vel;
	std::vector<Shape*> neighbors;

public:
	sf::Vector2<float> cur_pos;
	sf::Vector2<float> cur_vel;
	sf::Vector2<float> cur_acc;
	float mass;
	
    Shape(float radius) : sf::CircleShape(radius) {


		id = x++;
		this->mass = 1.0f;
		this->cur_pos = this->cur_vel = this->cur_acc = sf::Vector2f(0, 0);
		this->setOrigin(radius , radius );
		this->setPosition(cur_pos);

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
		new_pos = cur_pos;

		sf::Vector2f norm ;

		float offset_x = getRadius() + BOX_WIDTH;
		float offset_y = getRadius() + BOX_HEIGHT;

		if (cur_pos.x < offset_x) {
			norm = sf::Vector2f(1, 0);
			new_vel = reflect(cur_vel, norm)*EFFICIENCY;
			
			sf::Vector2f nudge = std::abs(cur_pos.x - offset_x) * norm;
			new_pos += nudge;
		}
		else if (cur_pos.x > (SCREEN_WIDTH - offset_x)) {
			norm = sf::Vector2f(-1, 0);
			new_vel = reflect(cur_vel, norm) * EFFICIENCY;

			sf::Vector2f nudge = std::abs(cur_pos.x - (SCREEN_WIDTH - offset_x)) * norm;
			new_pos += nudge;
		}
		else if (cur_pos.y > (SCREEN_HEIGHT - getRadius())) {
			norm = sf::Vector2f(0, -1);
			new_vel = reflect(cur_vel, norm) * EFFICIENCY;

			sf::Vector2f nudge = std::abs(cur_pos.y - (SCREEN_HEIGHT	 - getRadius())) * norm;
			new_pos += nudge;
		}
		else if (cur_pos.y < offset_y) {
			norm = sf::Vector2f(0, 1);
			new_vel = reflect(cur_vel, norm) * EFFICIENCY;

			sf::Vector2f nudge = std::abs(cur_pos.y - offset_y) * norm;
			new_pos += nudge;
		}
		else {
			new_vel = cur_vel;
		}

		
		for (auto ball : balls) {
			
			if (*ball != *this) {
				float dist = distance(ball->cur_pos, this->cur_pos);
				if (dist < (ball->getRadius() + this->getRadius()-2.0)) {
					neighbors.push_back(ball);
				}
			}
		}

		for (auto ball : neighbors) {
			handleCollision(ball,neighbors.size());
		}

		neighbors.clear();



	}
	void handleCollision(Shape* other,size_t neighborcnt ) {
		sf::Vector2f norm = (this->cur_pos - other->cur_pos);
		

		float dist = magnitude(norm);
		norm = normalize(norm);
		float intersect_dist = ((this->getRadius() + other->getRadius()) -dist) / (2);
		
		if (intersect_dist > COLLISION_OFFSET)
		new_pos += intersect_dist * norm;
		sf::Vector2f relativeVel = componentAlongNormal((this->new_vel - other->cur_vel), -norm);

		new_vel -= (EFFICIENCY) * (2 * this->mass / (this->mass + other->mass)) * relativeVel ;

		new_vel.x = clamp(new_vel.x, -MAX_VEL_COMPONENT, MAX_VEL_COMPONENT);
		new_vel.y = clamp(new_vel.y, -MAX_VEL_COMPONENT, MAX_VEL_COMPONENT);
	}

	void calculate(float delta) {
		new_pos += new_vel * delta + 0.5f * cur_acc * delta * delta;
		new_vel += cur_acc * delta;
	}
	void update() {
		cur_pos = new_pos;
		cur_vel = new_vel;
	 }


	
};




