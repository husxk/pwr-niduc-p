cmake_minimum_required(VERSION 3.13)

project(ble_sim CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
  main.cpp
  mt_rng.cpp
  tea.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

add_compile_options(-Wall -Wextra -fstack-protector-all -fsanitize=address)

add_link_options(-fsanitize=address)
