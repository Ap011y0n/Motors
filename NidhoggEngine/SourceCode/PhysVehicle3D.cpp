#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* rigidbody, btRaycastVehicle* vehicle, const VehicleInfo& info) : vehicle(vehicle), info(info)
{
	body = rigidbody;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	PrimCylinder wheel;

	wheel.color = Blue;

	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	Cube wing(info.mudguard_size.x, info.mudguard_size.y, info.mudguard_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing.transform);
	btVector3 fw_offset(info.mudguard_offset.x, info.mudguard_offset.y, info.mudguard_offset.z);
	fw_offset = fw_offset.rotate(q.getAxis(), q.getAngle());
	wing.transform.M[12] += fw_offset.getX();
	wing.transform.M[13] += fw_offset.getY();
	wing.transform.M[14] += fw_offset.getZ();

	Cube wing2(info.mudguard_size2.x, info.mudguard_size2.y, info.mudguard_size2.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing2.transform);
	btVector3 fw_offset2(info.mudguard_offset2.x, info.mudguard_offset2.y, info.mudguard_offset2.z);
	fw_offset2 = fw_offset2.rotate(q.getAxis(), q.getAngle());
	wing2.transform.M[12] += fw_offset2.getX();
	wing2.transform.M[13] += fw_offset2.getY();
	wing2.transform.M[14] += fw_offset2.getZ();

	Cube wing3(info.mudguard_size3.x, info.mudguard_size3.y, info.mudguard_size3.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing3.transform);
	btVector3 fw_offset3(info.mudguard_offset3.x, info.mudguard_offset3.y, info.mudguard_offset3.z);
	fw_offset3 = fw_offset3.rotate(q.getAxis(), q.getAngle());
	wing3.transform.M[12] += fw_offset3.getX();
	wing3.transform.M[13] += fw_offset3.getY();
	wing3.transform.M[14] += fw_offset3.getZ();
	
	Cube wing4(info.mudguard_R_size.x, info.mudguard_R_size.y, info.mudguard_R_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing4.transform);
	btVector3 fw_offset4(info.mudguard_R_offset.x, info.mudguard_R_offset.y, info.mudguard_R_offset.z);
	fw_offset4 = fw_offset4.rotate(q.getAxis(), q.getAngle());
	wing4.transform.M[12] += fw_offset4.getX();
	wing4.transform.M[13] += fw_offset4.getY();
	wing4.transform.M[14] += fw_offset4.getZ();

	Cube wing5(info.mudguard_R_size2.x, info.mudguard_R_size2.y, info.mudguard_R_size2.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing5.transform);
	btVector3 fw_offset5(info.mudguard_R_offset2.x, info.mudguard_R_offset2.y, info.mudguard_R_offset2.z);
	fw_offset5 = fw_offset5.rotate(q.getAxis(), q.getAngle());
	wing5.transform.M[12] += fw_offset5.getX();
	wing5.transform.M[13] += fw_offset5.getY();
	wing5.transform.M[14] += fw_offset5.getZ();

	Cube wing6(info.mudguard_R_size3.x, info.mudguard_R_size3.y, info.mudguard_R_size3.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing6.transform);
	btVector3 fw_offset6(info.mudguard_R_offset3.x, info.mudguard_R_offset3.y, info.mudguard_R_offset3.z);
	fw_offset6 = fw_offset6.rotate(q.getAxis(), q.getAngle());
	wing6.transform.M[12] += fw_offset6.getX();
	wing6.transform.M[13] += fw_offset6.getY();
	wing6.transform.M[14] += fw_offset6.getZ();
	
	chassis.Render();
	wing.Render();
	wing2.Render();
	wing3.Render();
	wing4.Render();
	wing5.Render();
	wing6.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}