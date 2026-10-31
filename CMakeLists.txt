cmake_minimum_required(VERSION 3.21)

project(Semantico LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(Semantico WIN32
    src/main.cpp
    src/AnalizadorSemantico.cpp
    src/Formula.cpp
    src/LienzoArbol.cpp
    src/MotorArbol.cpp
    src/NodoArbol.cpp
    src/RegistroAtomos.cpp
    src/VentanaPrincipal.cpp
    include/AnalizadorSemantico.hpp
    include/Atomo.hpp
    include/Formula.hpp
    include/LienzoArbol.hpp
    include/MotorArbol.hpp
    include/NodoArbol.hpp
    include/RegistroAtomos.hpp
    include/VentanaPrincipal.hpp
)

target_include_directories(Semantico PRIVATE include)
target_link_libraries(Semantico PRIVATE Qt6::Widgets)
