#include "Globals.h"
#include "Application.h"
#include "ModuleUi.h"
#include "Primitive.h"
#include "ModuleWindow.h"
#include "ModuleSceneIntro.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "ModuleWindow.h"
#include "GameObject.h"
#include "Time.h"
#include "Win_CameraConfig.h"
#include "Win_Inspector.h"
#include "Win_Configuration.h"
#include "FileSystem.h"
#include "glew/include/glew.h"        
#define DROP_ID_HIERARCHY_NODES "hierarchy_node"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	io = new ImGuiIO;
}

ModuleUI::~ModuleUI()
{}

bool ModuleUI::Init()
{


	App->renderer3D->context = SDL_GL_CreateContext(App->window->window);
	SDL_GL_MakeCurrent(App->window->window, App->renderer3D->context);

	bool err = glewInit();


	if (err)
	{
		LOG("Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	/* *io = ImGui::GetIO(); (void)io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    */     // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	   // Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	//ImGui::StyleColorsLight();
	 // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init(App->window->glsl_version);

	// Load Fonts
// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
// - Read 'docs/FONTS.md' for more instructions and details.
// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
//io.Fonts->AddFontDefault();
//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
//IM_ASSERT(font != NULL);



// Load assets

	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	c1 = 0;
	show_demo_window = false;
	show_console = false;
	show_About = false;
	show_Configuration = false;
	active2 = false;
	show_another_window = true;
	resizable_bool = false;
	border_bool = false;
	Wireframe_bool = false;
	Hierarchy_open = true;
	Assetstree_open = true;
	importWindow = false;
	ResourceInfo_open = true;
	Inspector_open = true;
	Console_open = true;
	direction_camera = { 0,0,0 };
	cameras = 0;
	empty_GameObjects = 0;
	width = 960;
	height = 540;
	LocalGuizmo = false;
	WorldGuizmo = true;
	guizmo_mode = ImGuizmo::MODE::WORLD;
	Screen_i = 0;
	Screen_e = 1;
	int max_fps = 61;
	App->Maxfps(max_fps);
	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	selectedAsset = nullptr;
	currentDirectory = "Assets/";
	App->file_system->checkDirectoryFiles(currentDirectory.c_str(), &FilesInDir);
	SortFilesinDir();
	return ret;
}

// Load assets
bool ModuleUI::CleanUp()
{
	LOG("Unloading UI");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}

// Update: draw background
update_status ModuleUI::Update(float dt)
{
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	
	ShowAppinDockSpace(open_docking);
	
	
	ImGui::Begin("Game");
	winSize = ImGui::GetWindowSize();   //this will pick the current window size
	winPos = ImGui::GetWindowPos();


	if (winSize.x != windowSize.x || winSize.y != windowSize.y)
	{
		Change_Window_size(Vec2(winSize.x, winSize.y));
	}
	ImGui::SetCursorPos(ImVec2(image_offset.x, image_offset.y));

	img_corner = Vec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);

	int x = 0;
	int y = 0;
	SDL_GetWindowPosition(App->window->window, &x, &y);
	img_corner -= Vec2(x, y);
	
	imgcorner = Vec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y) + Vec2(0, image_size.y);
	imgcorner.y =App->window->windowSize.y - imgcorner.y; //ImGui 0y is on top so we need to convert 0y on botton

	ImGui::Image((ImTextureID)App->renderer3D->texColorBuffer, ImVec2(image_size.x, image_size.y), ImVec2(0, 1), ImVec2(1, 0));
	ControlsGuizmo();
	GuizmoUI();
	ImGui::End();

	ImGui::BeginMainMenuBar(); //this creates the top bar
	 

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save Scene")) 
		{
			App->serializer->CreateNewScene();
			App->scene_intro->SaveScene(App->scene_intro->scene);
			App->serializer->SaveScene("Scene.json");
		}
		if (ImGui::MenuItem("Load Scene"))
		{
			App->scene_intro->DeleteSceneObjects(App->scene_intro->scene);
			App->serializer->LoadScene("Assets/Scene.json");
		}
		ImGui::Separator();
		if (ImGui::Button("Quit"))
		{
			return UPDATE_STOP;
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Window"))
	{
		if (ImGui::BeginMenu("Menus")) {

			ImGui::MenuItem("Hierarchy", NULL, &Hierarchy_open);
			ImGui::MenuItem("Inspector", NULL, &Inspector_open);
			ImGui::MenuItem("Console", NULL, &Console_open);
			ImGui::MenuItem("Camera Config", NULL, &Config_Camera_open);
			ImGui::EndMenu();
		}
			

		ImGui::EndMenu();
	}
	if (Console_open == true) {
		ShowExampleAppLayout();
	}


	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Gui demo"))
			show_demo_window = true;

		if (ImGui::MenuItem("Documentation"))
			ShellExecuteA(NULL, "open", "https://github.com/Ap011y0n/Motors", NULL, NULL, SW_SHOWNORMAL);

		if (ImGui::MenuItem("Download latest"))
			ShellExecuteA(NULL, "open", "https://github.com/Ap011y0n/Motors/releases", NULL, NULL, SW_SHOWNORMAL);

		if (ImGui::MenuItem("Report bug"))
			ShellExecuteA(NULL, "open", "https://github.com/Ap011y0n/Motors/issues", NULL, NULL, SW_SHOWNORMAL);

		if (ImGui::MenuItem("About"))
			show_About = true;


		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::BeginMenu("Engine Theme"))
		{
			
			if (ImGui::MenuItem("Classic"))
			{
				ImGui::StyleColorsClassic();
			}
			if (ImGui::MenuItem("Dark")) 
			{
				ImGui::StyleColorsDark();
			}
			if (ImGui::MenuItem("Light"))
			{
				ImGui::StyleColorsLight();
			}
			ImGui::EndMenu();
			
		}

		if (ImGui::MenuItem("Configuration"))
			show_Configuration = true;

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("GameObjects"))
	{
		if (ImGui::BeginMenu("Create...")) {

			if (ImGui::MenuItem("Cube")) {
				App->PrimManager->CreateFBXCube();
			}
			if (ImGui::MenuItem("Pyramid")){
				App->PrimManager->CreateFBXPyramid();
			}
			if (ImGui::MenuItem("Sphere")) {
				App->PrimManager->CreateFBXSphere();
			}
			if (ImGui::MenuItem("Cylinder")) {
				App->PrimManager->CreateFBXCylinder();
			}
			
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Camera"))
		{
			cameras++;
			std::string obj = std::to_string(cameras);

			std::string name = "Camera";
			name.append(obj);

			GameObject* camera = new GameObject(name.c_str(), App->scene_intro->scene);
			camera->CreateComponent(ComponentType::TRANSFORM);
			camera->CreateComponent(ComponentType::CAMERA);
		}

		if (ImGui::MenuItem("Empty GameObject"))
		{
			empty_GameObjects++;
			std::string obj = std::to_string(empty_GameObjects);

			std::string name = "Empty_GameObject";
			name.append(obj);

			GameObject* empty_GameObject = new GameObject(name.c_str(), App->scene_intro->scene);
			empty_GameObject->CreateComponent(ComponentType::TRANSFORM);
		}
		ImGui::EndMenu();
	}
	
	ImGui::EndMainMenuBar();
	
	if (show_demo_window == true)
		ImGui::ShowDemoWindow(&show_demo_window);

	AboutMenu(show_About);
	Win_Configuration::UpdateUi(show_Configuration);
	HierarchyWin(); 
	AssetsTree();
	ImportWindow();
	ResourceInfo();
	Win_CameraConfig::UpdateUi(App->camera->cameraComp);
	Win_Inspector::InspectorWin();
	TimeMangmentWin();
	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleUI::PostUpdate(float dt)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
	SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	
	/*glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/

	return UPDATE_CONTINUE;
}

void ModuleUI::AboutMenu(bool show_windoww)
{
	if (show_windoww == true)
	{
		
		ImGui::Begin("About", &show_windoww);
		ImGui::Text("NIDHOGG ENGINE"); //ImGui::SameLine(); ImGui::Text("Nidhogg engine ");
		ImGui::Text("");
		ImGui::TextWrapped("This will be the next best 3D Game Engine");
		ImGui::Text("");
		ImGui::Text("Made by:");
		ImGui::Text("Albert Garcia github -->"); ImGui::SameLine();  
		if (ImGui::Button("Albert github"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/Ap011y0n", NULL, NULL, SW_SHOWNORMAL);
		} 
		ImGui::Text("Pol de la Torre github -->"); ImGui::SameLine(); 
		if (ImGui::Button("Pol github"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/polf780", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::Separator();
		ImGui::Text("3rd Party libraries used");
		ImGui::BulletText("SDL 2.0");
		ImGui::BulletText("SDL Mixer 2.0");
		ImGui::BulletText("Cereal 1.2.2");
		ImGui::BulletText("Glew 2.0.0");
		ImGui::BulletText("ImGui 1.78");
		ImGui::BulletText("MathGeoLib 1.5");
		ImGui::BulletText("OpenGL 3.1");
		ImGui::BulletText("Assimp 3.1.1");
		ImGui::BulletText("Devil 1.7.8");
		ImGui::Separator();
		ImGui::Text("License:");
		ImGui::Text("MIT License");
		ImGui::TextWrapped("Copyright 2020 Pol de la Torre Solé & Albert Garcia Belerda ");
		ImGui::TextWrapped("Permission is hereby granted, free of charge, to any person obtaining a copy"); 
		ImGui::TextWrapped("of this softwareand associated documentation files(the Software), to deal");
		ImGui::TextWrapped("in the Software without restriction, including without limitation the rights ");
		ImGui::TextWrapped("to use, copy, modify, merge, publish, distribute, sublicense, and /or sell ");
		ImGui::TextWrapped("copies of the Software, and to permit persons to whom the Software is");
		ImGui::TextWrapped("furnished to do so, subject to the following conditions :");
		ImGui::TextWrapped("");
		ImGui::TextWrapped("The above copyright noticeand this permission notice shall be included in all");
		ImGui::TextWrapped("copies or substantial portions of the Software.");
		ImGui::TextWrapped("THE SOFTWARE IS PROVIDED ¡'AS IS`, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
		ImGui::TextWrapped("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
		ImGui::TextWrapped("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE");
		ImGui::TextWrapped("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
		ImGui::TextWrapped("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
		ImGui::TextWrapped("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
		ImGui::TextWrapped("SOFTWARE.");
		ImGui::End();
	}
	if (show_windoww == false)
	{
		show_About = false;
		
	}
}

void ModuleUI::StoreLog(const char* message)
{
	string str;
	str = message;
	
	consoleOutput.push_back(str);
}

void ModuleUI::ShowAppinDockSpace(bool* p_open)
{
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", p_open, window_flags);
ImGui::PopStyleVar();

if (opt_fullscreen)
ImGui::PopStyleVar(2);

// DockSpace
ImGuiIO& io = ImGui::GetIO();
if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
{
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
}
else {
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

}

ImGui::End();

}

void ModuleUI::HierarchyWin()
{
	if (Hierarchy_open == true) {

		ImGui::Begin("Hierarchy", &Hierarchy_open);
		std::map<uint, Resource*>::iterator it = App->ResManager->resources.begin();
		for (int i = 0; i < App->scene_intro->scene->childs.size(); i++)
		{
			GameObjectHierarchyTree(App->scene_intro->scene->childs[i], i);
		}

		if (ImGui::BeginPopupContextWindow())
		{
			RightClick_Inspector_Menu();

			ImGui::EndPopup();
		}
		ImGui::End();
	}
}

AssetNode* ModuleUI::createAssetNode(Resource* resource)
{
	AssetNode* node = new AssetNode(resource);
	assets.push_back(node);
	return node;
}

void ModuleUI::AssetsTree()
{
	if (Assetstree_open == true) {

		ImGui::Begin("Assets Tree", &Assetstree_open);

		for (int i = 0; i < assets.size(); i++)
		{

			if (assets[i]->to_delete)
			{
				assets.erase(App->UI->assets.begin() + i);
				i--;
			}
			else
			AssetsHierarchyTree(assets[i]);
		}

		ImGui::End();
	}
}

ImportOptions* ModuleUI::CreateImportObject(const char* importpath, importType type)
{
	ImportOptions* newimport;
	switch (type)
	{
	case importType::UNKNOWN:
		break;
	case importType::MODEL:
		newimport = new ModelOptions(importpath);
		break;
	case importType::TEXTURE:
		newimport = new TextureOptions(importpath);
		break;
	}
	importsvec.push_back(newimport);
	return newimport;
}
void ModuleUI::ImportWindow()
{
	if (importsvec.size() > 0)
		importWindow = true;
	else
		importWindow = false;

	if (importWindow)
	{
		ImGui::Begin("ImportWindow", &importWindow);
		int id = 0;
			switch (importsvec[id]->type)
			{
			case importType::MODEL:
			{
				ModelOptions* modeloptions = (ModelOptions*)importsvec[id];
				static bool axis = false;
				static float f0 = modeloptions->GlobalScale;
				ImGui::InputFloat("GlobalScale", &f0, 0.01f, 1.0f, "%.3f");
				modeloptions->GlobalScale = f0;


				ImGui::Checkbox("Axis", &axis);
				if (axis)
				{
					modeloptions->axis = true;
				}
				static bool cameras = false;

				ImGui::Checkbox("Ignore cams and lights", &cameras);
				if (cameras)
				{
					modeloptions->ignoreCameras = true;
				}
				break;
			}
				
			case importType::TEXTURE:
			{
				TextureOptions* texoptions = (TextureOptions*)importsvec[id];

				static bool f0 = texoptions->filtering;
				ImGui::Checkbox("filtering", &f0);
				texoptions->filtering = f0;

				static bool f1 = texoptions->wrapping;
				ImGui::Checkbox("wrapping", &f1);
				texoptions->wrapping = f1;

				static bool f2 = texoptions->flipXY;
				ImGui::Checkbox("flip X/Y", &f2);
				texoptions->flipXY = f2;

				break;
			}
		
			}

			if (ImGui::Button("Import"))
			{
				App->scene_intro->WantToImport(importsvec[id]);
			}
			ImGui::End();

	}
}
void ModuleUI::ResourceInfo()
{
	if (ResourceInfo_open == true) {

		ImGui::Begin("Resource Info", &ResourceInfo_open);
		if (selectedAsset != nullptr)
		{
			if (ImGui::Button("Add to scene"))
			{
				switch (selectedAsset->owner->GetType())
				{
				case ResourceType::MODEL:
					App->serializer->LoadModel(selectedAsset->owner);
					break;
				case ResourceType::TEXTURE:
					App->FBX->ChangeTexture(selectedAsset->owner);
					break;
				case ResourceType::MESH:
					App->FBX->ChangeMesh(selectedAsset->owner);
					break;
				}
			}
			ImGui::Text("References: %d", selectedAsset->owner->references);
			switch (selectedAsset->owner->GetType())
			{
				
			case ResourceType::MODEL:
				break;
			case ResourceType::TEXTURE:
			{
				ResourceTexture* texture = (ResourceTexture*)selectedAsset->owner;
				ImGui::Text("Wraping: %d", texture->wrapping);
				ImGui::Text("References: %d", texture->filtering);
				ImGui::Text("References: %d", texture->flipXY);
			}
				break;
			case ResourceType::MESH:
				break;
			}
		}

		ImGui::End();
	}
}

void ModuleUI::GameObjectHierarchyTree(GameObject* node, int id)
{
	ImGuiTreeNodeFlags node_flags = /*ImGuiTreeNodeFlags_DefaultOpen | */ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	int node_clicked = -1;

	const char* GameObjname = node->Name.c_str();

	if (App->scene_intro->selectedObj == node)
	{
		node_flags += ImGuiTreeNodeFlags_Selected;
	}
	if (node->childs.empty())
	{
		node_flags += ImGuiTreeNodeFlags_Leaf;
	}
	
	bool open = ImGui::TreeNodeEx(GameObjname, node_flags);

	if (ImGui::IsItemClicked())
	{
		DeactivateGameObjects(App->scene_intro->scene);

		node->isSelected = true;
		App->scene_intro->selectedObj = node;

	}
	DropTrget_In_Inspector(node);
	if (ImGui::BeginDragDropTarget()) {
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DROP_ID_HIERARCHY_NODES, ImGuiDragDropFlags_SourceNoDisableHover);

		if (payload != nullptr) 
		{
			if (payload->IsDataType(DROP_ID_HIERARCHY_NODES)) 
			{
				GameObject* obj = *(GameObject**)payload->Data;

				if (obj != nullptr) //The second part of this is bug that needs to be fixed
				{
					ChangeParent(obj, node);
				}
				
			}
			//ImGui::ClearDragDrop();
		}
		ImGui::EndDragDropTarget();
	}

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoDisableHover)) {
		ImGui::SetDragDropPayload(DROP_ID_HIERARCHY_NODES, &node, sizeof(GameObject), ImGuiCond_Once);
		ImGui::Text(GameObjname);
		ImGui::EndDragDropSource();
	}

	if (open)
	{
		for (int i = 0; i < node->childs.size(); i++)
		{
			GameObjectHierarchyTree(node->childs[i], i);
		}

		ImGui::TreePop();
	}
}

void ModuleUI::DropTrget_In_Inspector(GameObject* node)
{
	ImVec2 low_point = ImGui::GetWindowContentRegionMin();
	ImVec2 top_point = ImGui::GetWindowContentRegionMax();

	low_point.x += ImGui::GetWindowPos().x;low_point.y += ImGui::GetWindowPos().y;
	top_point.x += ImGui::GetWindowPos().x;top_point.y += ImGui::GetWindowPos().y;
	
	ImRect space = { low_point.x, low_point.y, top_point.x, top_point.y };

	if (ImGui::BeginDragDropTargetCustom(space,10)) { //This case is when user drops an object but without doing reparent
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DROP_ID_HIERARCHY_NODES, ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_AcceptNoDrawDefaultRect);

		if (payload != nullptr)
		{
			if (payload->IsDataType(DROP_ID_HIERARCHY_NODES))
			{
				GameObject* obj = *(GameObject**)payload->Data;

				if (obj != nullptr) 
				{
					ChangeParent(obj, App->scene_intro->scene);
				}

			}
		}
		ImGui::EndDragDropTarget();
	}

}

void ModuleUI::AssetsHierarchyTree(AssetNode* node)
{
	ImGuiTreeNodeFlags node_flags = /*ImGuiTreeNodeFlags_DefaultOpen | */ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	int node_clicked = -1;

	std::string file, extension;
	App->file_system->SplitFilePath(node->owner->GetLibraryFile(), &file, &extension);
	file.append(extension);
	node->owner->name = file.c_str();

	const char* GameObjname = node->owner->name;

	if (node->is_selected == true)
	{
		node_flags += ImGuiTreeNodeFlags_Selected;
	}



	bool open = ImGui::TreeNodeEx(GameObjname, node_flags);

	if (ImGui::IsItemClicked())
	{
		DeactivateAssets();
		node->is_selected = true;
		selectedAsset = node;
	}

	if (open)
	{
		ImGui::TreePop();
	}

}
void ModuleUI::ChangeParent(GameObject* obj, GameObject* nextOwner)
{
	if (obj != nullptr && nextOwner != nullptr) {

		//obj->parent->to_delete = true;

		for (int i = 0; i < obj->parent->childs.size(); i++)
		{
			if (obj->parent->childs[i] == obj)
			{
				obj->parent->childs.erase(obj->parent->childs.begin() + i);
				i--;
				
			}
		}
		obj->parent = nextOwner;
		nextOwner->childs.push_back(obj);
	}
}

void ModuleUI::DeactivateGameObjects(GameObject* father)
{
	father->isSelected = false;
	for (int i = 0; i < father->childs.size(); i++)
	{
		DeactivateGameObjects(father->childs[i]);
	}
}

void ModuleUI::DeactivateAssets()
{
	selectedAsset = nullptr;
	for (int i = 0; i < assets.size(); i++)
	{
		assets[i]->is_selected = false;
	}
}

void ModuleUI::ShowExampleAppLayout(/*bool* p_open*/)
{
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		currentDirectory = "Assets/library";
		FilesInDir.clear();
		App->file_system->checkDirectoryFiles(currentDirectory.c_str(), &FilesInDir);
		SortFilesinDir();

	}
	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Example: Simple layout",&Console_open))
	{
		{
			ImGui::BeginGroup();
			if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Console"))
				{
					for (int i = 0; i < consoleOutput.size(); i++)
					{
						const char* text = consoleOutput[i].c_str();
						ImGui::Text(text);
					}
					ImGui::EndTabItem();
				}
			

				if (ImGui::BeginTabItem("Assets"))
				{
					ImGui::Columns(4, NULL, false);
			
					//	if (ImGui::Selectable(label, &selected[i])) {}

					for (int i = 0; i < FilesInDir.size(); i++)
					{
						std::string filedir = FilesInDir[i]->file.c_str();
						filedir += FilesInDir[i]->extension.c_str();
						ImGui::ImageButton((ImTextureID)App->scene_intro->FolderIco, { 64, 64 }, ImVec2( 0,1 ), ImVec2(1,0));
						ImGui::Text("%s", filedir.c_str());
						ImGui::Dummy(ImVec2(0.0f, 20.0f));

						ImGui::NextColumn();

					}
					ImGui::Columns(1);
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			
			ImGui::EndGroup();
		}
	}
	ImGui::End();
}

void ModuleUI::SortFilesinDir()
{
	vector< UiFile*> NewVec;
	for (int i = 0; i < FilesInDir.size(); i++)
	{
		if (FilesInDir[i]->extension == "")
		{
			NewVec.push_back(FilesInDir[i]);
		}
	}
	for (int i = 0; i < FilesInDir.size(); i++)
	{
		if (FilesInDir[i]->extension == "json")
		{
			NewVec.push_back(FilesInDir[i]);
		}
	}

	for (int i = 0; i < FilesInDir.size(); i++)
	{
		std::string path = FilesInDir[i]->file;
		path += FilesInDir[i]->extension;
		ResourceType type = App->ResManager->ReturnType(path.c_str());
		if(type == ResourceType::MODEL)
		NewVec.push_back(FilesInDir[i]);
		
	}

	for (int i = 0; i < FilesInDir.size(); i++)
	{
		std::string path = FilesInDir[i]->file;
		path += FilesInDir[i]->extension;
		ResourceType type = App->ResManager->ReturnType(path.c_str());
		if (type == ResourceType::TEXTURE)
			NewVec.push_back(FilesInDir[i]);

	}

	for (int i = 0; i < FilesInDir.size(); i++)
	{
		std::string path = FilesInDir[i]->file;
		path += FilesInDir[i]->extension;
		ResourceType type = App->ResManager->ReturnType(path.c_str());
		if (type == ResourceType::MESH)
			NewVec.push_back(FilesInDir[i]);

	}

	for (int i = 0; i < FilesInDir.size(); i++)
	{
		if (FilesInDir[i]->extension == "meta")
		{
			NewVec.push_back(FilesInDir[i]);
		}
	}


	FilesInDir = NewVec;

}
void ModuleUI::Change_Window_size(Vec2 newSize)
{
	//Get window size
	win_size = newSize;
	float offset = 10.0f;
	//Creates window size parameter
	image_size = App->window->windowSize ;
	if (image_size.x > win_size.x - offset)
	{image_size /= (image_size.x / (win_size.x - offset));}
	if (image_size.y > win_size.y - offset)
	{image_size /= (image_size.y / (win_size.y - offset));}
	image_offset = Vec2(win_size.x - (offset/2) - image_size.x, win_size.y - (offset / 2) - image_size.y);
	image_offset = image_offset / 2;
}


void ModuleUI::GuizmoUI() 
{
	
	GameObject* gameObject = App->scene_intro->selectedObj;
	using_gizmo = false;

	if (gameObject != nullptr)
	{
		ComponentTransform* transform = (ComponentTransform*)gameObject->GetComponent(ComponentType::TRANSFORM);

		float4x4 view = App->camera->cameraComp->frustrum.ViewMatrix();
		view.Transpose();
		
		float4x4 projection = App->camera->cameraComp->frustrum.ProjectionMatrix();
		projection.Transpose();

		float4x4 modelProjection = transform->local_transform;
		modelProjection.Transpose();

		ImGuizmo::SetDrawlist();
		cornerPos = Vec2(imgcorner.x, App->window->windowSize.y - imgcorner.y - image_size.y);
		ImGuizmo::SetRect(imgcorner.x, cornerPos.y, image_size.x, image_size.y);

		ImGuizmo::Manipulate(view.ptr(), projection.ptr(),guizmo_type, guizmo_mode, modelProjection.ptr());
		if (ImGuizmo::IsUsing())
		{
			using_gizmo = true;
			float4x4 MovementMatrix;
			MovementMatrix.Set(modelProjection);
			transform->local_transform = MovementMatrix.Transposed();
			transform->UpdateFromGuizmo(transform->local_transform);
		}
	}
}

void ModuleUI::ControlsGuizmo()
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) 
	{
		guizmo_mode = ImGuizmo::MODE::LOCAL;
	} 
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) 
	{
		guizmo_mode = ImGuizmo::MODE::WORLD;
	} 
}

void ModuleUI::TimeMangmentWin()
{
	if (Inspector_open == true)
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
		//LOG("time: %f", Time::Game_Timer.ReadSec());
		ImGui::Text("|"); ImGui::SameLine();
		{
			ImGui::SetNextItemWidth(130);
			static int selectedMode = 0;
			static const char* Mode[]{ "WORLD","LOCAL" };
			ImGui::Combo("Mode", &selectedMode, Mode, IM_ARRAYSIZE(Mode)); ImGui::SameLine();
			if (selectedMode == 1)
			{
				guizmo_mode = ImGuizmo::MODE::LOCAL;
			}
			if (selectedMode == 0)
			{
				guizmo_mode = ImGuizmo::MODE::WORLD;
			}

		}
		ImGui::Text("|"); ImGui::SameLine();
		static bool BoundingBox = false;
		if (ImGui::Checkbox("BoundingBox",&BoundingBox))
		{
			for (int i = 0; i < App->scene_intro->scene->childs.size(); i++) 
			{
				Change_Visibility_BoundingBoxes(App->scene_intro->scene->childs[i],BoundingBox);
			}
		}

		ImGui::End();
	}

}

void ModuleUI::RightClick_Inspector_Menu()
{
	if (ImGui::MenuItem("Delete"))
	{
		App->scene_intro->selectedObj->to_delete = true;
	}

	if (ImGui::MenuItem("Create empty Child"))
	{
		empty_GameObjects++;
		std::string obj = std::to_string(empty_GameObjects);

		std::string name = "Empty_Child";
		name.append(obj);

		GameObject* empty_GameObject = new GameObject(name.c_str(), App->scene_intro->selectedObj);
		empty_GameObject->CreateComponent(ComponentType::TRANSFORM);
	}

	if (ImGui::MenuItem("Create empty GameObject"))
	{
		empty_GameObjects++;
		std::string obj = std::to_string(empty_GameObjects);

		std::string name = "Empty_GameObject";
		name.append(obj);

		GameObject* empty_GameObject = new GameObject(name.c_str(), App->scene_intro->scene);
		empty_GameObject->CreateComponent(ComponentType::TRANSFORM);
	}

}

void ModuleUI::Change_Visibility_BoundingBoxes(GameObject* node,bool visibility)
{
	node->displayAABB = visibility;
	for (int i = 0; i < node->childs.size(); i++)
	{
		Change_Visibility_BoundingBoxes(node->childs[i], visibility);
	}
}



ImportOptions::ImportOptions()
{
	path = "";
	type = importType::UNKNOWN;
}

ImportOptions::ImportOptions(const char* importpath)
{
	path = importpath;
	type = importType::UNKNOWN;
}
ImportOptions::~ImportOptions()
{

}

ModelOptions::ModelOptions(const char* importpath)
{
	type = importType::MODEL;
	path = importpath;
	GlobalScale = 1;
	axis = false;
	ignoreCameras = false;
}
ModelOptions::~ModelOptions() {

}

TextureOptions::TextureOptions(const char* importpath)
{
	type = importType::TEXTURE;
	path = importpath;
	filtering = false;
	wrapping = false;
	flipXY = false;
}
TextureOptions::~TextureOptions() {

}
