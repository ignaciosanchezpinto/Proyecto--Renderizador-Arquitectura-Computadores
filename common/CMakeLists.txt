add_library(common STATIC)

target_sources(common 
    PRIVATE 
        src/cilindro.cpp
        src/esfera.cpp
        src/material.cpp
        src/leer_escena.cpp
        src/escenario.cpp
        src/configuracion.cpp
        src/rayo.cpp
        src/camara.cpp






)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(common PUBLIC Microsoft.GSL::GSL TBB::tbb)
