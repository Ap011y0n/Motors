#include "Globals.h"
#include "Application.h"
#include "ModuleUi.h"
#include "Primitive.h"
#include "ModuleWindow.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "ModuleWindow.h"
#include "glew/include/glew.h"


#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include "GL/gl3w.h"           // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include "glew/include/glew.h"          // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>          // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#include <glad/gl.h>            // Initialize with gladLoadGL(...) or gladLoaderLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h>// Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif


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

	// Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
	bool err = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
	bool err = false;
	glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
	bool err = false;
	glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)SDL_GL_GetProcAddress(name); });
#else
	bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
	if (err)
	{
		LOG("Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	*io = ImGui::GetIO(); (void)io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	   // Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	//ImGui::StyleColorsLight();
	 // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
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

	LOG("Loading Intro assets");
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
	i = 0;
	e = 1;
	int max_fps = 61;
	App->Maxfps(max_fps);
	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		LOG("Hello World");
	}
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	ImGui::BeginMainMenuBar(); //this creates the top bar


	if (ImGui::BeginMenu("Console"))
	{
		
			show_console = true;

		ImGui::EndMenu();
	}
	


	if (show_console)
	{
		ImGui::Begin("Console", &show_console);

		for (int i = 0; i < consoleOutput.size(); i++)
		{
			const char* text = consoleOutput[i].c_str();
			ImGui::Text(text);
		}
		ImGui::End();
	}
	

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::Button("Quit"))
		{
			return UPDATE_STOP;
		}
		ImGui::EndMenu();
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

	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Configuration"))
			show_Configuration = true;

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	if (show_demo_window == true)
		ImGui::ShowDemoWindow(&show_demo_window);
	
	AboutMenu(show_About);
	Configuration(show_Configuration);

	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleUI::PostUpdate(float dt)
{
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
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::MenuItem("Save");
			ImGui::MenuItem("Load");
			ImGui::MenuItem("Reset to Default");
			ImGui::End();
		}
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
			if (Wireframe_visible) {
				Wireframe_bool = true;
			}
			else {
				Wireframe_bool = false;
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

			static bool texture2D = false;
			ImGui::Checkbox("Texture 2D", &texture2D); 
			if (texture2D) {
				App->PrimManager->texture2D_bool = true;
			}
			else {
				App->PrimManager->texture2D_bool = false;
			}
			
			static bool Lighting = false;
			ImGui::Checkbox("Lighting", &Lighting); ImGui::SameLine();
			if (Lighting) {
				App->PrimManager->LIGHTING_bool = true;
			}
			else {
				App->PrimManager->LIGHTING_bool = false;
			}

			static bool Color = false;
			ImGui::Checkbox("Colors", &Color);
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
				App->renderer3D->OnResize(i1, i2);
			}
			ImGui::SliderInt("height", &i2, 480, 1080);
			if (ImGui::IsItemActive())
			{
				SDL_SetWindowSize(App->window->window, i1, i2);
				App->renderer3D->OnResize(i1, i2);
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
	{
		fpsecond.push_back(fps);
	}
	char text[20];
	sprintf_s(text, 20, "Frames: %d", fps);
	ImGui::Text(text);
	ImGui::PlotHistogram("Framerate", &fpsecond[0], fpsecond.size(), 0, text, 0.0f, 100.0f, ImVec2(450, 100));
	
}
