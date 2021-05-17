#define STB_IMAGE_IMPLEMENTATION

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
#include <chrono>
#include <mutex>
#include "skybox.h"
#define STBI_IMPLEMENTATION
class Timer
{
private:
    std::chrono::_V2::system_clock::time_point m_StartTime;

public:
    void Start() { m_StartTime = std::chrono::high_resolution_clock::now(); };
    float GetDuration()
    {
        std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - m_StartTime;
        return duration.count();
    }
};
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
const int w = 400, h = 400;
const int windw = 400, windh = 400;
int main()
{
    using namespace std;
    camera cam;
    cam.pos = {-1000, 300, 0};
    cam.w = w;
    cam.h = h;

    triangle t();
    cam.resolutionFactor = 1;
    rayTracer::initAll(cam);
    rayTracer rt;
    mesh deer1;
    mesh deer2;
    mesh deer3;

    deer1.loadFromObj("asset/deer.obj", {1.f, 0.46f, 0.43f});
    deer2.loadFromObj("asset/deer.obj", {1.f, 1.f, 1.f});
    deer3.loadFromObj("asset/deer.obj", {1.f, 1.f, 1.f});

    deer1.absorption = 0.7f;
    deer2.absorption = 0.3f;
    deer3.absorption = 0.7f;

    for (auto &t : deer2.tris)
    {
        glm::vec3 offset = {0.f, 0.f, 750.f};
        t.v1 += offset;
        t.v2 += offset;
        t.v3 += offset;
    }
    for (auto &t : deer3.tris)
    {
        glm::vec3 offset = {0.f, 0.f, 750.f};
        t.v1 -= offset;
        t.v2 -= offset;
        t.v3 -= offset;
    }
    auto window = initGLW(windw, windh);
    glfwSwapInterval(1);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    int count = 0;
    bool hd = false;
    std::mutex m;
    std::vector<mesh> meshes;
    meshes.push_back(deer1);
    meshes.push_back(deer2);
    meshes.push_back(deer3);
    SkyBox skybox("asset/sb2.jpg");
    frameBuff buff(cam.w * cam.resolutionFactor, cam.h * cam.resolutionFactor, cam.c);
    int buffWidth = buff.getW();
    int buffHeight = buff.getH();

    std::thread rt_thread([](camera *cam, std::vector<mesh> *meshes, SkyBox *sb, frameBuff *buff) {
        auto threads = rayTracer::trace_tris(cam, meshes, sb, buff);
        for (int i = 0; i < threads.size(); i++)
        {
            threads[i].join();
        }
    },
                          &cam, &meshes, &skybox, &buff);
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

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
    rt_thread.join();

    return 0;
}