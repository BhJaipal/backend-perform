# CMake generated Testfile for 
# Source directory: /home/jaipal001/Desktop/files/chat-app/backend-c/cJSON
# Build directory: /home/jaipal001/Desktop/files/chat-app/backend-c/cJSON
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(cJSON_test "/home/jaipal001/Desktop/files/chat-app/backend-c/cJSON/cJSON_test")
set_tests_properties(cJSON_test PROPERTIES  _BACKTRACE_TRIPLES "/home/jaipal001/Desktop/files/chat-app/backend-c/cJSON/CMakeLists.txt;248;add_test;/home/jaipal001/Desktop/files/chat-app/backend-c/cJSON/CMakeLists.txt;0;")
subdirs("tests")
subdirs("fuzzing")
