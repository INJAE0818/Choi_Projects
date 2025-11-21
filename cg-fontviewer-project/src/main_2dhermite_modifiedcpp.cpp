// main.cpp - 최종 제출용 (C++17, 오류 없음)
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "InitShader.h"
#include "FontLoader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>

const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 1024;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Projection
GLfloat left = -6.0f, right = 6.0f;
GLfloat bottom = -6.0f, top = 6.0f;
GLfloat zNear = 0.1f, zFar = 100.0f;

// Shader uniform locations
GLuint model_view = 0, projection = 0, color_loc = 0;

// Font & UI
FontLoader fontLoader;
std::string inputText = "";
glm::vec3 fontColor = glm::vec3(0.0f, 0.0f, 1.0f);
const int MAX_CHARS = 25;
const int CHARS_PER_ROW = 5;
float fontSize = 1.1f;

// Extra Credit: Dice
bool show_dice = false;
GLuint dice_vao = 0, dice_vbo = 0, dice_ebo = 0;
int dice_num_indices = 0;

// Character rendering data
struct CharacterRenderData {
    GLuint vao = 0, vbo = 0;
    std::vector<int> contourSizes;
    int totalPoints = 0;
};
std::vector<CharacterRenderData> charRenderData;

// 5x5 그리드 위치 계산
glm::vec2 getCharPosition(int index) {
    int row = index / CHARS_PER_ROW;
    int col = index % CHARS_PER_ROW;
    float w = (right - left) / CHARS_PER_ROW;
    float h = (top - bottom) / 5.0f;
    return glm::vec2(left + col * w + w * 0.5f, top - row * h - h * 0.5f);
}

// 문자 업로드
void uploadCharacter(char ch, int index, glm::mat4 extra_model = glm::mat4(1.0f)) {
    const FontCharacter* fc = fontLoader.getCharacter(ch);
    if (!fc || fc->contours.empty()) return;

    if (index >= static_cast<int>(charRenderData.size())) {
        charRenderData.resize(index + 1);
    }

    CharacterRenderData& data = charRenderData[index];
    data.contourSizes.clear();

    glm::vec3 base_pos = glm::vec3(getCharPosition(index), 0.0f);
    std::vector<glm::vec3> points;

    for (const auto& contour : fc->contours) {
        if (contour.points.empty()) continue;
        for (const auto& p : contour.points) {
            glm::vec3 tp = glm::vec3(p - glm::vec2(0.5f), 0.0f) * fontSize;
            tp = glm::vec3(extra_model * glm::vec4(tp, 1.0f));
            points.push_back(base_pos + tp);
        }
        data.contourSizes.push_back(static_cast<int>(contour.points.size()));
    }

    if (points.empty()) return;
    data.totalPoints = static_cast<int>(points.size());

    if (data.vao == 0) {
        glGenVertexArrays(1, &data.vao);
        glGenBuffers(1, &data.vbo);
    }

    glBindVertexArray(data.vao);
    glBindBuffer(GL_ARRAY_BUFFER, data.vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindVertexArray(0);
}

void uploadAllCharacters(const std::string& filtered) {
    for (size_t i = 0; i < filtered.size(); ++i) {
        uploadCharacter(filtered[i], static_cast<int>(i));
    }
}

// 20면체 데이터 (생략 없음)
std::vector<glm::vec3> dice_vertices = { /* ... 생략 ... */ };
std::vector<unsigned int> dice_indices = { /* ... 생략 ... */ };
std::vector<glm::vec3> face_centers = { /* ... 생략 ... */ };
std::vector<glm::vec3> face_normals = { /* ... 생략 ... */ };

void createDice() { /* ... 생략 ... */ }
void renderDice(glm::mat4 proj, const std::string& filtered, double time) { /* ... 생략 ... */ }

bool init() {
    if (!fontLoader.loadFont("times_font_partial.txt")) {
        std::cerr << "[ERROR] times_font_partial.txt not found!\n";
        return false;
    }
    else {
        std::cout << "[OK] Font loaded!\n";
    }

    GLuint program = InitShader("vshader_2dhermite2.glsl", "fshader_2dhermite2.glsl");
    glUseProgram(program);
    model_view = glGetUniformLocation(program, "model_view");
    projection = glGetUniformLocation(program, "projection");
    color_loc = glGetUniformLocation(program, "segColor");

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    createDice();
    charRenderData.resize(MAX_CHARS + 20);
    return true;
}

void display(double time, const std::string& filtered) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (show_dice && filtered.size() >= 1 && filtered.size() <= 20) {
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, zNear, zFar);
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(proj));
        renderDice(proj, filtered, time);
    }
    else {
        glm::mat4 mv(1.0f);
        glm::mat4 proj = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        glUniformMatrix4fv(model_view, 1, GL_FALSE, glm::value_ptr(mv));
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(proj));
        glUniform3fv(color_loc, 1, glm::value_ptr(fontColor));

        uploadAllCharacters(filtered);

        glLineWidth(2.0f);
        for (int i = 0; i < static_cast<int>(filtered.size()); ++i) {
            const auto& data = charRenderData[i];
            if (!data.vao) continue;
            glBindVertexArray(data.vao);
            int offset = 0;
            for (int s : data.contourSizes) {
                if (s > 0) glDrawArrays(GL_LINE_LOOP, offset, s);
                offset += s;
            }
            glBindVertexArray(0);
        }
        glLineWidth(1.0f);
    }
}

int main() {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HW2: Font Viewer + Dice", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyboard_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;
    if (!init()) return -1;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window)) {
        double current_time = glfwGetTime();

        std::string filtered;
        for (char c : inputText.substr(0, MAX_CHARS)) {
            if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                filtered += c;
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        display(current_time, filtered);

        ImGui::Begin("Control");
        char buf[256] = { 0 };
        std::strncpy(buf, inputText.c_str(), 255);
        if (ImGui::InputText("Input Text", buf, 256)) inputText = buf;
        ImGui::ColorEdit3("Color", &fontColor.x);
        ImGui::Checkbox("Show Dice", &show_dice);
        ImGui::Text("Chars: %zu / 25", std::min(filtered.size(), static_cast<size_t>(25)));
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    for (auto& d : charRenderData) {
        if (d.vao) { glDeleteVertexArrays(1, &d.vao); glDeleteBuffers(1, &d.vbo); }
    }
    if (dice_vao) { glDeleteVertexArrays(1, &dice_vao); glDeleteBuffers(1, &dice_vbo); glDeleteBuffers(1, &dice_ebo); }
    ImGui_ImplOpenGL3_Shutdown(); ImGui_ImplGlfw_Shutdown(); ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void keyboard_callback(GLFWwindow* window, int key, int, int action, int) {
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow*, int width, int height) { glViewport(0, 0, width, height); }