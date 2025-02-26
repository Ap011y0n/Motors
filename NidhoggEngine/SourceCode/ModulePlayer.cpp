#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(3, 1.5, 6);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mudguard_size.Set(0.7f, 0.30f, 1.8f);
	car.mudguard_offset.Set(1.4, 1.6, -2);
	car.mudguard_size2.Set(0.7f, 0.90f, 0.30f);
	car.mudguard_offset2.Set(1.4, 1.3, -3);
	car.mudguard_size3.Set(0.7f, 0.90f, 0.30f);
	car.mudguard_offset3.Set(1.4, 1.3, -1.2);
	car.mudguard_R_size.Set(0.7f, 0.30f, 1.8f);
	car.mudguard_R_offset.Set(-1.4, 1.6, -2);
	car.mudguard_R_size2.Set(0.7f, 0.90f, 0.30f);
	car.mudguard_R_offset2.Set(-1.4, 1.3, -3);
	car.mudguard_R_size3.Set(0.7f, 0.90f, 0.30f);
	car.mudguard_R_offset3.Set(-1.4, 1.3, -1.2);

	car.Front_mudguard_size.Set(0.7f, 0.30f, 1.8f);
	car.Front_mudguard_offset.Set(1.4, 1.6, 2);
	car.Front_mudguard_size2.Set(0.7f, 0.90f, 0.30f);
	car.Front_mudguard_offset2.Set(1.4, 1.3, 3);
	car.Front_mudguard_size3.Set(0.7f, 0.90f, 0.30f);
	car.Front_mudguard_offset3.Set(1.4, 1.3, 1.2);
	car.Front_mudguard_R_size.Set(0.7f, 0.30f, 1.8f);
	car.Front_mudguard_R_offset.Set(-1.4, 1.6, 2);
	car.Front_mudguard_R_size2.Set(0.7f, 0.90f, 0.30f);
	car.Front_mudguard_R_offset2.Set(-1.4, 1.3, 3);
	car.Front_mudguard_R_size3.Set(0.7f, 0.90f, 0.30f);
	car.Front_mudguard_R_offset3.Set(-1.4, 1.3, 1.2);

	car.bumper_size.Set(2.8, 1.6, 3);
	car.bumper_offset.Set(0, 3, 0);
	car.antenna_size.Set(0.1, 3, 0.1);
	car.antenna_offset.Set(-1.3, 3, -2.8);
	car.rearview_size.Set(4, 0.4, 0.4);
	car.rearview_offset.Set(0,2.7,1);
	car.antenna_Ball_size.Set(0.4, 0.6, 0.4);
	car.antenna_Ball_offset.Set(-1.3, 4.3, -2.8);

	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.4f;
	float wheel_radius = 0.9f;
	float wheel_width = 0.8f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x * 0.5f;
	float half_length = car.chassis_size.z * 0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->Physics->AddVehicle(car);
	vehicle->SetPos(0, 12, 10);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && acceleration < 0.2)
	{
		acceleration = -MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && acceleration > 0.2)
	{
		brake = BRAKE_POWER;
	}
	if (vehicle != nullptr)
	{
		vehicle->ApplyEngineForce(acceleration);
		vehicle->Turn(turn);
		vehicle->Brake(brake);

		vehicle->Render();
	}


	/*char title[80];
	sprintf_s(title, "%.1f Km/h", vehicle->GetKmh());
	App->window->SetTitle(title);*/

	return UPDATE_CONTINUE;
}