
#pragma once
#include "glmath.h"
#include "Color.h"

#include <vector>

using namespace std;

typedef float GLfloat;
typedef unsigned short GLushort;


enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder,
	Primitive_Pyramid
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	bool axis,wire;
	mat4x4 transform;

protected:
	PrimitiveTypes type;
};

// ============================================
class Cube : public Primitive
{
public :
	Cube();
	Cube(float sizeX = 1.f, float sizeY = 1.f, float sizeZ = 1.f);
	void InnerRender() const;
public:
	vec3 size;
	uint my_indices = 0;
	uint my_vertex = 0;
	int num_vertices;
	int num_indices;
	float vert[24];
	uint index[36];

};

// ============================================
class Pyramid : public Primitive
{
public:
	Pyramid();
	Pyramid(float sizeX = 1.f, float sizeY = 1.f, float sizeZ = 1.f);
	void InnerRender() const;
public:
	vec3 size;
	uint my_indices = 0;
	uint my_vertex = 0;
	int num_vertices;
	int num_indices;
	float vert[12];
	uint index[36];

};

// ============================================
class PrimSphere : public Primitive
{
public:
	PrimSphere();
	PrimSphere(float radius, unsigned int rings, unsigned int sectors);
	void InnerRender() const;
protected:
	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<GLushort> indices;
public:
	uint my_indices = 0;
	uint my_vertex = 0;
	float radius;
};

// ============================================
class Cylinder : public Primitive
{
public:
	Cylinder();
	Cylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class Line : public Primitive
{
public:
	Line();
	Line(float x, float y, float z);
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class PrimPlane : public Primitive
{
public:
	PrimPlane();
	PrimPlane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	vec3 normal;
	float constant;
};

