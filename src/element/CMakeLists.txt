set(SNAKE_EYE_ELEMENT_SUBMODULE element)
project(se_${SNAKE_EYE_ELEMENT_SUBMODULE})

set(${SNAKE_EYE_ELEMENT_SUBMODULE}_HEADER_FILES
    clock.hpp
    format_info.hpp
    play_frame.hpp
    stream_info.hpp
    )


add_library(${PROJECT_NAME} INTERFACE
    ${${SNAKE_EYE_ELEMENT_SUBMODULE}_HEADER_FILES}
     )

add_library(se::${SNAKE_EYE_ELEMENT_SUBMODULE} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../static
    )

target_include_directories(${PROJECT_NAME} INTERFACE ${FFMPEG_INCLUDE})

install(FILES ${${SNAKE_EYE_ELEMENT_SUBMODULE}_HEADER_FILES}
    DESTINATION include/se/${SNAKE_EYE_ELEMENT_SUBMODULE}
    )

add_dependencies(${PROJECT_NAME} se_static)
