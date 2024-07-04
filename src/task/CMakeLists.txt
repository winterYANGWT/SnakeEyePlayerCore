set(SNAKE_EYE_TASK_SUBMODULE "task")
project(se_${SNAKE_EYE_TASK_SUBMODULE})

set(${SNAKE_EYE_TASK_SUBMODULE}_HEADER_FILES
    base_task.hpp
    decode_task.hpp
    encode_task.hpp
    read_task.hpp
    read_transfer_task.hpp
    transform_task.hpp
    write_task.hpp
    write_transfer_task.hpp
    )

set(${SNAKE_EYE_TASK_SUBMODULE}_SOURCE_FILES
    decode_task.cpp
    encode_task.cpp
    read_task.cpp
    read_transfer_task.cpp
    transform_task.cpp
    write_task.cpp
    write_transfer_task.cpp
    )

add_library(${PROJECT_NAME} STATIC
    ${${SNAKE_EYE_TASK_SUBMODULE}_HEADER_FILES}
    ${${SNAKE_EYE_TASK_SUBMODULE}_SOURCE_FILES}
    )

add_library(se::${SNAKE_EYE_TASK_SUBMODULE} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../context
    ${CMAKE_CURRENT_SOURCE_DIR}/../static
    ${CMAKE_CURRENT_SOURCE_DIR}/../util
    )

target_link_libraries(${PROJECT_NAME} PRIVATE se::context se::static se::util)

# link third-party libraries
target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})

install(FILES ${${SNAKE_EYE_TASK_SUBMODULE}_HEADER_FILES}
    DESTINATION include/se/${SNAKE_EYE_TASK_SUBMODULE}
    )

add_dependencies(${PROJECT_NAME} se_context se_static se_util)
