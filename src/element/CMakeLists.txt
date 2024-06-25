set(OSFF_SUB_MODULE element)
project(osff_${OSFF_SUB_MODULE})

set(${OSFF_SUB_MODULE}_HEADER_FILES
    audio_resampler.h
    clock.h
    format_info.h
    play_frame.h
    stream_info.h
    transformer.h
    video_scaler.h
    )

set(${OSFF_SUB_MODULE}_SOURCE_FILES
    audio_resampler.cpp
    video_scaler.cpp
    )

add_library(${PROJECT_NAME} STATIC
    ${${OSFF_SUB_MODULE}_HEADER_FILES}
     ${${OSFF_SUB_MODULE}_SOURCE_FILES}
     )
add_library(osff::${OSFF_SUB_MODULE} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../util
    )
target_link_libraries(${PROJECT_NAME} PRIVATE osff::util)

target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})

install(FILES ${${OSFF_SUB_MODULE}_HEADER_FILES}
    DESTINATION include/osff/${OSFF_SUB_MODULE}
    )