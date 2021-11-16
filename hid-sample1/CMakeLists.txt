cmake_minimum_required (VERSION 3.8)

project (hid-sample1 CXX)

my_find_src_files (src_files hid-sample1)
add_executable (hid-sample1 ${src_files})
my_add_compile_options_c (hid-sample1)



if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS hid-sample1 DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  hid-sample1
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx hid-sample1 rsync://debian@devtgt/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_BBB_KbdModal/hid-sample1)
endif()
