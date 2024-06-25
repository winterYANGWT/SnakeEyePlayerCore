set(OSFF_SUB_MODULE queue)
project(osff_${OSFF_SUB_MODULE})

set(${OSFF_SUB_MODULE}_HEADER_FILES
    format_packet_queue.h
    play_frame_queue.h
    stream_frame_queue.h
    stream_packet_queue.h
    )

set(${OSFF_SUB_MODULE}_SOURCE_FILES
    format_packet_queue.cpp
    play_frame_queue.cpp
    stream_frame_queue.cpp
    stream_packet_queue.cpp
    )

add_library(${PROJECT_NAME} STATIC
    ${${OSFF_SUB_MODULE}_HEADER_FILES}
    ${${OSFF_SUB_MODULE}_SOURCE_FILES}
    )
add_library(osff::${OSFF_SUB_MODULE} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../element
    ${CMAKE_CURRENT_SOURCE_DIR}/../util
    )
target_link_libraries(${PROJECT_NAME} PRIVATE
    osff::element
    osff::util
    )

target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})

install(FILES ${${OSFF_SUB_MODULE}_HEADER_FILES}
    DESTINATION include/osff/${OSFF_SUB_MODULE}
    )
add_dependencies(${PROJECT_NAME} osff_element osff_util)