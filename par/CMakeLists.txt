add_executable(render-par
    src/main.cpp
    src/imagen.cpp   
    src/renderizador.cpp 
)

target_include_directories(render-par PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include    
    ${CMAKE_SOURCE_DIR}/common/include     
)

target_link_libraries(render-par PRIVATE common Microsoft.GSL::GSL TBB::tbb)
