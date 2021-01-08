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
	
	Cube wing7(info.bumper_size.x, info.bumper_size.y, info.bumper_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing7.transform);
	btVector3 fw_offset7(info.bumper_offset.x, info.bumper_offset.y, info.bumper_offset.z);
	fw_offset7 = fw_offset7.rotate(q.getAxis(), q.getAngle());
	wing7.transform.M[12] += fw_offset7.getX();
	wing7.transform.M[13] += fw_offset7.getY();
	wing7.transform.M[14] += fw_offset7.getZ();

	Cube wing8(info.Front_mudguard_size.x, info.Front_mudguard_size.y, info.Front_mudguard_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing8.transform);
	btVector3 fw_offset8(info.Front_mudguard_offset.x, info.Front_mudguard_offset.y, info.Front_mudguard_offset.z);
	fw_offset8 = fw_offset8.rotate(q.getAxis(), q.getAngle());
	wing8.transform.M[12] += fw_offset8.getX();
	wing8.transform.M[13] += fw_offset8.getY();
	wing8.transform.M[14] += fw_offset8.getZ();

	Cube wing9(info.Front_mudguard_size2.x, info.Front_mudguard_size2.y, info.Front_mudguard_size2.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing9.transform);
	btVector3 fw_offset9(info.Front_mudguard_offset2.x, info.Front_mudguard_offset2.y, info.Front_mudguard_offset2.z);
	fw_offset9 = fw_offset9.rotate(q.getAxis(), q.getAngle());
	wing9.transform.M[12] += fw_offset9.getX();
	wing9.transform.M[13] += fw_offset9.getY();
	wing9.transform.M[14] += fw_offset9.getZ();

	Cube wing10(info.Front_mudguard_size3.x, info.Front_mudguard_size3.y, info.Front_mudguard_size3.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing10.transform);
	btVector3 fw_offset10(info.Front_mudguard_offset3.x, info.Front_mudguard_offset3.y, info.Front_mudguard_offset3.z);
	fw_offset10 = fw_offset10.rotate(q.getAxis(), q.getAngle());
	wing10.transform.M[12] += fw_offset10.getX();
	wing10.transform.M[13] += fw_offset10.getY();
	wing10.transform.M[14] += fw_offset10.getZ();

	Cube wing11(info.Front_mudguard_R_size.x, info.Front_mudguard_R_size.y, info.Front_mudguard_R_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing11.transform);
	btVector3 fw_offset11(info.Front_mudguard_R_offset.x, info.Front_mudguard_R_offset.y, info.Front_mudguard_R_offset.z);
	fw_offset11 = fw_offset11.rotate(q.getAxis(), q.getAngle());
	wing11.transform.M[12] += fw_offset11.getX();
	wing11.transform.M[13] += fw_offset11.getY();
	wing11.transform.M[14] += fw_offset11.getZ();

	Cube wing12(info.Front_mudguard_R_size2.x, info.Front_mudguard_R_size2.y, info.Front_mudguard_R_size2.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing12.transform);
	btVector3 fw_offset12(info.Front_mudguard_R_offset2.x, info.Front_mudguard_R_offset2.y, info.Front_mudguard_R_offset2.z);
	fw_offset12 = fw_offset12.rotate(q.getAxis(), q.getAngle());
	wing12.transform.M[12] += fw_offset12.getX();
	wing12.transform.M[13] += fw_offset12.getY();
	wing12.transform.M[14] += fw_offset12.getZ();

	Cube wing13(info.Front_mudguard_R_size3.x, info.Front_mudguard_R_size3.y, info.Front_mudguard_R_size3.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing13.transform);
	btVector3 fw_offset13(info.Front_mudguard_R_offset3.x, info.Front_mudguard_R_offset3.y, info.Front_mudguard_R_offset3.z);
	fw_offset13 = fw_offset13.rotate(q.getAxis(), q.getAngle());
	wing13.transform.M[12] += fw_offset13.getX();
	wing13.transform.M[13] += fw_offset13.getY();
	wing13.transform.M[14] += fw_offset13.getZ();

	Cube wing14(info.antenna_size.x, info.antenna_size.y, info.antenna_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing14.transform);
	btVector3 fw_offset14(info.antenna_offset.x, info.antenna_offset.y, info.antenna_offset.z);
	fw_offset14 = fw_offset14.rotate(q.getAxis(), q.getAngle());
	wing14.transform.M[12] += fw_offset14.getX();
	wing14.transform.M[13] += fw_offset14.getY();
	wing14.transform.M[14] += fw_offset14.getZ();

	Cube wing15(info.rearview_size.x, info.rearview_size.y, info.rearview_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing15.transform);
	btVector3 fw_offset15(info.rearview_offset.x, info.rearview_offset.y, info.rearview_offset.z);
	fw_offset15 = fw_offset15.rotate(q.getAxis(), q.getAngle());
	wing15.transform.M[12] += fw_offset15.getX();
	wing15.transform.M[13] += fw_offset15.getY();
	wing15.transform.M[14] += fw_offset15.getZ();

	Cube wing16(info.antenna_Ball_size.x, info.antenna_Ball_size.y, info.antenna_Ball_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&wing16.transform);
	btVector3 fw_offset16(info.antenna_Ball_offset.x, info.antenna_Ball_offset.y, info.antenna_Ball_offset.z);
	fw_offset16 = fw_offset16.rotate(q.getAxis(), q.getAngle());
	wing16.transform.M[12] += fw_offset16.getX();
	wing16.transform.M[13] += fw_offset16.getY();
	wing16.transform.M[14] += fw_offset16.getZ();

	chassis.Render();
	wing.Render();
	wing2.Render();
	wing3.Render();
	wing4.Render();
	wing5.Render();
	wing6.Render();
	wing7.Render();
	wing8.Render();
	wing9.Render();
	wing10.Render();
	wing11.Render();
	wing12.Render();
	wing13.Render();
	wing14.Render();
	wing15.Render();
	wing16.Render();
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