workspace "raytracer"
    configurations {"debug","release"}
    dofileopt "raytracer/premake5.lua"

-- dependencies 
project "app"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    targetdir ""
    files {"app.cpp"}
    raytracer_dir = "raytracer/"
    includedirs {"%{raytracer_dir}source/includes","raytracer/source/vendor/imgui","%{raytracer_dir}source/vendor/stb_image/","%{raytracer_dir}source/vendor/obj_loader/"}
    links{"raytracer","GL","glfw","GLEW","GLU","pthread"}
    include "raytracer/"
    filter "configurations:debug"
        defines{"DEBUG"}
        symbols "On"

    filter "configurations:release"
        defines{"NDEBUG"}
        optimize "On"
