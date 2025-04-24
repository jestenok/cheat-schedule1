cmake_minimum_required(VERSION 3.10)
project(cheat)

# --------------------------------------------------------------------------
# 0.  Общие настройки компилятора
# --------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Собираем все exe/DLL рядом с исходниками
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# --------------------------------------------------------------------------
# 1.  Подхватываем переменные окружения из .env.cmake (если есть)
# --------------------------------------------------------------------------
if (EXISTS "${CMAKE_SOURCE_DIR}/.env.cmake")
    include("${CMAKE_SOURCE_DIR}/.env.cmake")
else()
    message(WARNING "Файл .env.cmake не найден, использую значения по умолчанию.")
endif()

# Имя DLL из переменной CHEAT_DLL_NAME
set(ENV{CHEAT_DLL_NAME} "${CHEAT_DLL_NAME}")

# --------------------------------------------------------------------------
# 2.  Параметры сборки DLL
# --------------------------------------------------------------------------
set(CMAKE_SHARED_LIBRARY_PREFIX "")   # убираем префикс lib
set(CMAKE_SHARED_LIBRARY_SUFFIX ".dll")
set(CMAKE_BUILD_TYPE Debug)

# Проверяем поддержку C++20
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++20" HAS_CPP20)
if (NOT HAS_CPP20)
    message(FATAL_ERROR "Ваш компилятор не поддерживает C++20!")
endif()

# --------------------------------------------------------------------------
# 3.  Пути к заголовкам проекта
# --------------------------------------------------------------------------
include_directories("${CMAKE_SOURCE_DIR}/DLL")

# --------------------------------------------------------------------------
# 4.  Основной исполняемый файл
# --------------------------------------------------------------------------
add_executable(cheat
        main.cpp
        ThreadReader.cpp
        MemoryCalculator.cpp
        Injector.hpp
        ProcessWorker.hpp
        ExternalCheat.hpp
        InternalCheat.hpp
)

# --------------------------------------------------------------------------
# 5.  DLL (CHEAT_DLL_NAME приходит из .env.cmake)
# --------------------------------------------------------------------------
add_library(${CHEAT_DLL_NAME} SHARED dllmain.cpp)
target_compile_features(${CHEAT_DLL_NAME} PRIVATE cxx_std_20)

# --------------------------------------------------------------------------
# 6.  MinHook
# --------------------------------------------------------------------------
add_subdirectory(minhook)

# --------------------------------------------------------------------------
# 7.  GLFW (втягиваем исходники как подпроект, без INSTALL/тестов)
# --------------------------------------------------------------------------
add_subdirectory(extern/glfw EXCLUDE_FROM_ALL)
set_target_properties(glfw PROPERTIES FOLDER "third_party")

# --------------------------------------------------------------------------
# 8.  Dear ImGui
# --------------------------------------------------------------------------
file(GLOB IMGUI_CORE
        extern/imgui/imgui*.cpp
)
set(IMGUI_BACKEND
        extern/imgui/backends/imgui_impl_glfw.cpp
        extern/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_CORE} ${IMGUI_BACKEND})
target_include_directories(imgui PUBLIC
        extern/imgui
        extern/imgui/backends
)
find_package(OpenGL REQUIRED)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

# --------------------------------------------------------------------------
# 9.  Линковка библиотек
# --------------------------------------------------------------------------
target_link_libraries(${CHEAT_DLL_NAME}
        PRIVATE
        imgui
        minhook
        -static-libstdc++ -static-libgcc
)

target_link_libraries(cheat PRIVATE imgui)

# --------------------------------------------------------------------------
# 10. Сообщения для пользователя
# --------------------------------------------------------------------------
message(STATUS "Сборка DLL: ${CHEAT_DLL_NAME}")
message(STATUS "ImGui       : подключён статически")
message(STATUS "GLFW        : собран как подпроект extern/glfw")
message(STATUS "MinHook     : подключён через add_subdirectory(minhook)")
