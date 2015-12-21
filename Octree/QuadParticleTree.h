#pragma once

#include <vector>
#include "TreeParticle.h"
#include "Particle.h"
#include "Settings.h"

namespace brandonpelfrey {

	class QuadParticleTree {
		static const uint8_t NUM_CHILDREN = 4;
		Particle origin;
		Particle halfDimension;
		QuadParticleTree *children[NUM_CHILDREN];
		TreeParticle *data;
		bool hasChildren;

		float center_of_mass_x_;
		float center_of_mass_y_;
		float total_mass_;

		uint8_t depth = 0;

	public:		
		QuadParticleTree(const Particle& origin, const Particle& halfDimension)
			: origin(origin), halfDimension(halfDimension), data(nullptr) {
			for (int i = 0; i < NUM_CHILDREN; ++i)
				children[i] = nullptr;
		}

		QuadParticleTree(const QuadParticleTree& copy)
			: origin(copy.origin), halfDimension(copy.halfDimension), data(copy.data) {
			
			++depth;
		}
		
		~QuadParticleTree() {
			for (int i = 0; i < NUM_CHILDREN; ++i)
				delete children[i];
		}

		int getQuadrantContainingPoint(const Particle& point) const {
			int quadent = 0;
			if (point.x_ >= origin.x_)
				quadent |= 2;
			if (point.y_ >= origin.y_)
				quadent |= 1;
			return quadent;
		}

		bool isLeafNode() const {
			return children[0] == nullptr;
		}

		void insert(TreeParticle* point) {
			if (isLeafNode()) {
				if (data == nullptr) {
					data = point;
				}
				else {

					if (this->depth < MAX_TREE_DEPTH) {
						TreeParticle *oldPoint = data;
						data = nullptr;

						for (uint8_t i = 0; i < NUM_CHILDREN; ++i) {
							Particle newOrigin = origin;
							newOrigin.x_ += halfDimension.x_ * (i & 2 ? .5f : -.5f);
							newOrigin.y_ += halfDimension.y_ * (i & 1 ? .5f : -.5f);
							children[i] = new QuadParticleTree(newOrigin, halfDimension *.5f);
							children[i]->depth = this->depth + 1; // increase the depth on children
						}

						children[getQuadrantContainingPoint(oldPoint->getPosition())]->insert(oldPoint);
						children[getQuadrantContainingPoint(point->getPosition())]->insert(point);
					}
				}
			}
			else {
				int octant = getQuadrantContainingPoint(point->getPosition());

				// Calculate total mass
				if (octant < NUM_CHILDREN) {
					float mass = 0.0f;
					float center_y = 0.0f;
					float center_x = 0.0f;

					uint8_t children_count = 0;
					for (uint8_t i = 0; i < NUM_CHILDREN; ++i) {
						if (children[i]->data != nullptr) {
							++children_count;
							mass += children[i]->data->get_mass();
							center_x += children[i]->data->getPosition().x_;
							center_y += children[i]->data->getPosition().y_;
						}
					}
					if (children_count > 0) {
						this->center_of_mass_x_ = center_x / children_count;
						this->center_of_mass_y_ = center_y / children_count;
						this->total_mass_ += mass;
					}

					
				}

				// Calculate Center Of mass

				children[octant]->insert(point);
			}
		}

		void getPointsInsideBox(const Particle& bmin, const Particle& bmax, std::vector<TreeParticle*>& results) const {

			if (isLeafNode()) {
				if (data != nullptr) {
					const Particle& p = data->getPosition();
					if (p.x_ > bmax.x_ || p.y_ > bmax.y_)
						return;
					if (p.x_ < bmin.x_ || p.y_ < bmin.y_)
						return;
					results.push_back(data);
				}
			}
			else {
				for (uint8_t i = 0; i < NUM_CHILDREN; ++i) {
					Particle cmax = children[i]->origin + children[i]->halfDimension;
					Particle cmin = children[i]->origin - children[i]->halfDimension;

					if (cmax.x_ < bmin.x_ || cmax.y_  <bmin.y_)
						continue;
					if (cmin.x_ > bmax.x_ || cmin.y_ > bmax.y_)
						continue;

					children[i]->getPointsInsideBox(bmin, bmax, results);
				}
			}
		}
	};
}
