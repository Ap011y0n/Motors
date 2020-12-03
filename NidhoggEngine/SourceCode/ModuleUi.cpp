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
#include "glew/include/glew.h"
#include "GameObject.h"
#include "Time.h"


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
	i = 0;
	e = 1;
	int max_fps = 61;
	App->Maxfps(max_fps);
	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	selectedAsset = nullptr;
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
	Configuration(show_Configuration);
	HierarchyWin(); 
	AssetsTree();
	ImportWindow();
	ResourceInfo();
	App->scene_intro->Camera_Editor_Window(App->camera->cameraComp);
	InspectorWin();
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

void ModuleUI::Configuration(bool show_config)
{

	if (show_config == true)
	{
		ImGui::Begin("Configuration", &show_config);

		ImGui::MenuItem("Wellcome to the Configuration menu", NULL, false, false);

		if (ImGui::CollapsingHeader("Application"))
		{
			static char buf[32] = "";
			ImGui::InputText("App name", buf, IM_ARRAYSIZE(buf));
			SDL_SetWindowTitle(App->window->window, buf);

			static char buf2[32] = "";
			ImGui::InputText("Organitzation", buf2, IM_ARRAYSIZE(buf));
			PlotGraph();
		}

		if (ImGui::CollapsingHeader("Window"))
		{
			static bool Wireframe_visible = false;
			ImGui::Checkbox("Wirframe visible", &Wireframe_visible); ImGui::SameLine();
			if (Wireframe_visible)
			{
				if (Wireframe_bool)
				{
					Wireframe_bool = false;
					App->PrimManager->ShowWireframe();
				}
				
			}
			else {
				if (!Wireframe_bool)
				{
					App->PrimManager->HideWireframe();
				}
					Wireframe_bool = true;
				
			}

			static bool Depth = false;
			ImGui::Checkbox("Deph", &Depth);
			if (Depth) {
				App->PrimManager->depth_bool = true;
			}
			else {
				App->PrimManager->depth_bool = false;
			}

			static bool cullface = false;
			ImGui::Checkbox("CullFace", &cullface); ImGui::SameLine();
			if (cullface) {
				App->PrimManager->cullface_bool = true;
			}
			else {
				App->PrimManager->cullface_bool = false;
			}

			static bool texture2D = true;
			ImGui::Checkbox("Texture 2D", &texture2D); 
			if (texture2D) {
				App->PrimManager->texture2D_bool = true;
			}
			else {
				App->PrimManager->texture2D_bool = false;
			}
			
			static bool Lighting = false;
			ImGui::Checkbox("No Lighting", &Lighting); ImGui::SameLine();
			if (Lighting) {
				App->PrimManager->LIGHTING_bool = true;
			}
			else {
				App->PrimManager->LIGHTING_bool = false;
			}

			static bool Color = false;
			ImGui::Checkbox("Disable Colors", &Color);
			if (Color) {
				App->PrimManager->Color_bool = true;
			}
			else {
				App->PrimManager->Color_bool = false;
			}

			
			static float f1 = 1.0f;
			ImGui::SliderFloat("Brightness", &f1, 0.0f, 1.0f, "%.3f");
			if (ImGui::IsItemActive())
			{
				SDL_SetWindowBrightness(App->window->window, f1);
			}
			
			static int i1 = App->window->screen_surface->w;
			static int i2 = App->window->screen_surface->h;
			ImGui::SliderInt("width", &i1, 640, 1920);
			if (ImGui::IsItemActive())
			{
				SDL_SetWindowSize(App->window->window, i1, i2);
				//App->renderer3D->OnResize(i1, i2); if we don not coment this will change the width and heigh from the
			}
			ImGui::SliderInt("height", &i2, 480, 1080);
			if (ImGui::IsItemActive())
			{
				SDL_SetWindowSize(App->window->window, i1, i2);
				//App->renderer3D->OnResize(i1, i2);
			}
			
			ImGui::Text("Refresh rate: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1.f), "%d ",App->GetFPS());

			ImGui::Checkbox("Fullscreen", &active2); ImGui::SameLine();
			App->window->Fullscreen_UI(active2);
			
		
			
			ImGui::Checkbox("Resizable", &resizable_bool);
			static int j = 0;
			if (resizable_bool == false && j == 0)
			{
				j = 1;
				SDL_SetWindowResizable(App->window->window, SDL_FALSE);
			}
			else if (resizable_bool == true && j == 1)
			{
				j = 0;
				SDL_SetWindowResizable(App->window->window, SDL_TRUE);
			}

			static int j2 = 0;
			ImGui::Checkbox("Borderless", &border_bool); ImGui::SameLine();
			if (border_bool == false && j2 == 0)
			{
				j2 = 1;
				SDL_SetWindowBordered(App->window->window, SDL_TRUE );
			}
			else if (border_bool == true && j2 == 1)
			{
				j2 = 0;
				SDL_SetWindowBordered(App->window->window, SDL_FALSE);
			}

			static bool bool_fullscreeen_Desktop = false;
			ImGui::Checkbox("Full Desktop", &bool_fullscreeen_Desktop);

			if (bool_fullscreeen_Desktop == true && i == 0)
			{
				e = 0;
				i++;
				SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
			if (bool_fullscreeen_Desktop == false && e == 0)
			{
				SDL_SetWindowFullscreen(App->window->window, 0);
				i = 0;
				e++;
			}
		}

		if (ImGui::CollapsingHeader("File System"))
		{

		}

		if (ImGui::CollapsingHeader("Input"))
		{
			int mousex=0; int mousey=0;
			SDL_GetMouseState(&mousex, &mousey);
			ImGui::Text("Mouse Position:"); ImGui::SameLine();  ImGui::TextColored(ImVec4(1, 1, 0, 1.f), "%dx %dy", mousex,mousey);

			ImGuiIO& io = ImGui::GetIO();
			ImGui::Text("Mouse Motion: ");  ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1.f),"% g, % g", io.MouseDelta.x, io.MouseDelta.y);

			ImGui::Text("Mouse wheel:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1.f), "%.0f",io.MouseWheel);
			ImGui::Separator();

			ImGui::Text("Mouse down:");     
			for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) 
			if (io.MouseDownDuration[i] >= 0.0f) 
			{
				ImGui::SameLine(); 
				ImGui::TextColored(ImVec4(1, 1, 0, 1.f), "b%d (%.02f secs)", i, io.MouseDownDuration[i]);
			}

			ImGui::Text("Keys down:");
			for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
				if (io.KeysDownDuration[i] >= 0.0f)
				{
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(1, 1, 0, 1.f), "%d (0x%X) (%.02f secs)", i, i, io.KeysDownDuration[i]);
				}
		}

		if (ImGui::CollapsingHeader("Hardware"))
		{
			SDL_version compiled;
			SDL_VERSION(&compiled);
			static bool active = false;
			ImGui::Checkbox("Active", &active);
			ImGui::Text("SDL Version :"); ImGui::SameLine();  ImGui::TextColored(ImVec4(1, 1, 0, 1.f),"%d.%d.%d",compiled.major, compiled.minor, compiled.patch);
			ImGui::Separator(); ImGui::Spacing();
			ImGui::Text("Number of logical CPU cores:"); ImGui::SameLine();  ImGui::TextColored(ImVec4(1, 1, 0, 1.f),"%d cores (Cache: %dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize()); 
			
			ImGui::Text("System Ram:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1.f),"%dGB", (SDL_GetSystemRAM()/1000));ImGui::Spacing();
			
			ImGui::Text("Caps:"); ImGui::SameLine(); ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1.f));
			if (SDL_HasRDTSC())ImGui::Text("RDTSC,"); ImGui::SameLine();
			if (SDL_HasMMX())ImGui::Text("MMX,"); ImGui::SameLine();
			if (SDL_HasMMX())ImGui::Text("MMX,"); ImGui::SameLine();
			if (SDL_HasAltiVec())ImGui::Text("AltiVec,"); ImGui::SameLine();
			if (SDL_Has3DNow())ImGui::Text("3DNow,"); ImGui::SameLine();
			if (SDL_HasSSE())ImGui::Text("SSE,"); ImGui::SameLine();
			if (SDL_HasSSE2())ImGui::Text("SSE2,"); ImGui::SameLine();
			if (SDL_HasSSE3())ImGui::Text("SSE3,"); ImGui::SameLine();
			if (SDL_HasSSE41())ImGui::Text("SSE41,"); ImGui::SameLine();
			if (SDL_HasSSE42())ImGui::Text("SSE42,"); ImGui::SameLine();
			if (SDL_HasAVX())ImGui::Text("AVX,"); ImGui::SameLine();
			if (SDL_HasAVX2())ImGui::Text("AVX2,"); ImGui::SameLine();
			if (SDL_HasAVX512F())ImGui::Text("AVX512F,"); ImGui::SameLine();
			if (SDL_HasARMSIMD())ImGui::Text("ARMSIMD,"); ImGui::SameLine();
			if (SDL_HasNEON())ImGui::Text("NEON, ");
			ImGui::PopStyleColor();
		
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Text("Brand: ");  ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1.f), "%s", glGetString(GL_VENDOR));// Returns the vendor
			ImGui::Text("Graphic Card: ");  ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1.f), "%s", glGetString(GL_RENDERER));// Returns a hint to the model
		}

		ImGui::End();
	}

	if (show_config == false)
	{
		show_Configuration = false;

	}
}

void ModuleUI::StoreLog(const char* message)
{
	string str;
	str = message;
	
	consoleOutput.push_back(str);
}

void ModuleUI::PlotGraph()
{
	if (ImGui::SliderInt("Maximum FPS", &max_fps, -1, 200, NULL))
	{
		App->Maxfps(max_fps);
	}
	bool active = true;
	int fps = App->GetFPS();
	if (fpsecond.size() > 120) 
	{
		for (int i = 1; i < fpsecond.size(); i++)
		{
			fpsecond[i - 1] = fpsecond[i];      //with this we change the frist for the 2nd to update all
		}
		fpsecond[fpsecond.size() - 1] = fps;
	}
	else
	{fpsecond.push_back(fps);}
	char text[20];
	sprintf_s(text, 20, "Frames: %d", fps);
	ImGui::Text(text);
	ImGui::PlotHistogram("Framerate", &fpsecond[0], fpsecond.size(), 0, text, 0.0f, 100.0f, ImVec2(450, 100));
	
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

void ModuleUI::CreateImportObject(const char* importpath, importType type)
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
		
			switch (importsvec[0]->type)
			{
			case importType::MODEL:
			{
				ModelOptions* modeloptions = (ModelOptions*)importsvec[0];
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
				TextureOptions* texoptions = (TextureOptions*)importsvec[0];

				static float f0 = texoptions->filtering;
				ImGui::InputFloat("filtering", &f0, 0.01f, 1.0f, "%.3f");
				texoptions->filtering = f0;

				static float f1 = texoptions->wrapping;
				ImGui::InputFloat("wrapping", &f1, 0.01f, 1.0f, "%.3f");
				texoptions->wrapping = f1;

				break;
			}
		
			}

			if (ImGui::Button("Import"))
			{
				App->scene_intro->WantToImport(importsvec[0]);
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

void ModuleUI::GameObjectInspector(GameObject* obj)
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
		if(ImGui::InputText("Name", str0, IM_ARRAYSIZE(str0)))
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
			ImGui::Checkbox("Active",&active); ImGui::NextColumn(); ImGui::Text("x"); ImGui::NextColumn(); ImGui::Text("y"); ImGui::NextColumn(); ImGui::Text("z"); ImGui::NextColumn();
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

			ImGui::DragFloat("   ", &s1);
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
			ImGui::DragFloat("      .", &s2);
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
			ImGui::DragFloat("         ", &s3);
			if (ImGui::IsItemActive())
			{
				transform->Scale(0, 0, s3 - transform->scale.z);

			}

			//transform->rot = Quat::FromEulerXYZ(r1 * pi / 180, r2 * pi / 180, r3 * pi / 180);
			ImGui::NextColumn();

			ImGui::Columns(1);
			ImGui::TreePop();
			
			direction_camera = { transform->pos.x,transform->pos.y,transform->pos.z };
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
				if(!mesh->triggerNormals)
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
			ImGui::Text("Texture h: %d", material->texture_h ); ImGui::SameLine(); ImGui::Text(" w:%d", material->texture_w);
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
			ImGui::Text("Horizontal FOV: %f",camera->GetHorizontalFov());
			ImGui::Text("Vertical FOV: %f", camera->GetFOV());
			ImGui::Text("Aspect ratio: %f", camera->aspectRatio);
			ImGui::TreePop();
		}

	}
	
}

void ModuleUI::InspectorWin()
{
	if (Inspector_open == true)
	{
		ImGui::Begin("Inspector",&Inspector_open);
		if(App->scene_intro->selectedObj != nullptr)
		GameObjectInspector(App->scene_intro->selectedObj);
			
		ImGui::End();
	}
}

void ModuleUI::ShowExampleAppLayout(/*bool* p_open*/)
{
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
					ImGui::Text("Folders...");

					
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			
			ImGui::EndGroup();
		}
	}
	ImGui::End();
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
	ignoreCameras = true;
}
ModelOptions::~ModelOptions() {

}

TextureOptions::TextureOptions(const char* importpath)
{
	type = importType::TEXTURE;
	path = importpath;
	filtering = 0;
	wrapping = 0;
	flipXY = false;
}
TextureOptions::~TextureOptions() {

}
