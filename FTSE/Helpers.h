#pragma once

#include <string>
#include <sstream>

class Helpers
{
public:
	static std::string WcharToUTF8(wchar_t* str);
	static std::wstring UTF8ToWchar(std::string const& str);
	static wchar_t* UTF8ToWcharFOTHeap(std::string const& str,int start_ref);

};
struct Vector3;
inline Vector3 operator+(Vector3 const& l, Vector3 const& r);
inline Vector3 operator-(Vector3 const& l, Vector3 const& r);
inline float operator*(Vector3 const& l, Vector3 const& r);
struct Vector3
{
	float v[3];
	Vector3() : v{ 0.0f,.0f,.0f } {}
	Vector3(float a, float b, float c) : v{ a,b,c } {}
	Vector3(float* s) : v{ s[0], s[1], s[2] } {}
	float distance() const {
		return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	}
	static float angle(Vector3 const& l, Vector3 const& r)
	{
		return acos(fabs(l*r) / l.distance() / r.distance())*180.0f/3.1415926535f;
	}
	std::string tostring() {
		std::stringstream ss;
		ss << "(" << v[0] << "," << v[1] << "," << v[2] << ")";
		return ss.str();
	}
};
inline Vector3 operator-(Vector3 const& l, Vector3 const& r)
{
	return Vector3(l.v[0] - r.v[0], l.v[1] - r.v[1], l.v[2] - r.v[2]);
}
inline Vector3 operator+(Vector3 const& l, Vector3 const& r)
{
	return Vector3(l.v[0] + r.v[0], l.v[1] + r.v[1], l.v[2] + r.v[2]);
}

// dot product
inline float operator*(Vector3 const& l, Vector3 const& r)
{
	return l.v[0] * r.v[0] + l.v[1] * r.v[1] + l.v[2] * r.v[2];
}



