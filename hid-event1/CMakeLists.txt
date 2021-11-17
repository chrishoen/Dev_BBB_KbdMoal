cmake_minimum_required (VERSION 3.8)

project (hid-event1 CXX)

my_find_src_files (src_files hid-event1)
add_executable (hid-event1 ${src_files})
my_add_compile_options_c (hid-event1)



if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS hid-event1 DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  hid-event1
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx hid-event1 rsync://debian@devtgt/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_BBB_KbdModal/hid-event1)
endif()
