#pragma once
#include <omp.h>
#include"Shape.h"
#include<vector>
#include<algorithm>
#include<iostream>
#include<algorithm>
#include <unordered_map>




class Handler
{
private:
	
	uint32_t _ballcount;
	std::vector<Shape*> _shapes;
	bgi::rtree < rtree_value, bgi::rstar<16 >> _rtree;

	std::unordered_map<int, sf::Color> _colorMap;
	std::vector<rtree_value> neighbors;

public:
	Handler(uint32_t ballcount) : _ballcount(ballcount) {

	}

	bool spawnSingle(sf::Vector2f pos,sf::Vector2f vel,float radius) {
		
		if (_ballcount <= _shapes.size()) return false;
		
		Ball* ball = new Ball(radius);
		ball->pos = pos;
		ball->vel = vel;

		sf::Color col = getColor(ball->_id);
		ball->getShape()->setFillColor(col);
		ball->formAABB();


		_shapes.push_back(ball);
		_rtree.insert(ball->getRTreeNode());
		
		//ball->info();
		
		return true;
	}




	void update(float delta) {

		for (auto shape : _shapes) {
			
			neighbors.clear();
			
			rtree_value node = shape->getRTreeNode();
			_rtree.query(bgi::overlaps(shape->getAABB()), std::back_inserter(neighbors));
			neighbors.push_back(node);
			/*for (auto other : neighbors) {
				if(other.second->_id != shape->_id)shape->collisionCheck(other.second,neighbors.size());
			}*/
			
			
			shape->wallCheck();
			float totOverlap = 0.0f;
			do {
				totOverlap = 0.0f;

				for (int i = 0; i < neighbors.size(); i++) {
					for (int j = i+1; j < neighbors.size(); j++) {
						totOverlap+=handleCollision(neighbors[i].second,neighbors[j].second);
					}
				}
			} while (totOverlap > 0.1 * neighbors.size());

			if(_rtree.remove(node)==0) std::cout<<"miss ";
			else {
				shape->formAABB();
				node = shape->getRTreeNode();
				_rtree.insert(node);
			}
		}

		for (auto shape : _shapes) {
			shape->update(delta);
			shape->setPosition();
		}
	

	}


	float handleCollision(Shape* a,Shape*b) {

		Ball* b1 = static_cast<Ball*>(a);
		Ball* b2 = static_cast<Ball*>(b);


		float dist = distance(b1->pos, b2->pos);
		float collThresh = b1->getShape()->getRadius() + b2->getShape()->getRadius();
		if (dist > collThresh) return 0.0f;

		float nudge = collThresh - dist;
		sf::Vector2f norm = normalize(b2->pos - b1->pos);
		b2->pos += (nudge / 2) * norm;
		b1->pos -= (nudge / 2) * norm;

		float vrelComp = dot(b2->vel - b1->vel, norm);

		if (vrelComp > 0) return 0.0f;

		float j = (-(1.0 + EFFICIENCY) / ((1.0f / b1->mass) + (1.0f / b2->mass)));

		b1->vel -= (j / b1->mass) * vrelComp * norm;
		b2->vel += (j / b2->mass) * vrelComp * norm;

		b1->vel.x = clamp(b1->vel.x, -MAX_VEL_COMPONENT, MAX_VEL_COMPONENT);
		b1->vel.y = clamp(b1->vel.y, -MAX_VEL_COMPONENT, MAX_VEL_COMPONENT);

		b2->vel.x = clamp(b2->vel.x, -MAX_VEL_COMPONENT, MAX_VEL_COMPONENT);
		b2->vel.y = clamp(b2->vel.y, -MAX_VEL_COMPONENT, MAX_VEL_COMPONENT);
		return nudge;
	}

	void draw(sf::RenderTexture& renderTexture) {

		for (auto shape : _shapes) {
			renderTexture.draw(*shape->getShape());
		}
		renderTexture.display();
	}

	void registerColor(sf::RenderTexture& rt) {
		sf::Image finalframe = rt.getTexture().copyToImage();
		for (auto x : _shapes) {
			_colorMap[x->_id] = finalframe.getPixel(x->pos.x, x->pos.y);
		}
		std::cout << "Size: " << _colorMap.size() << '\n';

	}

	void resetHandler() {
		_rtree.clear();
		neighbors.clear();
		_shapes.clear();
		Shape::_x = 0;
	}


	void printColorMap() {
		printf_s("shapes.size %d", _shapes.size());
	}

	sf::Color getColor(int id) {

		if (_colorMap.find(id) == _colorMap.end()) { 
			//std::cout << "nocolor\n";
			return sf::Color::Transparent; 
		}
		return _colorMap[id];
	}
	
};

