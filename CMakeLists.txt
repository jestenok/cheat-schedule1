cmake_minimum_required(VERSION 3.10)
project(cheat)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

if (EXISTS "${CMAKE_SOURCE_DIR}/.env.cmake")
    include("${CMAKE_SOURCE_DIR}/.env.cmake")
else()
    message(WARNING "File .env.cmake not found, using default values.")
endif()

set(ENV{CHEAT_DLL_NAME} "${CHEAT_DLL_NAME}")

# Настройки для DLL
set(CMAKE_SHARED_LIBRARY_PREFIX "")  # Убираем префикс
set(CMAKE_SHARED_LIBRARY_SUFFIX ".dll")
set(CMAKE_BUILD_TYPE Debug)

# Проверка поддержки C++20
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++20" HAS_CPP20)
if (NOT HAS_CPP20)
    message(FATAL_ERROR "Ваш компилятор не поддерживает C++20!")
endif()

# Добавляем пути к заголовкам
include_directories(

        "${CMAKE_SOURCE_DIR}/DLL"  # Чтобы подхватывались локальные заголовки
)

# Основной исполняемый файл
add_executable(cheat
        main.cpp
        ThreadReader.cpp
        MemoryCalculator.cpp
        Injector.hpp
        ProcessWorker.hpp
        ExternalCheat.hpp
        InternalCheat.hpp
)

# DLL
add_library(${CHEAT_DLL_NAME} SHARED
        dllmain.cpp
)

# Явно указываем C++20 для DLL
target_compile_features(${CHEAT_DLL_NAME} PRIVATE cxx_std_20 cxx_lambdas)

# Подключаем MinHook
add_subdirectory(minhook)
target_link_libraries(${CHEAT_DLL_NAME} minhook -static-libstdc++ -static-libgcc)