set(OSFF_SUB_MODULE "manager")
project(osff_${OSFF_SUB_MODULE})

set(${OSFF_SUB_MODULE}_HEADER_FILES
    codec_context_manager.h
    data_manager.h
    format_context_manager.h
    format_packet_transfer_manager.h
    info_manager.h
    play_manager.h
    transformer_manager.h)

set(${OSFF_SUB_MODULE}_SOURCE_FILES
    codec_context_manager.cpp
    data_manager.cpp
    format_context_manager.cpp
    format_packet_transfer_manager.cpp
    play_manager.cpp
    transformer_manager.cpp)

add_library(${PROJECT_NAME} STATIC
    ${${OSFF_SUB_MODULE}_HEADER_FILES}
    ${${OSFF_SUB_MODULE}_SOURCE_FILES})
add_library(osff::${OSFF_SUB_MODULE} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../element
    ${CMAKE_CURRENT_SOURCE_DIR}/../queue
    ${CMAKE_CURRENT_SOURCE_DIR}/../thread
    ${CMAKE_CURRENT_SOURCE_DIR}/../util)
target_link_libraries(${PROJECT_NAME} PRIVATE
    osff::element
    osff::queue
    osff::thread
    osff::util)

target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})

install (FILES ${${OSFF_SUB_MODULE}_HEADER_FILES}
    DESTINATION include/osff/${OSFF_SUB_MODULE})
add_dependencies(${PROJECT_NAME} 
    osff_element 
    osff_queue 
    osff_thread 
    osff_util)