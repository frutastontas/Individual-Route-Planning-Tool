cmake_minimum_required(VERSION 3.26)

project(Individual-Route-Planning-Tool)

set(CMAKE_CXX_STANDARD 14)
set( CMAKE_BUILD_TYPE_TMP "${CMAKE_BUILD_TYPE}" )
set( CMAKE_BUILD_TYPE "Release" )

add_executable(Individual-Route-Planning-Tool main.cpp
)