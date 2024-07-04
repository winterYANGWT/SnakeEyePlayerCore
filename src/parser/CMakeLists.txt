# CMakeLists.txt of se::parser
set(SNAKE_EYE_PARSER_SUBMODULE parser)
project(se_${SNAKE_EYE_PARSER_SUBMODULE})

# set SOURCE_FILES
set(${SNAKE_EYE_PARSER_SUBMODULE}_HEADER_FILES
    codec_parser.hpp
    )

set(${SNAKE_EYE_PARSER_SUBMODULE}_SOURCE_FILES
    codec_parser.cpp
    )

# Add the library
add_library(${PROJECT_NAME} STATIC
    ${${SNAKE_EYE_PARSER_SUBMODULE}_HEADER_FILES}
    ${${SNAKE_EYE_PARSER_SUBMODULE}_SOURCE_FILES}
    )

add_library(se::${SNAKE_EYE_PARSER_SUBMODULE} ALIAS ${PROJECT_NAME})

# target libraries
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../util)
target_link_libraries(${PROJECT_NAME} PRIVATE se::util)

# target third-party libraries
target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE nlohmann_json)

install (FILES ${${SNAKE_EYE_PARSER_SUBMODULE}_HEADER_FILES}
    DESTINATION include/se/${SNAKE_EYE_PARSER_SUBMODULE})

add_dependencies(${PROJECT_NAME} se_util)
