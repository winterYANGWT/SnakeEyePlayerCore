set(SNAKE_EYE_SUBMODULE queue)
project(se_${SNAKE_EYE_SUBMODULE})

set(${SNAKE_EYE_SUBMODULE}_HEADER_FILES
    format_packet_queue.hpp
    play_frame_queue.hpp
    stream_frame_queue.h
    stream_packet_queue.h
    )

set(${SNAKE_EYE_SUBMODULE}_SOURCE_FILES
    format_packet_queue.cpp
    play_frame_queue.cpp
    stream_frame_queue.cpp
    stream_packet_queue.cpp
    )

add_library(${PROJECT_NAME} STATIC
    ${${SNAKE_EYE_SUBMODULE}_HEADER_FILES}
    ${${SNAKE_EYE_SUBMODULE}_SOURCE_FILES}
    )
add_library(se::${SNAKE_EYE_SUBMODULE} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../element
    ${CMAKE_CURRENT_SOURCE_DIR}/../util
    )
target_link_libraries(${PROJECT_NAME} PRIVATE
    se::element
    se::util
    )

target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})

install(FILES ${${SNAKE_EYE_SUBMODULE}_HEADER_FILES}
    DESTINATION include/se/${SNAKE_EYE_SUBMODULE}
    )
add_dependencies(${PROJECT_NAME} se_element se_util)
