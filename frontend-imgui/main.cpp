#include "json/value.h"
#include <GLFW/glfw3.h>
#include <cctype>
#include <iostream>
#include <jsoncpp/json/reader.h>
#include <map>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "cpp-httplib/httplib.h"
#include <json/json.h>

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

enum LoginStatus {
	LoginSuccess,
	LoginFailed,
	LoginNotYet
};
LoginStatus login_status = LoginNotYet;

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

	std::string json = "{\"name\":\"" + name + "\",\"password\":\"" + pass + "\"}";
	httplib::Client client("http://localhost:8000");
	httplib::Result res = client.Post("/login", json, "application/json");
	Json::CharReaderBuilder reader;
    std::string errs;
    Json::Value jsonData;
    std::istringstream s(res->body);
    Json::parseFromStream(reader, s, &jsonData, &errs);

	if (jsonData.get("output", "NONE").asString() != "NONE") {
		login_status = LoginFailed;
		return;
	}
	strncpy(c_pass, jsonData["auth"].asCString(), 64);
	login_status = LoginSuccess;
}

void login(char name[21], char pass[65]) {
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
Json::Value get_msg(std::string name, std::string token) {
	std::string json = "{\"name\":\"" + name + "\",\"token\":\"" + token + "\"}";
	static httplib::Client client("http://localhost:8000");
	httplib::Result res = client.Post("/", json, "application/json");
	Json::CharReaderBuilder reader;
    std::string errs;
    Json::Value jsonData;
    std::istringstream s(res->body);
    Json::parseFromStream(reader, s, &jsonData, &errs);

	return jsonData;
}

void chatPage(std::string name, std::string pass, char chatInput[100]) {
	if (login_status == LoginFailed) {
		ImGui::BeginTable("table1", 3);
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TableNextColumn();
		ImGui::Text("Please Retry login by restarting app");
		ImGui::EndTable();
		return;
	}
	Json::Value msg = get_msg(name, pass);
	if (msg.get("output", "NONE").asString() != "NONE" && msg["output"] == "USER_404") {
		ImGui::BeginTable("table1", 3);
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TableNextColumn();
		ImGui::Text("User does not exist or Invalid token, Try re-logging in again");
		ImGui::EndTable();
		return;
	}
	ImGui::PushItemWidth(-1);
	ImGui::InputText("chat-input", chatInput, 100);
	ImGui::PopItemWidth();
	ImGui::BeginTable("table1", 3);
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	if (msg.get("output", "NONE").asString() != "NONE" && msg["output"] != "USER_404")
		ImGui::Text("No messages yet");
	else
		ImGui::Text("%s: %s", msg["author"].asCString(), msg["msg"].asCString());
	ImGui::TableNextColumn();
	ImGui::EndTable();
}

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

	char chatInput[100];

	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	io.DisplaySize = ImVec2(1920, 1080);
    io.DeltaTime = 1.0f / 60.0f;

	char name[21] = "";
	char pass[65] = "dffe86797a27a6cc1e7d4f3b7628783bc1292f310eeb352148f62a993c30c027";

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// render your GUI
		ImGui::Begin("Demo window");
		if (login_status == LoginNotYet)
			login(name, pass);
		else chatPage(name, pass, chatInput);
		
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
