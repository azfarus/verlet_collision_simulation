#pragma once
#include <omp.h>
#include"Shape.h"
#include<vector>
#include<algorithm>
#include<iostream>
#include<algorithm>
#include <unordered_map>
#include<boost/geometry.hpp>
#include<boost/geometry/index/rtree.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef std::pair<box, unsigned> rtree_value;

class Handler
{
private:
	std::unordered_map<size_t,std::vector<size_t>> collidingBalls;
	double radius;
	int ballcount;

	Shape* lowball = new Shape(BALL_RADIUS);
	Shape* highball = new Shape(BALL_RADIUS);

public:
	std::vector<Shape*> balls;
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
	void update_structured(float delta) {
		
		/*std::sort(balls.begin(), balls.end(), [](Shape* a, Shape* b) {
			return a->cur_pos.x <= b->cur_pos.x;
			});*/
		for (size_t i = 0; i < balls.size(); i++) {
			
			balls[i]->wallCheck();
			float cur_x = balls[i]->cur_pos.x;


		
			lowball->cur_pos.x = cur_x - balls[i]->getRadius();
			highball->cur_pos.x = cur_x + balls[i]->getRadius();

			auto lower = std::lower_bound(balls.begin(), balls.end(), lowball, [](Shape* ball, Shape* _ball) {
				return ball->cur_pos.x < ball->cur_pos.x;
				});
		
			auto upper = std::upper_bound(balls.begin(), balls.end(), highball, [](Shape* ball, Shape* _ball) {
				return ball->cur_pos.x < _ball->cur_pos.x;
				});

			size_t low = lower - balls.begin();
			size_t high = upper - balls.begin();

			for (int j = low; j < high; j++) {
				
				float dist = distance(balls[i]->cur_pos, balls[j]->cur_pos);
				float collision_thresh = balls[i]->getRadius() + balls[j]->getRadius();
				if (dist <= collision_thresh) {
					handleCollision(balls[i], balls[j],collision_thresh - dist);
				}
			}


			float totalOverlap = 0.0f;
			do {
				totalOverlap = 0.0f;
				for (int k = low; k < high;k++) {


				if(k!=i) totalOverlap += separateBalls(balls[i], balls[k]);

				}
			} while (totalOverlap > 0.2f * (upper-lower+1));
		}
		//for (size_t i = 0; i < balls.size(); i++) {

		//	balls[i]->wallCheck();
		//	for (size_t j = i + 1; j < balls.size(); j++) {
		//		
		//		float dist = distance(balls[i]->cur_pos, balls[j]->cur_pos);
		//		float dist_new = distance(balls[i]->new_pos, balls[j]->new_pos);
		//		float collision_thresh = balls[i]->getRadius() + balls[j]->getRadius();

		//		if (dist <= collision_thresh) {
		//			handleCollision(balls[i], balls[j],collision_thresh - dist);
		//		}
		//		
		//		if (dist_new <= collision_thresh) {
		//			collidingBalls[i].push_back(j);
		//		}
		//	}
		//}


		////std::cout << "Overlap: " << totalOverlap << std::endl;

		for (auto ball : balls) {
			ball->calculate(delta);
		}
		for (auto ball : balls) {
			ball->update();
		}
	}

	float separateBalls(Shape* b1, Shape* b2) {
		sf::Vector2f norm = normalize(b2->new_pos - b1->new_pos);

		float dist = distance(b1->new_pos, b2->new_pos);
		float collision_thresh = b1->getRadius() + b2->getRadius();
		float nudge = collision_thresh - dist;

		if (nudge > 0.0f) {
			b1->new_pos -= (nudge / 2) * norm;
			b2->new_pos += (nudge / 2) * norm;
			return nudge;
		}
		return 0.0f;
	}

	void handleCollision(Shape* b1, Shape* b2 , float nudge) {
		sf::Vector2f norm = normalize(b2->cur_pos - b1->cur_pos);
		sf::Vector2f vrel = b2->cur_vel - b1->cur_vel;

		b1->new_pos -= (nudge/2.0f) * norm;
		b2->new_pos += (nudge/2.0f) * norm;

		float vrel_comp = dot(vrel, norm);
		if (vrel_comp > 0) return;

		vrel = vrel_comp * norm;
		float j = (-(1.0f + EFFICIENCY) / ((1.0f / b1->mass) + (1.0f / b2->mass)));

		b1->new_vel += (j / b1->mass) * norm;
		b2->new_vel -= (j / b2->mass) * norm;

		b1->new_vel.x = clamp(b1->new_vel.x, -MAX_VEL_COMPONENT, MAX_VEL_COMPONENT);
		b1->new_vel.y = clamp(b1->new_vel.y, -MAX_VEL_COMPONENT, MAX_VEL_COMPONENT);

		b2->new_vel.x = clamp(b2->new_vel.x, -MAX_VEL_COMPONENT, MAX_VEL_COMPONENT);
		b2->new_vel.y = clamp(b2->new_vel.y, -MAX_VEL_COMPONENT, MAX_VEL_COMPONENT);


	}


	
};

