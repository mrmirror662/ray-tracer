workspace "raytracer"
    configurations {"debug","release"}

    
project "raytracer"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "lib/%{cfg.buildcfg}"
    files {"source/src/**.cpp","source/vendor/imgui/**.cpp"}
    includedirs {"source/includes/","source/vendor/imgui/","source/vendor/stb_image/","source/vendor/obj_loader/"}
    links {"glfw","GLU","GL","GLEW","pthread"}
    filter "configurations:debug"
        defines{"DEBUG"}
        linkoptions "-g"
        symbols "On"

    filter "configurations:release"
        defines{"NDEBUG"}
        optimize "On"
    