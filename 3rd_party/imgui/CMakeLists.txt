project(imgui)

if(TARGET ${PROJECT_NAME})
    return()
endif()

add_library(${PROJECT_NAME}
    STATIC
    imgui/imgui.h
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_opengl3.h
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_sdl.h
    imgui/backends/imgui_impl_sdl.cpp)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD2
    PUBLIC IMGUI_DEFINE_MATH_OPERATORS)

target_include_directories(${PROJECT_NAME}
    PUBLIC imgui .)

target_link_libraries(${PROJECT_NAME}
    PUBLIC sdl2 glad2-loader)
