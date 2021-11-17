cmake_minimum_required (VERSION 3.8)

project (mytty1 CXX)

my_find_src_files (src_files mytty1)
add_executable (mytty1 ${src_files})
my_add_compile_options_c (mytty1)



if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS mytty1 DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  mytty1
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx mytty1 rsync://debian@devtgt/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_BBB_KbdModal/mytty1)
endif()
