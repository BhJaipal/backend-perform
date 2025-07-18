#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <cctype>
#include <iostream>
#include <string>

static void glfw_error_callback(int error, const char *description) {
	std::cerr << "Glfw Error " << error << description << "\n";
}

enum FormAlert {
	Started = 0,
	EmptyPass = 1,
	EmptyName = 2,
	EmptyBoth = 3,
	Filled = 4
};

int status = FormAlert::Started;

std::string trim(char *src) {
	std::string out = "", str = src;
	int startSpace = false, middleSpace = -1;
	for (size_t i = 0; i < str.size(); i++) {
		if (i == 0 && std::isspace(str[0])) {
			startSpace = true;
			continue;
		}
		if (startSpace) {
			if (std::isspace(str[i]))
				continue;
			startSpace = false;
			out += str[i];
			continue;
		}
		if (std::isspace(str[i])) {
			if (middleSpace != -1) continue;
			else {
				middleSpace = i;
			}
		} else {
			if (middleSpace != -1) {
				out += str.substr(middleSpace, i);
				out += str[i];
			} else {
				out += str[i];
			}
		}
	}
	return out;
}

void submit(char c_name[21], char c_pass[65]) {
	std::string name = trim(c_name),
		pass = trim(c_pass);
	if (name.empty() && pass.empty()) {
		status = FormAlert::EmptyBoth;
		return;
	} else if (pass.empty()) {
		status = FormAlert::EmptyPass;
		return;
	} else if (name.empty()) {
		status = FormAlert::EmptyName;
		return;
	}
	status = FormAlert::Filled;
}

char name[21] = "";
char pass[65] = "";

void login() {
	ImGui::BeginTable("table1", 2);
	ImGui::TableNextRow();

	if (status & FormAlert::EmptyName) {
		ImGui::TableNextColumn();
		ImGui::Text("Name is empty");
		ImGui::TableNextRow();
	}
	ImGui::TableNextColumn();
	ImGui::Text("Name:");
	ImGui::TableNextColumn();
	ImGui::PushItemWidth(-1);
	if (ImGui::InputText("name", name, 20)) {
		status |= trim(name).empty() ? FormAlert::EmptyName : FormAlert::Started;
	}
	ImGui::PopItemWidth();

	ImGui::TableNextRow();
	if (status & FormAlert::EmptyPass) {
		ImGui::TableNextColumn();
		ImGui::Text("Password is empty");
		ImGui::TableNextRow();
	}
	ImGui::TableNextColumn();
	ImGui::Text("Password:");
	ImGui::TableNextColumn();
	ImGui::PushItemWidth(-1);
	if (ImGui::InputText("pass", pass, 64)) {
		status |= trim(pass).empty() ? FormAlert::EmptyPass : FormAlert::Started;
	}
	ImGui::PopItemWidth();

	ImGui::TableNextColumn();
	if (ImGui::Button("Submit")) {
		submit(name, pass);
	}

	ImGui::EndTable();
}
void chatPage() {}

bool loggedIn = false;

int main() {
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	const char* glsl_version = "#version 300 es";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(1280, 720, "Dear ImGui - Conan", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	io.DisplaySize = ImVec2(1920, 1080);
    io.DeltaTime = 1.0f / 60.0f;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// render your GUI
		ImGui::Begin("Demo window");
		if (!loggedIn)
			login();
		else chatPage();
		
		ImGui::End();

		// Render dear imgui into screen
		ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
}
