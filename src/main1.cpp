#include "glu.h"
#include "buff_view.h"
#include <iostream>
#include "raytracer.h"
#include <unistd.h>
#include <fstream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/gtc/matrix_transform.hpp>
void ppm(frameBuff &buff)
{
    std::ofstream os;
    os.open("image.ppm");
    auto size = buff.getW() * buff.getH() * buff.getChannel();
    //std::cout << buff.data.size();
    os << "P6" << '\n'
       << buff.getW() << '\n'
       << buff.getH() << '\n'
       << 255 << '\n';
    for (int i = 0; i < buff.data.size(); i++)
        os << buff[i];
}
const int w = 1000, h = 1000;
int main()
{
    using namespace std;
    camera cam;
    cam.pos = {0, 0, 0};
    cam.w = w;
    cam.h = h;

    triangle t({10, 0, 0.f}, {-10.f, 0, -10.f}, {0, 30.f, -10.f}, {1.f, 0.f, 0.f});
    cam.resolutionFactor = 1;
    rayTracer::initRays(cam);
    rayTracer rt;
    std::vector<triangle> ts;
    ts.push_back(t);
    auto window = initGLW(w, h);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        auto buff = rt.trace(cam, ts);
        glDrawPixels(buff.getW(), buff.getH(), GL_RGB, GL_UNSIGNED_BYTE, buff.data.data());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
        ImGui::Text("Hello GUI!!\n");
        float angle = 0.1f;

        float x = 0.f;
        float y = 0.f;
        float z = 0.f;

        ImGui::SliderFloat("x", &x, -10.0f, 10.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("y", &y, -10.0f, 10.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("z", &z, -10.0f, 10.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        cam.pos = {x, y, z};
        rayTracer::updateCam(cam);
        glm::mat4 trans(1.);
        trans = glm::rotate(trans, angle, glm::vec3(0., 0., 1.));
        float dumy;
        // ts[0].v1 = glm::vec3(trans * glm::vec4(ts[0].v1, 1.0));
        // ts[0].v2 = glm::vec3(trans * glm::vec4(ts[0].v2, 1.0));
        // ts[0].v3 = glm::vec3(trans * glm::vec4(ts[0].v3, 1.0));

        ImGui::End();

        angle += 0.01;
        ImGui::Render();
        /* Swap front and back buffers */
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}