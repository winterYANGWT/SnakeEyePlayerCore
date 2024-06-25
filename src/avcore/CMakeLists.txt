set(OSFF_SUB_MODULE avcore)
project(osff_avcore)

set(${OSFF_SUB_MODULE}_HEADER_FILES
    avcore.h
    )

set(${OSFF_SUB_MODULE}_SOURCE_FILES
    avcore_basic.cpp
    avcore_info.cpp
    avcore_pipeline.cpp
    avcore_play.cpp
    )

add_library(${PROJECT_NAME} SHARED
    ${${OSFF_SUB_MODULE}_HEADER_FILES}
    ${${OSFF_SUB_MODULE}_SOURCE_FILES})

add_library(osff::${OSFF_SUB_MODULE} ALIAS ${PROJECT_NAME})
add_executable(main main.cpp)



target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../manager
    ${CMAKE_CURRENT_SOURCE_DIR}/../parser
    ${CMAKE_CURRENT_SOURCE_DIR}/../queue
    ${CMAKE_CURRENT_SOURCE_DIR}/../thread
    ${CMAKE_CURRENT_SOURCE_DIR}/../util
    )
target_link_libraries(${PROJECT_NAME} PUBLIC
    osff::manager
    osff::parser
    osff::queue
    osff::thread
    )

target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE nlohmann_json)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    )
install(FILES ${${OSFF_SUB_MODULE}_HEADER_FILES}
    DESTINATION include/osff/${OSFF_SUB_MODULE})

add_dependencies(${PROJECT_NAME} 
    osff_manager 
    osff_parser
    osff_util)

target_link_libraries(main ${PROJECT_NAME})