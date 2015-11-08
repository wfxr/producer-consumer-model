cmake_minimum_required (VERSION 3.1.0)
project (ProducerConsumerModel)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi -lpthread")

add_executable (demo Program.cpp)
