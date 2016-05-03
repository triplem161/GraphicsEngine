#include "Vector3.hpp"
#include "Matrix.hpp"
#include <cmath>

Vector3::Vector3(void) :
	Vector3(0.f, 0.f, 0.f)
{}

Vector3::Vector3(float x, float y, float z) :
	x(x),
	y(y),
	z(z)
{}

Vector3::Vector3(Vector3 const & vector)
{
	*this = vector;
}

Vector3::Vector3(Vector3 && vector)
{
	*this = std::move(vector);
}

float Vector3::length(void)
{
	return (std::sqrt(x * x + y * y + z * z));
}

Vector3 & Vector3::normalize(void)
{
	float len = length();
	x /= len;
	y /= len;
	z /= len;
	return (*this);
}

	Vector3 inverse(void);

Vector3 & Vector3::operator=(Vector3 const & vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	return (*this);
}

Vector3 & Vector3::operator=(Vector3 && vector)
{
	x = std::move(vector.x);
	y = std::move(vector.y);
	z = std::move(vector.z);
	return (*this);
}

Vector3 & Vector3::operator+=(Vector3 const & vector3)
{
	x += vector3.x;
	y += vector3.y;
	z += vector3.z;
	return (*this);
}

Vector3 & Vector3::operator*=(Vector3 const & vector3)
{
	x *= vector3.x;
	y *= vector3.y;
	z *= vector3.z;
	return (*this);
}

Vector3 & Vector3::operator-=(Vector3 const & vector3)
{
	x -= vector3.x;
	y -= vector3.y;
	z -= vector3.z;
	return (*this);
}

Vector3 Vector3::operator+(Vector3 const & vector3) const
{
	return (Vector3(x + vector3.x, y + vector3.y, z + vector3.z));
}

Vector3 Vector3::operator*(float factor) const
{
	return (Vector3(x * factor, y * factor, z * factor));
}

Vector3 Vector3::operator*(Vector3 const & vector3) const
{
	return (Vector3(x * vector3.x, y * vector3.y, z * vector3.z));
}

Vector3 Vector3::operator*(Matrix const & matrix) const
{
	Vector3 res;

	res.x += matrix.getMatrix()[0 * 4 + 0] * x;
	res.x += matrix.getMatrix()[0 * 4 + 1] * y;
	res.x += matrix.getMatrix()[0 * 4 + 2] * z;

	res.y += matrix.getMatrix()[1 * 4 + 0] * x;
	res.y += matrix.getMatrix()[1 * 4 + 1] * y;
	res.y += matrix.getMatrix()[1 * 4 + 2] * z;

	res.z += matrix.getMatrix()[2 * 4 + 0] * x;
	res.z += matrix.getMatrix()[2 * 4 + 1] * y;
	res.z += matrix.getMatrix()[2 * 4 + 2] * z;

	return (res);
}

Vector3 Vector3::operator-(Vector3 const & vector3) const
{
	return (Vector3(x - vector3.x, y - vector3.y, z - vector3.z));
}

Vector3 Vector3::operator-(void) const
{
	return (Vector3(-x, -y, -z));
}

std::ostream & operator<<(std::ostream & out, Vector3 const & vector)
{
	out << "Vector3(" << vector.x  << ", " << vector.y << ", " << vector.z << ")";
	return (out);
}

Vector3 operator/(float value, Vector3 const & vector3)
{
	return Vector3(value / vector3.x, value / vector3.y, value / vector3.z);
}
