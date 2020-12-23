#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "glmath.h"
//#include "Module.h"
#include "Globals.h"
#include <vector>

class btRigidBody;
class btCollisionShape;
struct btDefaultMotionState;
class Module;
class btRaycastVehicle;

class vec3;
class PrimSphere;
class Cube;
class PrimCylinder;
class Plane;
class btQuaternion;

class GameObject;
class Primitive;

class PhysBody3D
{
public:
	PhysBody3D();
	~PhysBody3D();

	void SetBody(PrimSphere* primitive, float mass);
	void SetBody(Cube* primitive, float mass);
	void SetBody(PrimCylinder* primitive, float mass);
	void SetBody(GameObject* primitive, float mass);

	bool HasBody() const;
	btRigidBody* GetBody() const;

	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void SetRotation(btQuaternion quat);

	const vec3 GetPos() const;
	vec3 GetForwardVector() const;
	vec3 GetaAboveVector() const;

	void SetAsSensor(bool is_sensor);
	bool IsSensor() const;

	void SetSpeed(vec3 speed);
	void Push(vec3 force);
	void Stop();

private:
	void SetBody(btCollisionShape* shape, Primitive* parent, float mass);	
	void SetBody(btCollisionShape* shape, GameObject* parent, float mass);

	btRigidBody* body;
	btCollisionShape* colShape;
	btDefaultMotionState* motionState;
public:
	Primitive* parentPrimitive;
	GameObject* parentGameObject;
	std::vector<Module*> collision_listeners;
	btRaycastVehicle* vehicle;
	bool is_sensor = false;
};

#endif // __PhysBody3D_H__