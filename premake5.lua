workspace "raytracer"
    configurations {"debug","release"}

    
project "raytracer"
    kind "WindowedApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    files {"src/**.cpp","imgui/**.cpp"}
    includedirs {"includes/","imgui/"}
    links {"glfw","GLU","GL","GLEW"}
    
    filter "configurations:debug"
        defines{"DEBUG"}
        symbols "On"

    filter "configurations:release"
        defines{"NDEBUG"}
        optimize "On"
    