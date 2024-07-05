set(SNAKE_EYE_QUEUE_SUBMODULE queue)
project(se_${SNAKE_EYE_QUEUE_SUBMODULE})

set(${SNAKE_EYE_QUEUE_SUBMODULE}_HEADER_FILES
    data_queue.hpp
    format_packet_queue.hpp
    play_frame_queue.hpp
    stream_frame_queue.hpp
    stream_packet_queue.hpp
    )

set(${SNAKE_EYE_QUEUE_SUBMODULE}_SOURCE_FILES
    data_queue.cpp
    format_packet_queue.cpp
    play_frame_queue.cpp
    stream_frame_queue.cpp
    stream_packet_queue.cpp
    )

add_library(${PROJECT_NAME} STATIC
    ${${SNAKE_EYE_QUEUE_SUBMODULE}_HEADER_FILES}
    ${${SNAKE_EYE_QUEUE_SUBMODULE}_SOURCE_FILES}
    )
add_library(se::${SNAKE_EYE_QUEUE_SUBMODULE} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../element
    ${CMAKE_CURRENT_SOURCE_DIR}/../static
    )
target_link_libraries(${PROJECT_NAME} PRIVATE
    se::element
    se::static
    )

target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})

install(FILES ${${SNAKE_EYE_QUEUE_SUBMODULE}_HEADER_FILES}
    DESTINATION include/se/${SNAKE_EYE_QUEUE_SUBMODULE}
    )
add_dependencies(${PROJECT_NAME} se_element se_static)
