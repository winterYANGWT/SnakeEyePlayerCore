set(OSFF_SUB_MODULE util)
project(osff_${OSFF_SUB_MODULE})

set(${OSFF_SUB_MODULE}_HEADER_FILES
    osff_macro.h
    util.h
    utils.h
    )

set(${OSFF_SUB_MODULE}_SOURCE_FILES
    data_alloc.cpp
    dump_info.cpp
    err.cpp
    get_param.cpp
    list.cpp
    parse.cpp
    time_op.cpp
    utils.cpp
    )

add_library(${PROJECT_NAME} STATIC
    ${${OSFF_SUB_MODULE}_HEADER_FILES}
    ${${OSFF_SUB_MODULE}_SOURCE_FILES}
    )

add_library(osff::${OSFF_SUB_MODULE} ALIAS ${PROJECT_NAME})

# link third-party libraries
target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE nlohmann_json)

install (FILES ${${OSFF_SUB_MODULE}_HEADER_FILES}
    DESTINATION include/osff/${OSFF_SUB_MODULE}
    )