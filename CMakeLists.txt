cmake_minimum_required (VERSION 3.8)

set (CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/MyOverrides.cmake")
include(MyPrecompiledHeader.cmake)
include(MyCodeMacros.cmake)
include(MyGlobalImports.cmake)
include(MyLocalImports.cmake)

project ("Dev_BBB_KbdModal")
my_init_global_import_variables()

add_subdirectory ("kbdmodal")
return()

add_subdirectory ("Test")
return()

