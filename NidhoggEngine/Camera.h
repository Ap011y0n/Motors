#ifndef _CAMERA_
#define _CAMERA_

#include "SourceCode/Globals.h"
#include "SourceCode/GameObject.h"

class Camera: public Component
{
public:

	Camera(GameObject* GOlink, int id);
	~Camera();

	void Update();
	static ComponentType GetType() { return ComponentType::CAMERA; }

	void Draw();

	bool active = true;
	math::Frustum frustum;
	std::string name;

};

#endif // !_CAMERA_
