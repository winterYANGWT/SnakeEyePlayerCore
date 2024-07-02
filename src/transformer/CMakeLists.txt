set(SNAKE_EYE_TRANSFORMER_SUBMODULE transformer)
project(se_${SNAKE_EYE_TRANSFORMER_SUBMODULE})

set(${SNAKE_EYE_TRANSFORMER_SUBMODULE}_HEADER_FILES
    audio_resampler.hpp
    transformer.hpp
    video_scaler.hpp
    )

set(${SNAKE_EYE_TRANSFORMER_SUBMODULE}_SOURCE_FILES
    audio_resampler.cpp
    video_scaler.cpp
    )

add_library(${PROJECT_NAME} STATIC
    ${${SNAKE_EYE_TRANSFORMER_SUBMODULE}_HEADER_FILES}
    ${${SNAKE_EYE_TRANSFORMER_SUBMODULE}_SOURCE_FILES}
    )

add_library(se::${SNAKE_EYE_TRANSFORMER_SUBMODULE} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../static
    ${CMAKE_CURRENT_SOURCE_DIR}/../util
    )
target_link_libraries(${PROJECT_NAME} PRIVATE
    se::static
    se::util
    )

#link third-party libraries
target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})


install (FILES ${${SNAKE_EYE_TRANSFORMER_SUBMODULE}_HEADER_FILES}
    DESTINATION include/se/${SNAKE_EYE_TRANSFORMER_SUBMODULE}
    )
