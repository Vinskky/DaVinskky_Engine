#include "Application.h"
#include "Globals.h"
#include "imgui.h"
#include "E_Configuration.h"

E_Configuration::E_Configuration(Application* app, const char* name, bool isActive): Editor(app,"Configuration")
{
	appName = TITLE;
	orgName = "Organization";
	fps = 0;
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
	checkResizable = WIN_RESIZABLE;
	checkBorderless = WIN_BORDERLESS;
	checkFullDesktop = WIN_FULLSCREEN_DESKTOP;
}

E_Configuration::~E_Configuration()
{
}

bool E_Configuration::Draw(ImGuiIO& io)
{
	bool ret = true;
	ImGui::Begin(GetName(), nullptr, ImGuiWindowFlags_None);
	
	OptionsPanel();
	ApplicationHeader();
	WindowHeader();
	FileSystemHeader();
	InputHeader();
	HardwareHeader();

	ImGui::End();
	return ret;
}

bool E_Configuration::CleanUp()
{
	bool ret = true;

	return ret;
}

void E_Configuration::UpdateFrameData(int frames, int ms)
{
	for (uint i = 0; i < (MAX_HISTOGRAM_SIZE - 1); ++i)
	{
		fpsData[i] = fpsData[i + 1];
		msData[i] = msData[i + 1];
	}
	fpsData[MAX_HISTOGRAM_SIZE - 1] = (float)frames;
	msData[MAX_HISTOGRAM_SIZE - 1] = (float)ms;
}

bool E_Configuration::OptionsPanel()
{
	bool ret = true;
	
	if (ImGui::BeginMenu("Options"))
	{
		if (ImGui::MenuItem("Default Settings"))
		{
			//Reset to default values
		}
		if (ImGui::MenuItem("Load"))
		{
			//Parser + JSON
		}	
		if (ImGui::MenuItem("Save"))
		{
			//Parser + JSON
		}	
		ImGui::EndMenu();
	}
	

	return ret;
}

bool E_Configuration::ApplicationHeader()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("Application"))
	{
		char buffer[128];
		strcpy_s(buffer, appName);
		if (ImGui::InputText("Engine Name", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			//modify variable
		}
		//ImGui::SameLine();
		//ImGui::Text("App Name");

		char bufferOrg[128];
		strcpy_s(bufferOrg, orgName);
		if (ImGui::InputText("Organization", bufferOrg, IM_ARRAYSIZE(bufferOrg), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			//modify variable
		}

		ImGui::SliderInt("Max FPS", &fps, 0, 144);
		ImGui::Text("Limit Framerate: %d", fps);
		//Plots
		PlotFrameHistogram();
		
	}

	return ret;
}

bool E_Configuration::PlotFrameHistogram()
{
	ImGui::PlotHistogram("FPS", fpsData, IM_ARRAYSIZE(fpsData), 0, NULL, 0.0f, 144.0f, ImVec2(0, 80));
	ImGui::PlotHistogram("MS", msData, IM_ARRAYSIZE(msData), 0, NULL, 0.0f, 40.0f, ImVec2(0, 80));
	return true;
}

bool E_Configuration::WindowHeader()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("Window"))
	{
		if (ImGui::Checkbox("Active",&checkActive ))
		{
			//activate / deactivate window??
			checkActive = !checkActive;
		}
		ImGui::Text("Icon: %s", iconStr);
		ImGui::SliderFloat("Brightness", &brightness, 0.000f, 1.000f);
		ImGui::SliderInt("Width", &width, 720, 2560);
		ImGui::SliderInt("Height", &height, 480, 1440);
		ImGui::Text("Refresh rate: %d", fps);

		if (ImGui::Checkbox("Fullscreen", &checkFullscreen))
			checkFullscreen = !checkFullscreen;

		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &checkResizable))
			checkResizable = !checkResizable;

		if (ImGui::Checkbox("Borderless", &checkBorderless))
			checkBorderless = !checkBorderless;
		
		ImGui::SameLine();
		if (ImGui::Checkbox("Full Desktop", &checkFullDesktop))
			checkFullDesktop = !checkFullDesktop;
	}
	

	return ret;
}

bool E_Configuration::FileSystemHeader()
{
	bool ret = true;
	if (ImGui::CollapsingHeader("File System"))
	{
		if (ImGui::Checkbox("Active", &ckActiveFS))
			ckActiveFS = !ckActiveFS;

		ImGui::Text("Base Path: \n %s", basePath);
		ImGui::Text("Read Path: \n %s", readPath);
		ImGui::Text("Write Path: \n %s", writePath);

	}
	return ret;
}

bool E_Configuration::InputHeader()
{
	bool ret = true;
	if (ImGui::CollapsingHeader("Input"))
	{
		if (ImGui::Checkbox("Active", &ckActiveInput))
			ckActiveInput = !ckActiveInput;
		
		ImGui::Text("Mouse Position: %i, %i", App->input->GetMouseX(), App->input->GetMouseY());
		ImGui::Text("Mouse Motion: %i, %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
		ImGui::Text("Mouse wheel: %i", App->input->GetMouseZ());
		ImGui::Separator();
		
		ImGui::BeginChild("Input Log");
		ImGui::TextUnformatted(logInputText.begin());
		ImGui::SetScrollHereY(1.0f);
		ImGui::EndChild();
	}
	return ret;
}

bool E_Configuration::HardwareHeader()
{
	bool ret = true;
	if (ImGui::CollapsingHeader("Hardware"))
	{
		int major, minor, patch;
		App->GetSDLVer(major, minor, patch);
		ImGui::Text("SDL Version: %d.%d.%d", major, minor, patch);
		ImGui::Separator();
		int count, size;
		App->GetCPU(count, size);
		ImGui::Text("CPUs: %d (%dKb)", count, size);
		float ram = App->GetRAM();
		ImGui::Text("RAM: %.2fGb", ram);
		ImGui::Separator();
		bool threeD, altiVec, avx, avx2, mmx, rdtsc, sse, sse2, sse3, sse41, sse42;
		App->GetCaps(threeD, altiVec, avx, avx2, mmx, rdtsc, sse, sse2, sse3, sse41, sse42);
		ImGui::Text("Caps: %s%s%s%s%s%s", threeD ? "3DNow, " : "", altiVec ? "AltiVec, " : "", avx ? "AVX, " : "", avx2 ? "AVX2, " : "", mmx ? "MMX, " : "", rdtsc ? "RDTSC, " : "");
		ImGui::Text("", "%s%s%s%s%s", sse ? "SSE, " : "", sse2 ? "SSE2, " : "", sse3 ? "SSE3, " : "", sse41 ? "SSE41, " : "", sse42 ? "SSE42" : "");

		ImGui::Separator();

		//Missing GPU
	}
	return ret;
}
