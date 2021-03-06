############################################################
# CMake Build Script for the rendering library

cmake_minimum_required(VERSION 2.8.7)
project(${CMAKE_PROJECT_NAME}_rendering)

set(PROJECT_INCLUDE_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/*.c*)
file(GLOB_RECURSE PROJECT_INCLUDES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_INCLUDE_DIR}/*.*h)

file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/*.c*)
file(GLOB_RECURSE PROJECT_INCLUDES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_INCLUDE_DIR}/rtr/ren/*.*h)

SOURCE_GROUP("rendering" FILES ${PROJECT_SOURCES})
SOURCE_GROUP("rendering" FILES ${PROJECT_INCLUDES})

############################################################
# Shaders

file(GLOB SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vs
                  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.gs
                  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.fs)

SOURCE_GROUP("shaders" FILES ${SHADERS})

############################################################
# Build library

include_directories(${PROJECT_INCLUDE_DIR} 
                    ${COMMON_INCLUDE_DIR} 
                    ${LAMURE_CONFIG_DIR}
                    ${REND_INCLUDE_DIR})

include_directories(SYSTEM
                          ${Boost_INCLUDE_DIR}
                          ${GLFW_INCLUDE_DIRS}
                          ${SDL2_INCLUDE_DIR}
                          ${ASSIMP_INCLUDE_DIR}
                          ${IL_INCLUDE_DIR}
                          ${GLEW_INCLUDE_DIRS}
                           )

link_directories(${SCHISM_LIBRARY_DIRS} ${Boost_LIBRARY_DIR})

add_library(${PROJECT_NAME} SHARED ${PROJECT_INCLUDES} ${PROJECT_SOURCES} ${SHADERS})

IF (MSVC)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-D RTR_RENDERING_LIBRARY")
ENDIF(MSVC)

set(REND_INCLUDE_DIR ${PROJECT_INCLUDE_DIR} PARENT_SCOPE)
set(REND_LIBRARY ${PROJECT_NAME} PARENT_SCOPE)
set(REND_LIBRARY ${PROJECT_NAME})


############################################################
# Libraries

target_link_libraries(${PROJECT_NAME}
    ${PROJECT_LIBS}
    ${SDL2_LIBRARY}
    ${IL_LIBRARIES}
    ${ILU_LIBRARIES}
    ${ILUT_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${ASSIMP_LIBRARY}
    ${GLEW_LIBRARY}
    ${COMMON_LIBRARY}
#    ${SCHISM_LIBRARIES}
#    optimized ${Boost_THREAD_LIBRARY_RELEASE} debug ${Boost_THREAD_LIBRARY_DEBUG}
#    optimized ${Boost_DATE_TIME_LIBRARY_RELEASE} debug ${Boost_DATE_TIME_LIBRARY_DEBUG}
#    optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE} debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG}
#    ${FREEIMAGE_LIBRARY}
    boost_filesystem
    boost_system
    )

###############################################################################
# install 
###############################################################################

IF (MSVC)
  install (TARGETS ${PROJECT_NAME}
    CONFIGURATIONS Release
    RUNTIME DESTINATION bin/Release
    LIBRARY DESTINATION lib/Release
    ARCHIVE DESTINATION lib/Release
  )

  install (TARGETS ${PROJECT_NAME}
    CONFIGURATIONS Debug
    RUNTIME DESTINATION bin/Debug
    LIBRARY DESTINATION lib/Debug
    ARCHIVE DESTINATION lib/Debug
  )
ELSEIF (UNIX)
  install (TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
  )
ENDIF (MSVC)

# header files 
install(DIRECTORY ${PROJECT_INCLUDE_DIR}/rtr/ren DESTINATION include/rtr FILES_MATCHING PATTERN "*.*h")
install(DIRECTORY ${PROJECT_INCLUDE_DIR}/rtr/ren DESTINATION include/rtr/transformation FILES_MATCHING PATTERN "*.*h")
install(DIRECTORY ${PROJECT_INCLUDE_DIR}/rtr/ren DESTINATION include/rtr/animation FILES_MATCHING PATTERN "*.*h")
install(DIRECTORY ${PROJECT_INCLUDE_DIR}/rtr/imgui DESTINATION include/rtr FILES_MATCHING PATTERN "*.*h")

