# CMakeLists.txt of osff::parser
set(OSFF_SUB_MODULE parser)
project(osff_${OSFF_SUB_MODULE})

# set SOURCE_FILES
set(${OSFF_SUB_MODULE}_HEADER_FILES
    codec_parser.h
    )

set(${OSFF_SUB_MODULE}_SOURCE_FILES
    codec_parser.cpp
    )

# Add the library
add_library(${PROJECT_NAME} STATIC
    ${${OSFF_SUB_MODULE}_HEADER_FILES}
    ${${OSFF_SUB_MODULE}_SOURCE_FILES}
    )

add_library(osff::${OSFF_SUB_MODULE} ALIAS ${PROJECT_NAME})

# target libraries
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../util)
target_link_libraries(${PROJECT_NAME} PRIVATE osff::util)

# target third-party libraries
target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE nlohmann_json)

install (FILES ${${OSFF_SUB_MODULE}_HEADER_FILES}
    DESTINATION include/osff/${OSFF_SUB_MODULE})

add_dependencies(${PROJECT_NAME} osff_util)

