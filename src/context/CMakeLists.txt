set(SNAKE_EYE_CONTEXT_SUBMODULE context)
project(se_${SNAKE_EYE_CONTEXT_SUBMODULE})

set(${SNAKE_EYE_CONTEXT_SUBMODULE}_HEADER_FILES
    format_context.hpp
    packet_transfer_context.hpp
    stream_context.hpp
    transformer_context.hpp
    )

set(${SNAKE_EYE_CONTEXT_SUBMODULE}_SOURCE_FILES
    format_context.cpp
    )

add_library(${PROJECT_NAME} STATIC
    ${${SNAKE_EYE_CONTEXT_SUBMODULE}_HEADER_FILES}
    ${${SNAKE_EYE_CONTEXT_SUBMODULE}_SOURCE_FILES}
    )

add_library(se::${SNAKE_EYE_CONTEXT_SUBMODULE} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../util
    ${CMAKE_CURRENT_SOURCE_DIR}/../transformer
    )

target_link_libraries(${PROJECT_NAME} PRIVATE se::util se::transformer)

# link third-party libraries
target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})

install (FILES ${${SNAKE_EYE_CONTEXT_SUBMODULE}_HEADER_FILES}
    DESTINATION include/se/${SNAKE_EYE_CONTEXT_SUBMODULE}
    )
