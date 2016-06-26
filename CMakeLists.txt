project(Kasnerik)
cmake_minimum_required (VERSION 3.0)

#switch to unicode mode
add_definitions(-DUNICODE -D_UNICODE)

#force release mode
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

#static linking for MSVC runtume
if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)

  foreach(flag_var CMAKE_C_FLAGS   CMAKE_C_FLAGS_DEBUG   CMAKE_C_FLAGS_RELEASE   CMAKE_C_FLAGS_MINSIZEREL   CMAKE_C_FLAGS_RELWITHDEBINFO
                   CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif()
  endforeach()
endif()

#static linking for GCC runtime
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  list(APPEND CMAKE_CXX_FLAGS "-fpermissive")

  list(APPEND CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "-static-libgcc")
  list(APPEND CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-static-libgcc -static-libstdc++")
endif()


#Library
add_library(libKasnerik SHARED 
  "src/Lib/dllmain.cpp"
  "3rdparty/minHook/src/buffer.c"
  "3rdparty/minHook/src/hook.c"
  "3rdparty/minHook/src/trampoline.c"
  "3rdparty/minHook/src/hde/hde32.c"
)
target_include_directories(libKasnerik PUBLIC "3rdparty/minHook/include")
set_target_properties(libKasnerik PROPERTIES PREFIX "")

#Executable
add_executable(Kasnerik WIN32 "src/Injector/main.cpp")

#Install to output directory
install(TARGETS libKasnerik Kasnerik
        RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/output" COMPONENT Runtime
        LIBRARY DESTINATION "${CMAKE_SOURCE_DIR}/output" COMPONENT Runtime
)
