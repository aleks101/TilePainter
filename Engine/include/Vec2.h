#pragma once

#include <iostream>

struct Vec2 {
    float x, y;
	Vec2(float _x, float _y) : x(_x), y(_y) { }
	Vec2() { x = 0; y = 0; }
	Vec2(const Vec2& vec2) : x(vec2.x), y(vec2.y) { }
    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }
    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }
    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }
    Vec2 operator/(float scalar) const {
        return Vec2(x / scalar, y / scalar);
    }
    bool operator==(const Vec2& vec2) {
        if (x == vec2.x && y == vec2.y)
            return true;
        return false;
    }
    bool operator!=(const Vec2& other) const {
        return (x != other.x) || (y != other.y);
    }
    float GetMagnitude() const {
        return std::sqrt(x * x + y * y);
    }
    void Normalize() {
        float magnitude = std::sqrt(x * x + y * y);
        if (magnitude > 0.0f) {
            x /= magnitude;
            y /= magnitude;
        }
    }
};