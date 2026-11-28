cmake_minimum_required(VERSION 3.16)

project(proyectoClase_prograIII VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

add_executable(proyectoClase_prograIII
    main.cpp
    VentanaJuego.h
    VentanaJuego.cpp
    VistaJuego.h
    VistaJuego.cpp
    MapaTemporal.h
    MapaTemporal.cpp
    Tipos.h
    recursos.qrc
    Jugador.h

    ConfigNivel.h
    JuegoEngine.cpp
    JuegoEngine.h
    Jugador.cpp
    ColaBombas.h
    ColaBombas.cpp
)

target_link_libraries(proyectoClase_prograIII PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
)

set_target_properties(proyectoClase_prograIII PROPERTIES
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS proyectoClase_prograIII
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
