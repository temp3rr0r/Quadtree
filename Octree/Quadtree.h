#pragma once

#include <vector>
#include "QuadtreePoint.h"
#include "Vec2.h"

namespace brandonpelfrey {

	class Quadtree {
		static const uint8_t NUM_CHILDREN = 4;
		Vec2 origin;
		Vec2 halfDimension;
		Quadtree *children[NUM_CHILDREN];
		QuadtreePoint *data;

	public:		
		Quadtree(const Vec2& origin, const Vec2& halfDimension)
			: origin(origin), halfDimension(halfDimension), data(nullptr) {
			for (int i = 0; i < NUM_CHILDREN; ++i)
				children[i] = nullptr;
		}

		Quadtree(const Quadtree& copy)
			: origin(copy.origin), halfDimension(copy.halfDimension), data(copy.data) {			
		}
		
		~Quadtree() {
			for (int i = 0; i < NUM_CHILDREN; ++i)
				delete children[i];
		}

		int getQuadrantContainingPoint(const Vec2& point) const {
			int quadent = 0;
			if (point.x >= origin.x)
				quadent |= 2;
			if (point.y >= origin.y)
				quadent |= 1;
			return quadent;
		}

		bool isLeafNode() const {
			return children[0] == nullptr;
		}

		void insert(QuadtreePoint* point) {
			if (isLeafNode()) {
				if (data == nullptr) {
					data = point;
				}
				else {
					QuadtreePoint *oldPoint = data;
					data = nullptr;

					for (uint8_t i = 0; i < NUM_CHILDREN; ++i) {
						Vec2 newOrigin = origin;
						newOrigin.x += halfDimension.x * (i & 2 ? .5f : -.5f);
						newOrigin.y += halfDimension.y * (i & 1 ? .5f : -.5f);
						children[i] = new Quadtree(newOrigin, halfDimension *.5f);
					}

					children[getQuadrantContainingPoint(oldPoint->getPosition())]->insert(oldPoint);
					children[getQuadrantContainingPoint(point->getPosition())]->insert(point);
				}
			}
			else {
				int octant = getQuadrantContainingPoint(point->getPosition());
				children[octant]->insert(point);
			}
		}

		void getPointsInsideBox(const Vec2& bmin, const Vec2& bmax, std::vector<QuadtreePoint*>& results) const {

			if (isLeafNode()) {
				if (data != nullptr) {
					const Vec2& p = data->getPosition();
					if (p.x > bmax.x || p.y > bmax.y) 
						return;
					if (p.x < bmin.x || p.y < bmin.y)
						return;
					results.push_back(data);
				}
			}
			else {
				for (uint8_t i = 0; i < NUM_CHILDREN; ++i) {
					Vec2 cmax = children[i]->origin + children[i]->halfDimension;
					Vec2 cmin = children[i]->origin - children[i]->halfDimension;

					if (cmax.x < bmin.x || cmax.y  <bmin.y)
						continue;
					if (cmin.x > bmax.x || cmin.y > bmax.y)
						continue;

					children[i]->getPointsInsideBox(bmin, bmax, results);
				}
			}
		}
	};
}
