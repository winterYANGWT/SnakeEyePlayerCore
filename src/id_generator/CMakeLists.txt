set(SNAKE_EYE_ID_GENERATOR_SUBMODULE id_generator)
project(se_${SNAKE_EYE_ID_GENERATOR_SUBMODULE})

set(${SNAKE_EYE_ID_GENERATOR_SUBMODULE}_HEADER_FILES
    data_queue_id_generator.hpp
    format_id_generator.hpp
    stream_id_generator.hpp
    )

add_library(${PROJECT_NAME} INTERFACE
    ${${SNAKE_EYE_ID_GENERATOR_SUBMODULE}_HEADER_FILES}
    )

add_library(se::${SNAKE_EYE_ID_GENERATOR_SUBMODULE} ALIAS ${PROJECT_NAME})

# link third-party libraries
target_include_directories(${PROJECT_NAME} INTERFACE ${FFMPEG_INCLUDE})

install(FILES ${${SNAKE_EYE_ID_GENERATOR_SUBMODULE}_HEADER_FILES}
    DESTINATION include/se/${SNAKE_EYE_ID_GENERATOR_SUBMODULE}
    )
