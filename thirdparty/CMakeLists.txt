find_package(nlohmann_json QUIET)

if (nlohmann_json_FOUND)
    message(STATUS "nlohmann_json found")
else()
    message(STATUS "nlohmann_json not found")
    set(JSON_Install ON)
    add_subdirectory(json)
endif()