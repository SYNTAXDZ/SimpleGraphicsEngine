add_library( glad STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c" )
target_include_directories(glad
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  )
