#include "Application.h"
#include "Win_TimeManagement.h"
#include "imgui.h"
#include "GameObject.h" 
#include "Time.h"

Win_TimeManagement::Win_TimeManagement(Application * app, bool start_enabled)
{

}

Win_TimeManagement::~Win_TimeManagement()
{

}

void Win_TimeManagement::Update_Ui()
{

	if (App->UI->Inspector_open == true)
	{
		ImGui::Begin("Time", (bool*)false, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::Text("|"); ImGui::SameLine();


		if (Time::Engine_Active == false) {

			if (ImGui::Button("PLAY"))
			{
				Time::Start();
				App->serializer->CreateNewScene();
				App->scene_intro->SaveScene(App->scene_intro->scene);
				App->serializer->SaveScene("PlayScene.json");
			} ImGui::SameLine();
		}
		else {

			if (Time::Game_Paused == false)
			{
				if (ImGui::Button("PAUSE"))
				{
					Time::Pause();

				} ImGui::SameLine();
			}

			if (Time::Game_Paused == true)
			{
				if (ImGui::Button("CONTINUE"))
				{
					Time::Resume();

				}ImGui::SameLine();
			}

			if (ImGui::Button("STOP"))
			{
				App->scene_intro->DeleteSceneObjects(App->scene_intro->scene);
				App->serializer->LoadScene("Assets/PlayScene.json");
				Time::Stop();
			}ImGui::SameLine();

		}
		ImGui::Text("|"); ImGui::SameLine();
		{
			ImGui::SetNextItemWidth(130);
			static int selectedMode = 0;
			static const char* Mode[]{ "WORLD","LOCAL" };
			ImGui::Combo("Mode", &selectedMode, Mode, IM_ARRAYSIZE(Mode)); ImGui::SameLine();
			if (selectedMode == 1)
			{
				App->UI->guizmo_mode = ImGuizmo::MODE::LOCAL;
			}
			if (selectedMode == 0)
			{
				App->UI->guizmo_mode = ImGuizmo::MODE::WORLD;
			}

		}
		ImGui::Text("|"); ImGui::SameLine();
		{
			static bool BoundingBox = false;
			if (ImGui::Checkbox("BoundingBox", &BoundingBox))
			{
				for (int i = 0; i < App->scene_intro->scene->childs.size(); i++)
				{
					App->UI->Change_Visibility_BoundingBoxes(App->scene_intro->scene->childs[i], BoundingBox);
				}
			}
		}
		ImGui::SameLine(); ImGui::Text("|"); ImGui::SameLine();

		if (ImGui::Button("Refresh Scene"))
		{
			App->file_system->checkDirectoryFiles(App->UI->currentDirectory.c_str(), &App->UI->FilesInDir);
			App->UI->SortFilesinDir();
			App->file_system->RefreshAssets();

		}

		ImGui::End();
	}
}