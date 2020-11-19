cmake_minimum_required(VERSION 3.10.2)
project(gc)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-w")
find_package (Threads)
#add_link_options(-pthread)
add_executable(preprocessor src/preprocessor.cpp src/preprocessor_main.cpp include/preprocessor.h include/libraries.h)
add_executable(helper src/helper.cpp src/helper_main.cpp include/helper.h include/libraries.h)
add_executable(alice src/alice.cpp src/alice_main.cpp src/server.cpp include/alice.h include/server.h include/message.h include/libraries.h)
add_executable(bob src/bob.cpp src/bob_main.cpp src/client.cpp include/bob.h include/client.h include/message.h include/libraries.h)

target_link_libraries (alice ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (bob ${CMAKE_THREAD_LIBS_INIT})

#add this for lib
#target_link_libraries(alice libsodium.a)