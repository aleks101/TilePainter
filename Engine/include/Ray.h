#pragma once

#include "Map.h"
#include "Tile.h"
#include "Physics.h"

class Ray
{
private:
	Vec2 m_currPos;

	bool m_hasReachedPoint;
public:
	Ray(Vec2 startPoint, Vec2 endPoint, Map* map) : m_currPos(startPoint) {
		Vec2 speed;
		bool running = true;

		while (running) {
			if (!map->CheckCollisionDest(m_currPos)) {
				speed = physics::CalculateVelocity(m_currPos, endPoint, 1);
				m_currPos = m_currPos + speed;
			}
			else {
				m_hasReachedPoint = false;
				running = false;
			}
			if (m_currPos == endPoint) {
				running = false;
				m_hasReachedPoint = true;
			}
			else if (((m_currPos.x > endPoint.x && m_currPos.x - 1 < endPoint.x) || (m_currPos.x < endPoint.x && m_currPos.x + 1 > endPoint.x)) &&
				((m_currPos.y > endPoint.y && m_currPos.y - 1 < endPoint.y) || (m_currPos.y < endPoint.y && m_currPos.y + 1 > endPoint.y))) {
				m_hasReachedPoint = true;
				running = false;
			}
		}
	}
	bool HasReachPoint() const { return m_hasReachedPoint; }
	Vec2 ReachedPoint() const { return m_currPos; }
};

