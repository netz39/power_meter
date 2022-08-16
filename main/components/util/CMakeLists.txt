cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0048 NEW)

project(util LANGUAGES CXX C ASM)

add_library(${PROJECT_NAME} STATIC
		src/wrappers/BinarySemaphore.cxx
        src/wrappers/Mutex.cxx
		src/wrappers/sync.cxx
        src/wrappers/Task.cxx
        
)

target_include_directories(${PROJECT_NAME} PUBLIC
        include
        )