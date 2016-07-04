#ifndef VECTOR2F_H
#define VECTOR2F_H

#include <cmath>

class Vector2f {
    public:
        float x;
        float y;
        
        Vector2f()
        	: x(0), y(0) { }
        Vector2f(float theX, float theY)
            : x(theX), y(theY) { }
        Vector2f(const Vector2f &other)
        	: x(other.x), y(other.y) { }
            
        Vector2f& operator =(const Vector2f &rhs) { x = rhs.x; y = rhs.y; return *this; }    
        
        Vector2f& operator +=(const Vector2f& rhs) { x += rhs.x; y += rhs.y; return *this; }
        Vector2f& operator -=(const Vector2f& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
        
        Vector2f operator +(const Vector2f& rhs) const { return Vector2f(*this) += rhs; }
        Vector2f operator -(const Vector2f& rhs) const { return Vector2f(*this) -= rhs; }
        
        bool operator ==(const Vector2f& rhs) const { return x == rhs.x && y == rhs.y; }
        bool operator !=(const Vector2f& rhs) const { return !(*this == rhs); }
        
        // dot product
        float operator *(const Vector2f& rhs) const { return x * rhs.x + y * rhs.y; }
        
        Vector2f& operator *=(float rhs) { x *= rhs; y *= rhs; return *this; }
        Vector2f operator *(float rhs) const { return Vector2f(*this) *= rhs; }
        
        float lengthSquared() const { return x * x + y * y; }
        float length() const { return std::sqrt(lengthSquared()); }
        
        Vector2f& normalise() { float norm = length(); x /= norm; y /= norm; return *this; }
        Vector2f& rotate(float radians) {
        	float c = std::cos(radians);
        	float s = std::sin(radians);
        	
        	float px = x * c - y * s;
        	float py = x * s + y * c;
        	x = px;
        	y = py;
        	
        	return *this;
        }
        
        Vector2f getNormal() const {
		    return Vector2f(-y, x);
        }
        Vector2f &setNormal() {
        	float temp = x;
        	x = -y;
        	y = temp;
        	return *this;
        }
        
        float angleTo(const Vector2f& other) const {
            float res = (*this * other) / (length() * other.length());
            // floating point error
            if (res < -1) res = -1;
            if (res > 1) res = 1;
            return std::acos(res);
        }
};

Vector2f operator *(float lhs, const Vector2f &rhs);

#endif
