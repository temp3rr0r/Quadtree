#include <cstdlib>
#include <cstdio>
#include <vector>

#include "Octree.h"
#include "Stopwatch.h"
#include "Vec2.h"
#include "Quadtree.h"
#include <iostream>
#include "Particle.h"
#include "treeParticle.h"
#include "QuadParticleTree.h"

using namespace brandonpelfrey;

// Used for testing
std::vector<Vec3> points;
std::vector<Vec2> points_2d;
std::vector<Particle> points_particle;

Octree *octree;
OctreePoint *octreePoints;
Vec3 qmin, qmax;

Quadtree *quadtree;
QuadtreePoint *quad_tree_points;
Vec2 qmin_2d, qmax_2d;

QuadParticleTree *quad_particle_tree;
TreeParticle *quad_tree_particles;
Particle qmin_particle(-0.05, -0.05, 0.0f, 0.0f, 0.0f, 0.0f, 0.0);
Particle qmax_particle(0.05f, 0.05, 0.0f, 0.0f, 0.0f, 0.0f, 0.0);

float rand11() // Random number between [-1,1]
{
	return -1.f + (2.f*rand()) * (1.f / RAND_MAX);
}

Vec3 randVec3() // Random vector with components in the range [-1,1]
{
	return Vec3(rand11(), rand11(), rand11());
}

Vec2 randVec2() {
	return Vec2(rand11(), rand11());
}

// Determine if 'point' is within the bounding box [bmin, bmax]
bool naivePointInBox(const Vec3& point, const Vec3& bmin, const Vec3& bmax) {
	return
		point.x >= bmin.x &&
		point.y >= bmin.y &&
		point.z >= bmin.z &&
		point.x <= bmax.x &&
		point.y <= bmax.y &&
		point.z <= bmax.z;
}

bool naivePointInBox_quad(const Vec2& point, const Vec2& bmin, const Vec2& bmax) {
	return
		point.x >= bmin.x &&
		point.y >= bmin.y &&
		point.x <= bmax.x &&
		point.y <= bmax.y;
}

bool naivePointInBox_quad_particle(const Particle& point, const Particle& bmin, const Particle& bmax) {
	return
		point.x_ >= bmin.x_ &&
		point.y_ >= bmin.y_ &&
		point.x_ <= bmax.x_ &&
		point.y_ <= bmax.y_;
}

void init_quad_tree() {
	quadtree = new Quadtree(Vec2(0, 0), Vec2(1, 1));

	//const int n_quad_points = 1 * 1000 * 1000;
	const size_t n_quad_points = 1 * 1000 * 100;
	for (size_t i = 0; i < n_quad_points; ++i) {
		points_2d.push_back(randVec2());
	}
	std::cout << "Created " << points_2d.size() << " 2d points" << std::endl;

	quad_tree_points = new QuadtreePoint[n_quad_points];
	for (size_t i = 0; i < n_quad_points; ++i) {
		quad_tree_points[i].setPosition(points_2d[i]);
		quadtree->insert(quad_tree_points + i);
	}
	std::cout << "Inserted points to quadtree" << std::endl;
	qmin_2d = Vec2(-0.05f, -0.05f);
	qmax_2d = Vec2(0.05f, 0.05f);
}

void init_particle_tree() {

	quad_particle_tree = new QuadParticleTree(Particle(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0), Particle(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0));
	
	//const int n_quad_points = 1 * 1000 * 1000;
	const size_t n_particles = 1 * 1000 * 80;

	for (size_t i = 0; i < n_particles; ++i) {
		points_particle.push_back(Particle(rand11(), rand11(), rand11(), rand11(), rand11(), rand11(), rand11()));
	}
	std::cout << "Created " << points_particle.size() << " 2d particles" << std::endl;

	quad_tree_particles = new TreeParticle[n_particles];
	for (size_t i = 0; i < n_particles; ++i) {
		quad_tree_particles[i].setPosition(points_particle[i]);
		quad_particle_tree->insert(quad_tree_particles + i);
	}
	std::cout << "Inserted points to quad particle tree" << std::endl;
}

void init() {
	// Create a new Octree centered at the origin
	// with physical dimension 2x2x2
	octree = new Octree(Vec3(0, 0, 0), Vec3(1, 1, 1));

	// Create a bunch of random points
	const int nPoints = 1 * 1000 * 1000;
	for (int i = 0; i<nPoints; ++i) {
		points.push_back(randVec3());
	}
	printf("Created %ld points\n", points.size()); fflush(stdout);

	// Insert the points into the octree
	octreePoints = new OctreePoint[nPoints];
	for (int i = 0; i<nPoints; ++i) {
		octreePoints[i].setPosition(points[i]);
		octree->insert(octreePoints + i);
	}
	printf("Inserted points to octree\n"); fflush(stdout);

	// Create a very small query box. The smaller this box is
	// the less work the octree will need to do. This may seem
	// like it is exagerating the benefits, but often, we only
	// need to know very nearby objects.
	qmin = Vec3(-.05f, -.05f, -.05f);
	qmax = Vec3(.05f, .05f, .05f);

	// Remember: In the case where the query is relatively close
	// to the size of the whole octree space, the octree will
	// actually be a good bit slower than brute forcing every point!
}

// Query using brute-force
void testNaive() {
	double start = stopwatch();

	std::vector<int> results;
	for (int i = 0; i < points.size(); ++i) {
		if (naivePointInBox(points[i], qmin, qmax)) {
			results.push_back(i);
		}
	}

	double T = stopwatch() - start;
	printf("testNaive found %ld points in %.5f sec.\n", results.size(), T);
}

// Brute force on 2D vector points
void testNaive_quad() {
	double start = stopwatch();

	std::vector<size_t> results_2d;
	for (size_t i = 0; i < points_2d.size(); ++i) {
		if (naivePointInBox_quad(points_2d[i], qmin_2d, qmax_2d)) {
			results_2d.push_back(i);
		}
	}

	double T = stopwatch() - start;
	std::cout << "testNaive 2D found " << results_2d.size() << " points in " << T << " sec." << std::endl;
}

void testNaive_quad_particles() {
	double start = stopwatch();

	std::vector<size_t> results_particles;
	for (size_t i = 0; i < points_particle.size(); ++i) {
		if (naivePointInBox_quad_particle(points_particle[i], qmin_particle, qmax_particle)) {
			results_particles.push_back(i);
		}
	}

	double T = stopwatch() - start;
	std::cout << "testNaive_quad_particles 2D found " << results_particles.size() << " particles in " << T << " sec." << std::endl;
}

// Query using Octree
void testOctree() {
	double start = stopwatch();

	std::vector<OctreePoint*> results;
	octree->getPointsInsideBox(qmin, qmax, results);

	double T = stopwatch() - start;
	printf("testOctree found %ld points in %.5f sec.\n", results.size(), T);
}

// Query using Quadtree
void test_quad_tree() {
	double start = stopwatch();

	std::vector<QuadtreePoint*> results_quadtree;
	quadtree->getPointsInsideBox(qmin_2d, qmax_2d, results_quadtree);

	double T = stopwatch() - start;
	std::cout << "test_quad_tree 2D found " << results_quadtree.size() << " points in " << T << " sec." << std::endl;
}

void test_quad_particles_tree() {
	double start = stopwatch();

	std::vector<TreeParticle*> results_quad_particle_tree;
	quad_particle_tree->getPointsInsideBox(qmin_particle, qmax_particle, results_quad_particle_tree);

	double T = stopwatch() - start;
	std::cout << "results_quad_particle_tree 2D found " << results_quad_particle_tree.size() << " particles in " << T << " sec." << std::endl;
}


int main(int argc, char **argv) {
	init();
	init_quad_tree();
	init_particle_tree();

	testNaive();
	testOctree();

	testNaive_quad();
	test_quad_tree();

	testNaive_quad_particles();
	test_quad_particles_tree();

	system("pause");
}
