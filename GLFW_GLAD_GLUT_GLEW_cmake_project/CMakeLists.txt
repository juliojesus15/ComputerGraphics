cmake_minimum_required(VERSION 3.10)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#set( CMAKE_BUILD_TYPE Release )
set( CMAKE_BUILD_TYPE Debug )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
  message(STATUS "\nBuild type not specified: Use Debug by default")
endif(NOT CMAKE_BUILD_TYPE)

set( DEPENDENCY_DIR 
${CMAKE_CURRENT_SOURCE_DIR}/dep)

message( "\nCurrent directory is: ${CMAKE_CURRENT_SOURCE_DIR}" )
message( "\nDependency directory is: ${DEPENDENCY_DIR}" )
message( "\nBuild mode is: ${CMAKE_BUILD_TYPE}" )

project( GLFW_Tutorials )

if ( WIN32 )
	message( "\nCONFIGURING FOR WINDOWS\n" )

else()
	message( "\nCONFIGURING FOR LINUX\n" )
	
endif()

add_subdirectory( src )

