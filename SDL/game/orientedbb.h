#ifndef ORIENTEDBB_H
#define ORIENTEDBB_H
#undef min
#undef max

#include "vector2f.h"

struct Manifold {
	Vector2f normal;
	float depth;
	
	Manifold() { }
	Manifold(Vector2f _normal, float _depth)
		: normal(_normal), depth(_depth) { }
};

struct Projection {
	float min;
	float max;
	
	Projection(float _min, float _max)
		: min(_min), max(_max) { }
	
	inline bool overlaps(const Projection &other) const;
	inline float getOverlap(const Projection &other) const;
};

class Edge {
	public:
		Edge() { }
		Edge(const Vector2f &_normal, const Vector2f &point1, const Vector2f &point2)
			: vector(point2 - point1), centre((point2 + point1) * 0.5f), normal(_normal) { }
			
		Edge &operator = (const Edge &rhs) {
			vector = rhs.vector;
			centre = rhs.centre;
			normal = rhs.normal;
			return *this;
		}
	
		const Vector2f &getVector() const { return vector; }
		const Vector2f &getCentre() const { return centre; }
		const Vector2f &getNormal() const { return normal; }
		
		void negateNormal() { normal *= -1; }
		
		void normalise() { normal.normalise(); }
		
	private:
		Vector2f vector;
		Vector2f centre;
		Vector2f normal;
};

class OrientedBB {
	public:
		OrientedBB(float xc, float yc, float hw, float hh)
			: centre(xc, yc), halfwidth(hw, hh), rotation(0) { }
			
		OrientedBB(float xc, float yc, float hw, float hh, float _rotation)
			: centre(xc, yc), halfwidth(hw, hh), rotation(_rotation) { }
	
		OrientedBB(const Vector2f &_centre, const Vector2f &_halfwidth)
			: centre(_centre), halfwidth(_halfwidth), rotation(0) { }
			
		OrientedBB(const Vector2f &_centre, const Vector2f &_halfwidth, float _rotation)
			: centre(_centre), halfwidth(_halfwidth), rotation(_rotation) { }
			
		const Vector2f &getCentre() const { return centre; }
		const Vector2f &getHalfWidth() const { return halfwidth; }
		float getRotation() const { return rotation; }
		
		inline bool intersects(const OrientedBB &other) const;
		bool intersects(const OrientedBB &other, Manifold &manifold) const;
		
	private:
		Vector2f centre;
		Vector2f halfwidth;
		float rotation;
		
		void getEdges(Edge array[]) const;
		inline void getIntersectionAxes(const OrientedBB &other, Edge array[]) const;
		Vector2f &rotateIdentity(Vector2f &vec) const { vec -= centre; vec.rotate(rotation); vec += centre; return vec; }
		Projection project(const Vector2f &axis) const;
};

#endif
