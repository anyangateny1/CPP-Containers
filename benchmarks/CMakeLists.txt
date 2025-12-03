include_directories(${CMAKE_SOURCE_DIR}/containers)

file(GLOB BENCHMARK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(BENCHMARK_SOURCE ${BENCHMARK_SOURCES})
    get_filename_component(BENCHMARK_NAME ${BENCHMARK_SOURCE} NAME_WE)
    
    add_executable(${BENCHMARK_NAME} ${BENCHMARK_SOURCE})
    
    target_link_libraries(${BENCHMARK_NAME} PRIVATE nanobench)
    
    target_compile_options(${BENCHMARK_NAME} PRIVATE
        -Wall -Wextra -Wpedantic -Wshadow
    )
    
    message(STATUS "Added benchmark: ${BENCHMARK_NAME}")
endforeach()

list(LENGTH BENCHMARK_SOURCES NUM_BENCHMARKS)
if(NUM_BENCHMARKS EQUAL 0)
    message(WARNING "No benchmark .cpp files found in ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

