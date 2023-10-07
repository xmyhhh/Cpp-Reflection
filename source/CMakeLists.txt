
add_subdirectory(meta_parser)


add_executable(MAIN main.cpp)

set(CODEGEN_TARGET "PreCompile")

include(precompile/precompile.cmake)

#set_target_properties("${CODEGEN_TARGET}" PROPERTIES FOLDER "Engine" )


add_dependencies(MAIN "${CODEGEN_TARGET}")
add_dependencies("${CODEGEN_TARGET}" "Parser")