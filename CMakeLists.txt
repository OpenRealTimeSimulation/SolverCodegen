cmake_minimum_required(VERSION 3.24)

project(SolverCodegen)

add_subdirectory(eigen)

file(GLOB SolverCodegen_SOURCE
    "src/exprpar/*.cpp"
    "src/codegen/*.cpp"
    "src/codegen/components/*.cpp"
    "src/codegen/netlist/*.cpp"
    "src/codegen/netlist/producers/*.cpp"
    "src/codegen/udc/*.cpp"
)

add_executable(SolverCodegen exe/codegen_cli_main.cpp)
target_sources(SolverCodegen PRIVATE ${SolverCodegen_SOURCE})
target_link_libraries(SolverCodegen PRIVATE eigen)
target_include_directories(SolverCodegen PRIVATE include/)
