set(COMMON_SRC_FILES 
  "${CMAKE_SOURCE_DIR}/common/src/cilindro.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/esfera.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/material.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/leer_escena.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/configuracion.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/camara.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/rayo.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/escenario.cpp"


)

set(CURRENT_DIR_SRC_FILES 
  "${CMAKE_CURRENT_SOURCE_DIR}/test_vector.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_cilindro.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_esfera.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_materiales.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_leerescena.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_conf.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_camara.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_rayo.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_escenario.cpp"





)

add_unit_test_target(
  TARGET_NAME utcommon
  SOURCE_FILES ${COMMON_SRC_FILES} ${CURRENT_DIR_SRC_FILES}
  LIBRARY_FILTER common
  COVERAGE_DIR coverage-common
  LIBRARY_TO_LINK common
)
