#include "Application.h"
#include "Win_Inspector.h"
#include "imgui.h"
#include "GameObject.h" 

Win_Inspector::Win_Inspector(Application* app, bool start_enabled)
{
}

Win_Inspector::~Win_Inspector()
{

}

void Win_Inspector::InspectorWin()
{
	if (App->UI->Inspector_open == true)
	{
		ImGui::Begin("Inspector", &App->UI->Inspector_open);
		if (App->scene_intro->selectedObj != nullptr)
		{
			GameObjectInspector(App->scene_intro->selectedObj);
			AddComponent(App->scene_intro->selectedObj);

		}

		ImGui::End();
	}
}


void Win_Inspector::GameObjectInspector(GameObject* obj)
{
	ComponentTransform* transform = nullptr;
	ComponentMaterial* material = nullptr;
	ComponentMesh* mesh = nullptr;
	ComponentCamera* camera = nullptr;
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	for (int i = 0; i < obj->Components.size(); i++)
	{
		if (obj->Components[i]->type == ComponentType::TRANSFORM)
		{
			transform = (ComponentTransform*)obj->Components[i];
		}
		if (obj->Components[i]->type == ComponentType::MESH)
		{
			mesh = (ComponentMesh*)obj->Components[i];
		}
		if (obj->Components[i]->type == ComponentType::MATERIAL)
		{
			material = (ComponentMaterial*)obj->Components[i];
		}
		if (obj->Components[i]->type == ComponentType::CAMERA)
		{
			camera = (ComponentCamera*)obj->Components[i];
		}
	}
	if (transform != nullptr)
	{
		static bool hastodelete = false;
		ImGui::Checkbox("Delete", &hastodelete);
		if (hastodelete)
		{
			hastodelete = false;
			obj->to_delete = true;
		}
		ImGui::SameLine();

		static bool hastodeactivate = obj->active;
		ImGui::Checkbox("Active", &hastodeactivate);
		obj->active = hastodeactivate;

		ImGui::SameLine();
		const char* items[] = { "Untagged" };
		static int item_current_idx = 0;
		const char* combo_label = items[item_current_idx];
		if (ImGui::BeginCombo("Tag", combo_label))
		{
			ImGui::EndCombo();
		}
		static char str0[128] = "GameObject";
		if (ImGui::InputText("Name", str0, IM_ARRAYSIZE(str0)))
			obj->Name = str0;

		ImGui::Separator();
		if (ImGui::TreeNodeEx("Transform", node_flags))
		{
			//
			float3 rot = transform->rot.ToEulerXYZ();
			rot *= 180 / pi;
			//ImGui::Columns(1);
			ImGui::Columns(4, "mycolumns");
			ImGui::Separator();
			
			ImGui::Separator();
			ImGui::Text("Position"); ImGui::Spacing(); ImGui::Spacing();// ImGui::NextColumn();
			ImGui::Text("Rotation"); ImGui::Spacing(); ImGui::Spacing(); //ImGui::NextColumn();
			ImGui::Text("Scale"); ImGui::NextColumn();

			// Position
			float t = transform->pos.x;
			ImGui::SetNextItemWidth(50);
			ImGui::DragFloat(" ", &t);
			if (ImGui::IsItemActive())
			{
				transform->SetPos(t - transform->pos.x, 0, 0);
			}
			//Rotation
			float r1 = rot.x;
			ImGui::SetNextItemWidth(50);
			ImGui::DragFloat("  ", &r1);
			if (ImGui::IsItemActive())
			{
				float3 axis(1, 0, 0);
				float newrot = r1 - rot.x;
				transform->SetRotation(Quat::RotateAxisAngle(axis, newrot * pi / 180));

				//transform->rot.x = r1;
			//	transform->UpdateRotation(r1, axis);
			}
			//Scale
			ImGui::SetNextItemWidth(50);
			float s1 = transform->scale.x;

			ImGui::DragFloat("   ", &s1,0.1f);
			if (ImGui::IsItemActive())
			{
				transform->Scale(s1 - transform->scale.x, 0, 0);
			}
			ImGui::NextColumn();


			// Position
			ImGui::SetNextItemWidth(50);
			float t1 = transform->pos.y;
			ImGui::DragFloat("    ", &t1);
			if (ImGui::IsItemActive())
			{
				transform->SetPos(0, t1 - transform->pos.y, 0);
			}
			// Rotation
			float r2 = rot.y;
			ImGui::SetNextItemWidth(50);
			ImGui::DragFloat("     ", &r2);
			if (ImGui::IsItemActive())
			{
				float3 axis(0, 1, 0);
				float newrot = r2 - rot.y;
				transform->SetRotation(Quat::RotateAxisAngle(axis, newrot * pi / 180));
				//	transform->rot.y = r2;

			}
			//Scale
			float s2 = transform->scale.y;
			ImGui::SetNextItemWidth(50);
			ImGui::DragFloat("      .", &s2, 0.1f);
			if (ImGui::IsItemActive())
			{
				transform->Scale(0, s2 - transform->scale.y, 0);
			}
			ImGui::NextColumn();


			// Position
			ImGui::SetNextItemWidth(50);
			float t2 = transform->pos.z;
			ImGui::DragFloat("       ", &t2);
			if (ImGui::IsItemActive())
			{
				transform->SetPos(0, 0, t2 - transform->pos.z);

			}
			// Rotation
			float r3 = rot.z;
			ImGui::SetNextItemWidth(50);
			ImGui::DragFloat("        ", &r3);
			if (ImGui::IsItemActive())
			{
				float3 axis(0, 0, 1);
				float newrot = r3 - rot.z;
				transform->SetRotation(Quat::RotateAxisAngle(axis, newrot * pi / 180));
				//		transform->UpdateRotation(r3, axis);
					//	transform->rot.z = r3;

			}
			//Scale
			float s3 = transform->scale.z;
			ImGui::SetNextItemWidth(50);
			ImGui::DragFloat("         ", &s3, 0.1f);
			if (ImGui::IsItemActive())
			{
				transform->Scale(0, 0, s3 - transform->scale.z);

			}

			//transform->rot = Quat::FromEulerXYZ(r1 * pi / 180, r2 * pi / 180, r3 * pi / 180);
			ImGui::NextColumn();

			ImGui::Columns(1);
			ImGui::TreePop();

			App->UI->direction_camera = { transform->pos.x,transform->pos.y,transform->pos.z };
		}
		ImGui::Separator();
	}

	if (mesh != nullptr)
	{
		if (ImGui::TreeNodeEx("Mesh", node_flags))
		{
			static bool cheked = false;
			static bool active = true;
			ImGui::Checkbox("Active", &active);
			mesh->active = active;
			ImGui::Checkbox("Display normals", &cheked);
			if (cheked)
			{
				if (mesh->triggerNormals)
				{
					mesh->DisplayNormals();
					mesh->triggerNormals = false;
				}
			}
			else
			{
				if (!mesh->triggerNormals)
					mesh->HideNormals();

				mesh->triggerNormals = true;
			}
			ImGui::Separator();
			ImGui::Text("Indexes : %d", mesh->num_index / 3);
			ImGui::Text("Vertex : %d", mesh->num_vertex);
			ImGui::Text("Normals : %d", mesh->num_normals);
			ImGui::TreePop();
		}
		ImGui::Separator();
	}

	if (material != nullptr)
	{
		if (ImGui::TreeNodeEx("Material", node_flags))
		{
			static bool active = true;
			ImGui::Checkbox("Active", &active);
			material->active = active;

			static bool checkers_tex = false;
			ImGui::Checkbox("Use checkers Texture", &checkers_tex);
			if (checkers_tex) {
				material->checkers = true;
			}
			else {

				material->checkers = false;
			}
			ImGui::Text("File:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1.f), "%s", material->texture_path.c_str());
			ImGui::Text("Texture h: %d", material->texture_h); ImGui::SameLine(); ImGui::Text(" w:%d", material->texture_w);
			ImGui::Image((ImTextureID)material->texbuffer, ImVec2(256, 256));

			ImGui::TreePop();
		}
	}

	if (camera != nullptr)
	{
		if (ImGui::TreeNodeEx("Camera", node_flags))
		{
			ImGui::Separator();
			ImGui::Text("Camera configuration:");

			static bool Culling = false;
			ImGui::Checkbox("Culling", &Culling);
			if (Culling) {
				camera->cullingActive = true;
				App->scene_intro->culling = camera;
			}
			else {
				camera->cullingActive = false;
				App->scene_intro->culling = nullptr;

			}

			float p = camera->frustrum.farPlaneDistance;
			ImGui::SetNextItemWidth(200);
			ImGui::DragFloat("Far Plane", &p);
			if (ImGui::IsItemActive())
			{
				camera->frustrum.farPlaneDistance = p;
			}

			float p1 = camera->frustrum.nearPlaneDistance;
			ImGui::SetNextItemWidth(200);
			ImGui::DragFloat("Near Plane", &p1);
			if (ImGui::IsItemActive())
			{
				camera->frustrum.nearPlaneDistance = p1;
			}

			float p2 = camera->GetFOV();
			ImGui::SetNextItemWidth(200);
			ImGui::DragFloat("Field of view", &p2);
			if (ImGui::IsItemActive())
			{
				camera->SetFOV(p2);
				//camera->frustrum.verticalFov = p2;
			}
			ImGui::Text("Horizontal FOV: %f", camera->GetHorizontalFov());
			ImGui::Text("Vertical FOV: %f", camera->GetFOV());
			ImGui::Text("Aspect ratio: %f", camera->aspectRatio);
			ImGui::TreePop();
		}

	}

}

void Win_Inspector::AddComponent(GameObject* obj)
{
	ImGui::Separator();
	ImGui::SetNextItemWidth(130);
	static int selectedMode = 0;
	static const char* Mode[]{ "Box","Sphere", "Capsule" };
	ImGui::Combo(" ", &selectedMode, Mode, IM_ARRAYSIZE(Mode)); ImGui::SameLine();

	if (selectedMode == 0)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
		if (ImGui::Button("AddComponent Box"))
		{
			ComponentType type = ComponentType::COLLIDER;

			if (obj->GetComponent(type) == nullptr)
			{
				obj->CreateComponent(type);

			    Collider* collider = (Collider*)obj->GetComponent(ComponentType::COLLIDER);
				collider->collidertype = ColliderType::BOX;
				collider->body.SetBody(obj, 1, collider->collidertype);
			}
		} 
		ImGui::PopStyleColor();
	}
	if (selectedMode == 1)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
		if (ImGui::Button("AddComponent Sphere"))
		{
			ComponentType type = ComponentType::COLLIDER;

			if (obj->GetComponent(type) == nullptr)
			{
				obj->CreateComponent(type);

				Collider* collider = (Collider*)obj->GetComponent(ComponentType::COLLIDER);
				collider->collidertype = ColliderType::SPHERE;
				collider->body.SetBody(obj, 1, collider->collidertype);
			}
			
		}
		ImGui::PopStyleColor();
	}

	

	if (selectedMode == 2)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
		if (ImGui::Button("AddComponent Capsule"))
		{
			ComponentType type = ComponentType::COLLIDER;

			if (obj->GetComponent(type) == nullptr)
			{
				obj->CreateComponent(type);

				Collider* collider = (Collider*)obj->GetComponent(ComponentType::COLLIDER);
				collider->collidertype = ColliderType::CAPSULE;
				collider->body.SetBody(obj, 1, collider->collidertype);
			}
			
		}ImGui::PopStyleColor();
	}

	
	if (ImGui::Button("AddJoint"))
	{
		App->scene_intro->CreatingJoint = true;
	}
	if (App->scene_intro->CreatingJoint)
	{
		App->scene_intro->JointObj1 = App->scene_intro->selectedObj;
		ImGui::Text("This Object: %s", App->scene_intro->JointObj1->Name.c_str());

		if (App->scene_intro->JointObj2 != nullptr)
			ImGui::Text("Object2 %s:", App->scene_intro->JointObj2->Name.c_str());
		else
		{
			ImGui::Text("Select another object to create a joint");
		}
		if (ImGui::Button("Confirm"))
		{
			App->scene_intro->CreatingJoint = false;
			App->Physics->AddConstraintP2P(App->scene_intro->JointObj1, App->scene_intro->JointObj2,
				btVector3{ 4.f,-0.,-0 }, btVector3{ 0, 0,0 });
		}
	}
	Collider* collider = (Collider*)obj->GetComponent(ComponentType::COLLIDER);
	if (collider != nullptr)
	{
		ColliderEditor(collider);
	}
}

void Win_Inspector::ColliderEditor(Collider* collider)
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	ImGui::Separator();
	if (ImGui::TreeNodeEx("Edit Collider", node_flags))
	{
		
		
		ImGui::Columns(4, "Collidercolumns");
		ImGui::Separator();

		ImGui::Text("Position"); ImGui::Spacing(); ImGui::Spacing();// ImGui::NextColumn();
		ImGui::Text("Rotation"); ImGui::Spacing(); ImGui::Spacing(); //ImGui::NextColumn();
		ImGui::Text("Scale"); ImGui::NextColumn();

		// Position
		float3 pos, scale;
		Quat rot;
		collider->body.TransformMatrix.Decompose(pos, rot, scale);
		float t = pos.x;
		ImGui::SetNextItemWidth(50);
		ImGui::DragFloat(" ", &t);
		if (ImGui::IsItemActive())
		{
			pos.x = t;
			collider->body.TransformMatrix = float4x4::FromTRS(pos, rot, scale);

			collider->body.SetTransform(collider->body.globalTransform);

			//transform->SetPos(t - transform->pos.x, 0, 0);
		}
		//Rotation
		float r1 = rot.x;
		ImGui::SetNextItemWidth(50);
		ImGui::DragFloat("  ", &r1);
		if (ImGui::IsItemActive())
		{
			float3 axis(1, 0, 0);
			float newrot = r1;
			Quat newquat = Quat::RotateAxisAngle(axis, newrot * pi / 180);
			rot = rot * newquat;
			collider->body.TransformMatrix = float4x4::FromTRS(pos, rot, scale);

			collider->body.SetTransform(collider->body.globalTransform);
			//transform->SetRotation(Quat::RotateAxisAngle(axis, newrot * pi / 180));

			//transform->rot.x = r1;
		//	transform->UpdateRotation(r1, axis);
		}
		//Scale
		ImGui::SetNextItemWidth(50);
		float s1 = scale.x;

		ImGui::DragFloat("   ", &s1, 0.1f);
		if (ImGui::IsItemActive())
		{
			scale.x = s1;
			btVector3 size;
			size.setValue(scale.x, scale.y, scale.z);

			collider->body.GetBody()->getCollisionShape()->setLocalScaling(size);
			collider->body.TransformMatrix = float4x4::FromTRS(pos, rot, scale);
			collider->body.SetTransform(collider->body.globalTransform);
		}
		ImGui::NextColumn();


		// Position
		ImGui::SetNextItemWidth(50);
		float t1 = pos.y;
		ImGui::DragFloat("    ", &t1);
		if (ImGui::IsItemActive())
		{
			pos.y = t1;
			collider->body.TransformMatrix = float4x4::FromTRS(pos, rot, scale);

			collider->body.SetTransform(collider->body.globalTransform);
			//transform->SetPos(0, t1 - transform->pos.y, 0);
		}
		// Rotation
		float r2 = rot.y;
		ImGui::SetNextItemWidth(50);
		ImGui::DragFloat("     ", &r2);
		if (ImGui::IsItemActive())
		{
			float3 axis(0, 1, 0);
			float newrot = r2;
			Quat newquat = Quat::RotateAxisAngle(axis, newrot * pi / 180);
			rot = rot * newquat;
			collider->body.TransformMatrix = float4x4::FromTRS(pos, rot, scale);
			//transform->SetRotation(Quat::RotateAxisAngle(axis, newrot * pi / 180));
			//	transform->rot.y = r2;

		}
		//Scale
		float s2 = scale.y;
		ImGui::SetNextItemWidth(50);
		ImGui::DragFloat("      .", &s2, 0.1f);
		if (ImGui::IsItemActive())
		{
			
			scale.y = s2;
			btVector3 size;
			size.setValue(scale.x, scale.y, scale.z);

			collider->body.GetBody()->getCollisionShape()->setLocalScaling(size);
			collider->body.TransformMatrix = float4x4::FromTRS(pos, rot, scale);
			collider->body.SetTransform(collider->body.globalTransform);
		}
		ImGui::NextColumn();


		// Position
		ImGui::SetNextItemWidth(50);
		float t2 = pos.z;
		ImGui::DragFloat("       ", &t2);
		if (ImGui::IsItemActive())
		{

			pos.z = t2;
			collider->body.TransformMatrix = float4x4::FromTRS(pos, rot, scale);

			collider->body.SetTransform(collider->body.globalTransform);
			//transform->SetPos(0, 0, t2 - transform->pos.z);

		}
		// Rotation
		float r3 = rot.z;
		ImGui::SetNextItemWidth(50);
		ImGui::DragFloat("        ", &r3);
		if (ImGui::IsItemActive())
		{
			float3 axis(0, 0, 1);
			float newrot = r3;
			Quat newquat = Quat::RotateAxisAngle(axis, newrot * pi / 180);
			rot = rot * newquat;
			collider->body.TransformMatrix = float4x4::FromTRS(pos, rot, scale);
			collider->body.SetTransform(collider->body.globalTransform);

			//transform->SetRotation(Quat::RotateAxisAngle(axis, newrot * pi / 180));
			//		transform->UpdateRotation(r3, axis);
				//	transform->rot.z = r3;

		}
		//Scale
		float s3 = scale.z;
		ImGui::SetNextItemWidth(50);
		ImGui::DragFloat("         ", &s3, 0.1f);
		if (ImGui::IsItemActive())
		{
			scale.z = s3;
			btVector3 size;
			size.setValue(scale.x, scale.y, scale.z);

			collider->body.GetBody()->getCollisionShape()->setLocalScaling(size);
			collider->body.TransformMatrix = float4x4::FromTRS(pos, rot, scale);
			collider->body.SetTransform(collider->body.globalTransform);
		}

		//transform->rot = Quat::FromEulerXYZ(r1 * pi / 180, r2 * pi / 180, r3 * pi / 180);
		ImGui::NextColumn();

		ImGui::Columns(1);
		ImGui::TreePop();


	}
	ImGui::Separator();
}