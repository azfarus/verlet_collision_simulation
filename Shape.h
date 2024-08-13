#pragma once
#include "Macros.h"
#include <sstream>
#include<iostream>



class Shape 
{

private:
	
	box _BB;

protected:
	sf::Shape* _shape;
	sf::Vector2f _tempVel;

public:
	static int _x ;
	int _id;
	float mass;
	sf::Vector2f pos, vel, acc;

public:

	Shape(sf::Shape* shape) {

		mass = 1.0f;

		_shape = shape;
		this->formAABB();
		_id = _x++;
		acc = g_VALUE;
	}

	void formAABB() {
		sf::FloatRect f= _shape->getGlobalBounds();
		
		float left = f.left;
		float right = f.left + f.width;

		float top = f.top;
		float bottom = f.top + f.height;

		_BB = box(point(left, top), point(right, bottom));


		return;
	}

	rtree_value getRTreeNode() {

		return rtree_value(_BB, this);
	}

	box getAABB() {

		return _BB;
	}

	void update(float delta) {

		pos += vel * delta + 0.5f * acc * delta * delta;
		vel += (acc * delta - DRAG*vel);

	}

	virtual void setPosition() {
		_shape->setPosition(pos);
	}

	bool operator==(Shape& other) {
		return _id == other._id;
	}

	virtual sf::Shape* getShape() {
		return _shape;
	}

	virtual void wallCheck()=0;
	virtual void info() = 0;

};


class Ball : public Shape
{
private:
	
	sf::CircleShape* _circleShape;
	bool checked = false;

public:
	
	Ball(float radius) : Shape(new sf::CircleShape(radius)) {
		_circleShape = static_cast<sf::CircleShape*>(_shape);
		_circleShape->setOrigin(sf::Vector2f(radius, radius));
		_circleShape->setFillColor(sf::Color::Transparent);
		//_circleShape->setOutlineColor(sf::Color::White);
		//_circleShape->setOutlineThickness(-(BALL_RADIUS/5.0f));
	}

	void wallCheck() {
		//_tempVel = vel;

		float offsetXL = _circleShape->getRadius() + BOX_WIDTH;
		float offsetXR = SCREEN_WIDTH - offsetXL;
		float offsetYB = _circleShape->getRadius() + BOX_HEIGHT;
		float offsetYT = SCREEN_HEIGHT - _circleShape->getRadius();

		sf::Vector2f normal = sf::Vector2f(0.0f, 0.0f);
		if (pos.x <= offsetXL) {
			normal.x = 1.0f;
			vel = reflect(vel, normal)*EFFICIENCY;
		}
		else if (pos.x >= offsetXR) {
			normal.x = -1.0f;
			vel = reflect(vel, normal) * EFFICIENCY;
		}
		if (pos.y <= offsetYB) {
			normal.y = 1.0f;
			vel = reflect(vel, normal) * EFFICIENCY;
		}
		else if (pos.y >= offsetYT) {
			normal.y = -1.0f;
			vel = reflect(vel, normal) * EFFICIENCY;
		}

		pos.x = clamp(pos.x, offsetXL, offsetXR);
		pos.y = clamp(pos.y, offsetYB, offsetYT);
	}

	void setPosition() {
		_circleShape->setPosition(pos);
	}

	sf::CircleShape* getShape() {
		return _circleShape;
	}


	void info() {
		std::cout << "Ball: " << pos.x << " " << pos.y << "\n";
	}


};




