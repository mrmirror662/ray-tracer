workspace "raytracer"
    configurations {"debug","release"}

    
project "raytracer"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "lib/%{cfg.buildcfg}"
    files {"source/src/**.cpp","source/imgui/**.cpp"}
    includedirs {"source/includes/","source/imgui/"}
    links {"glfw","GLU","GL","GLEW"}
    
    filter "configurations:debug"
        defines{"DEBUG"}
        symbols "On"

    filter "configurations:release"
        defines{"NDEBUG"}
        optimize "On"
    