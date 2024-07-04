set(SNAME_EYE_THREAD_SUBMODULE "thread")
project(se_${SNAME_EYE_THREAD_SUBMODULE})

set(${SNAME_EYE_THREAD_SUBMODULE}_HEADER_FILES
    base_thread.hpp
    decode_thread.hpp
    encode_thread.hpp
    read_thread.hpp
    read_transfer_thread.hpp
    transform_thread.hpp
    write_thread.hpp
    write_transfer_thread.hpp
    )

add_library(${PROJECT_NAME} INTERFACE
    ${${SNAME_EYE_THREAD_SUBMODULE}_HEADER_FILES}
    )

add_library(se::${SNAME_EYE_THREAD_SUBMODULE} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../task
    )

install(FILES ${${SNAME_EYE_THREAD_SUBMODULE}_HEADER_FILES}
    DESTINATION include/se/${SNAME_EYE_THREAD_SUBMODULE}
    )

add_dependencies(${PROJECT_NAME} se_task)
