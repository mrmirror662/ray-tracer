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
    includedirs {"raytracer/source/includes","raytracer/source/imgui"  }
    links{"raytracer","GL","glfw","GLEW","GLU","pthread"}
    include "raytracer/"
    filter "configurations:debug"
        defines{"DEBUG"}
        symbols "On"

    filter "configurations:release"
        defines{"NDEBUG"}
        optimize "On"
