cmake_minimum_required (VERSION 3.8)

project (kbdmodal CXX)
my_find_src_files (src_files kbdmodal)
add_executable (kbdmodal ${src_files})
my_add_compile_options (kbdmodal)
my_add_pch (kbdmodal)

my_inc_import_RisLib(kbdmodal)
my_lib_import_RisLib(kbdmodal)


add_custom_command(
  TARGET  kbdmodal
  POST_BUILD
  COMMAND rsync -azv --chmod=ugo=rwx kbdmodal rsync://debian@devtgt/prime/bin
  WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_BBB_KbdModal/kbdmodal)
