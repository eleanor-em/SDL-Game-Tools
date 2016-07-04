#include "orientedbb.h"
#include <limits>

#define MIN(a,b) ((a > b) ? (b) : (a))
#define MAX(a,b) ((a > b) ? (a) : (b))

inline bool Projection::overlaps(const Projection &other) const {
	return !(min > other.max || other.min > max);
}

inline float Projection::getOverlap(const Projection &other) const {
	return MIN(max, other.max) - MAX(min, other.min);
}

void OrientedBB::getEdges(Edge array[]) const {
	Vector2f vectors[2];
	
	Vector2f topLeft = centre - halfwidth;
	Vector2f topRight(centre.x + halfwidth.x, centre.y - halfwidth.y);
	Vector2f bottomLeft(centre.x - halfwidth.x, centre.y + halfwidth.y);
	Vector2f bottomRight = centre + halfwidth;
	
	// x axis
	vectors[0] = (topLeft - topRight).rotate(rotation);
	// y axis
	vectors[1] = (bottomRight - topRight).rotate(rotation);
	
	array[0] = Edge(vectors[0].setNormal(), topLeft.rotate(rotation), topRight.rotate(rotation));
	array[1] = Edge(vectors[1].setNormal(), bottomLeft.rotate(rotation), bottomRight.rotate(rotation));
	
	if (array[0].getNormal() * vectors[1] > 0) {
		array[0].negateNormal();
	}
	array[0].normalise();
	
	if (array[1].getNormal() * vectors[0] > 0) {
		array[1].negateNormal();
	}
	array[1].normalise();
}

inline void OrientedBB::getIntersectionAxes(const OrientedBB &other, Edge array[]) const {
	Edge axes1[2], axes2[2];
	getEdges(axes1);
	other.getEdges(axes2);
	
	array[0] = axes1[0];
	array[1] = axes1[1];
	array[2] = axes2[0];
	array[3] = axes2[1];
}

Projection OrientedBB::project(const Vector2f &axis) const {
	Vector2f topLeft(centre - halfwidth);
	Vector2f topRight(centre.x + halfwidth.x, centre.y - halfwidth.y);
	Vector2f bottomLeft(centre.x - halfwidth.x, centre.y + halfwidth.y);
	Vector2f bottomRight(centre + halfwidth);
	
	rotateIdentity(topLeft);
	rotateIdentity(topRight);
	rotateIdentity(bottomLeft);
	rotateIdentity(bottomRight);
	
	Vector2f norm(axis);
	norm.normalise();
	float min = topLeft * norm;
	float max = min;
	
	float p = topRight * norm;	
	if (p < min) {
		min = p;
	} else if (p > max) {
		max = p;
	}
	
	p = bottomLeft * norm;
	if (p < min) {
		min = p;
	} else if (p > max) {
		max = p;
	}
	
	p = bottomRight * norm;
	if (p < min) {
		min = p;
	} else if (p > max) {
		max = p;
	}
	
	return Projection(min, max);
}

inline bool OrientedBB::intersects(const OrientedBB &other) const {
	Manifold m;
	return intersects(other, m);
}

bool OrientedBB::intersects(const OrientedBB &other, Manifold &manifold) const {
	float overlap = std::numeric_limits<float>::max(); // float max
	Edge *smallest = nullptr;
	
	Edge axes[4];
	getIntersectionAxes(other, axes);
	
	for (int i = 0; i < 4; ++i) {
		if ((centre - axes[i].getCentre()) * (other.centre - axes[i].getCentre()) < 0) {
			axes[i].negateNormal();
		}
		Projection p1 = project(axes[i].getNormal()),
				   p2 = other.project(axes[i].getNormal());
				   
		if (!p1.overlaps(p2)) {
			return false;
		} else {
			float o = p1.getOverlap(p2);
			if (o < overlap) {
				overlap = o;
				smallest = &axes[i];
			}
		}
	}
	manifold.normal = smallest->getNormal();
	manifold.normal.normalise();
	manifold.depth = overlap;
	return true;
}

