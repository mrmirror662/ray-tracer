workspace "raytracer"
    configurations {"debug","release"}
project "app"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    targetdir ""
    files {"app.cpp"}
    includedirs {"ray-tracer/source/includes/","ray-tracer/source/imgui/"}
    libdirs {"ray-tracer/lib/%{cfg.buildcfg}"}
    links{"raytracer","glfw","GLU","GL","GLEW"}
    filter "configurations:debug"
        defines{"DEBUG"}
        symbols "On"

    filter "configurations:release"
        defines{"NDEBUG"}
        optimize "On"
