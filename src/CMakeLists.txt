cmake_minimum_required (VERSION 3.16)

project(geo.surveyblox.com)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(geo.surveyblox.com
  "Main.cpp"
  "Server.cpp"
  "Log.cpp"
)

