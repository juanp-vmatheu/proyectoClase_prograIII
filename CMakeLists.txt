cmake_minimum_required(VERSION 3.16)

project(proyectoClase_prograIII LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(proyectoClase_prograIII main.cpp)

include(GNUInstallDirs)
install(TARGETS proyectoClase_prograIII
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
