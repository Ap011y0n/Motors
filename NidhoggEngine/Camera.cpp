#include "Camera.h"

#include "SourceCode/GameObject.h"
#include "SourceCode/imgui.h"
#include "SourceCode/glmath.h"


Camera::Camera(GameObject* GOlink, int id) :Component(/*GOlink, id*/)
{
	char tmp[1024];
	sprintf(tmp, "Camera##%i", id);
	name = tmp;
	//type = CAMERA;
	//frustum.SetWorldMatrix()
}

Camera::~Camera()
{
}

void Camera::Update()
{
	/*if (IsEnabled())
	{*/
		Draw();
	//}
}

/*void Camera::EditorContent()
{

}*/

void Camera::Draw()
{

}




















