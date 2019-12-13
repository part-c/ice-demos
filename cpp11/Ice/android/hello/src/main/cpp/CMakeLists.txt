# Copyright (c) ZeroC, Inc. All rights reserved.

cmake_minimum_required(VERSION 3.4.1)

# Check for Slice translator
if(${ICE_HOME})
    if(EXISTS ${ICE_HOME}/cpp/bin/slice2cpp)
        set(slice2cpp ${ICE_HOME}/cpp/bin/slice2cpp)
    else(EXISTS ${ICE_HOME}/bin/slice2cpp)
        set(slice2cpp ${ICE_HOME}/bin/slice2cpp)
    endif()
else()
    if(EXISTS /usr/bin/slice2cpp)
        set(slice2cpp /usr/bin/slice2cpp)
    elseif(EXISTS /usr/local/bin/slice2cpp)
        set(slice2cpp /usr/local/bin/slice2cpp)
    endif()
endif()

add_custom_command(OUTPUT Hello.cpp Hello.h
                   COMMAND ${slice2cpp}
                   ARGS ${CMAKE_CURRENT_SOURCE_DIR}/../slice/Hello.ice
                   MAIN_DEPENDENCY ../slice/Hello.ice)

include_directories("${ICE_ANDROID_NDK_HOME}/include")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_definitions(-DICE_CPP11_MAPPING)

add_library(ice STATIC IMPORTED)
set_target_properties(ice PROPERTIES IMPORTED_LOCATION
    ${ICE_ANDROID_NDK_HOME}/lib/static/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}/libice.a)

add_library(helloclient SHARED Hello.cpp HelloClient.cpp)
target_link_libraries(helloclient ice android)
