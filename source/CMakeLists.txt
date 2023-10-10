
file(GLOB HEADER_FILES "*.h")
set(RUNTIME_HEADS "${HEADER_FILES}")
message(xmy include)
message(${RUNTIME_HEADS})
add_executable(MAIN main.cpp)

add_subdirectory(parser)

include(precompile/precompile.cmake)
set(CODEGEN_TARGET "PreCompile")

#add_dependencies(MAIN "${CODEGEN_TARGET}")

#add_dependencies("${CODEGEN_TARGET}" "Parser")