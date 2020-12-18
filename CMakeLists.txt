cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)
project (advent-of-code-2020)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)
add_library(boost INTERFACE IMPORTED)
set_property(TARGET boost PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})

add_subdirectory(aoc_common/aoc_input)

#add_subdirectory(day1)
#add_subdirectory(day2)
#add_subdirectory(day3)
#add_subdirectory(day4)
#add_subdirectory(day5)
#add_subdirectory(day6)
#add_subdirectory(day7)
#add_subdirectory(day8)
#add_subdirectory(day9)
#add_subdirectory(day10)
#add_subdirectory(day11)
#add_subdirectory(day12)
#add_subdirectory(day13)
#add_subdirectory(day14)
#add_subdirectory(day15)
add_subdirectory(day16)
add_subdirectory(day17)
