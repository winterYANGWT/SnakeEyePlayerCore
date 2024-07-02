set(SNAKE_EYE_UTIL_SUBMODULE util)
project(se_${SNAKE_EYE_UTIL_SUBMODULE})

set(${SNAKE_EYE_UTIL_SUBMODULE}_HEADER_FILES
    read_dir.hpp
    util.hpp
    utils.hpp
    )

set(${SNAKE_EYE_UTIL_SUBMODULE}_SOURCE_FILES
    data_alloc.cpp
    dump_info.cpp
    err.cpp
    get_param.cpp
    list.cpp
    parse.cpp
    read_dir.cpp
    time_op.cpp
    utils.cpp
    )

add_library(${PROJECT_NAME} STATIC
    ${${SNAKE_EYE_UTIL_SUBMODULE}_HEADER_FILES}
    ${${SNAKE_EYE_UTIL_SUBMODULE}_SOURCE_FILES}
    )

add_library(se::${SNAKE_EYE_UTIL_SUBMODULE} ALIAS ${PROJECT_NAME})

# link third-party libraries
target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE nlohmann_json)

install (FILES ${${SNAKE_EYE_UTIL_SUBMODULE}_HEADER_FILES}
    DESTINATION include/se/${SNAKE_EYE_UTIL_SUBMODULE}
    )
