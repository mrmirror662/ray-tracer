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
const int w = 500, h = 500;
const int windw = 500, windh = 500;
int main()
{
    using namespace std;
    camera cam;
    cam.pos = {-1000, -1000, 0};
    cam.w = w;
    cam.h = h;

    triangle t();
    cam.resolutionFactor = 1;
    rayTracer::initRays(cam);
    rayTracer rt;
    mesh deer;
    deer.loadFromObj("asset/deer.obj");
    glm::mat4 trans(1.0f);

    trans = glm::rotate(trans, 3.1415f, glm::vec3(0.f, 0.f, 0.f));
    for (auto &t : deer.tris)
    {
        // t.v1 = trans * glm::vec4(t.v1, 1.f);
        // t.v2 = trans * glm::vec4(t.v2, 1.f);
        // t.v3 = trans * glm::vec4(t.v3, 1.f);
    }
    auto window = initGLW(windw, windh);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    int count = 0;
    bool hd = false;

    while (!glfwWindowShouldClose(window))
    {
        std::cout << "count:" << count << '\n';
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        frameBuff buff;
        buff = rt.trace(cam, deer.tris);

        glPixelZoom(windw / cam.w, windh / cam.h);
        glDrawPixels(buff.getW(), buff.getH(), GL_RGB, GL_UNSIGNED_BYTE, (void *)buff.data.data());
        // glDrawPixels(buff.getW(), buff.getH(), GL_RGB, GL_UNSIGNED_BYTE, buff.data.data());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
        ImGui::Text("Hello GUI!!\n");
        float x = 0.f;
        float y = 0.f;
        float z = 0.f;
        ImGui::SliderFloat("x", &x, -10.0f, 10.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("y", &y, -10.0f, 10.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("z", &z, -10.0f, 10.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::Checkbox("high def:", &hd);
        cam.pos = {x, y, z};
        rayTracer::updateCam(cam);

        if (hd)
        {
            cam.h = 200;
            cam.w = 200;
            cam.c = 3;
            hd = false;
            rayTracer::initRays(cam);
        }
        ImGui::End();
        ImGui::Render();
        /* Swap front and back buffers */
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
        count++;
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}