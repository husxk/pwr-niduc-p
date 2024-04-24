cmake_minimum_required(VERSION 3.13)

project(gen_dist_data CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/../)

add_executable(${PROJECT_NAME} gen_dist_data.cpp ../mt_rng.cpp )

add_compile_options(-Wall -Wextra -fstack-protector-all -fsanitize=address)

add_link_options(-fsanitize=address)
