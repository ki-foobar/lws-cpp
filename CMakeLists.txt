cmake_minimum_required(VERSION 3.1)

project(lws)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")


add_executable(${PROJECT_NAME}
    src/main.cpp
    src/elona.cpp
    src/enchantment.cpp
    src/random/engine.cpp
    src/random/title_generator.cpp
    src/weapon_enchantment_searcher.cpp
)

target_link_libraries(${PROJECT_NAME} pthread)


add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    -E copy_directory
    ${CMAKE_SOURCE_DIR}/data/
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/data/
)

