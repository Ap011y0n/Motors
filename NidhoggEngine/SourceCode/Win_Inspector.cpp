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
			GameObjectInspector(App->scene_intro->selectedObj);

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
			static bool active = false;
			ImGui::Checkbox("Active", &active); ImGui::NextColumn(); ImGui::Text("x"); ImGui::NextColumn(); ImGui::Text("y"); ImGui::NextColumn(); ImGui::Text("z"); ImGui::NextColumn();
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