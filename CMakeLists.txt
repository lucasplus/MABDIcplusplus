cmake_minimum_required( VERSION 2.8.12 ) 

# variables I'm hard coding for now
set( CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "Forcing only Debug" FORCE )

# name of solution
project( MABDI )

# ______________________________________________ project variables

# path folders in my project
set( MABDI_FOLDER_UTILITIES ${CMAKE_CURRENT_SOURCE_DIR}/util/ )

# for configuring the util.h file
set( MABDI_UTIL_STL_ENVIRONMENT ${MABDI_FOLDER_UTILITIES}stl/environment/ )

# the qt5 widgets we need 
set( MABDI_QT5_MODULES Core Widgets Gui )

# ______________________________________________ project wide configurations

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_ISGNUCXX )
  message("Compiler is gcc or g++ variant. Telling compiler to use C++11")
  list( APPEND CMAKE_CXX_FLAGS "-std=c++0x")
endif()

# ______________________________________________ define macros

include( CMakeMyMacros.cmake )

# ______________________________________________ find third party

# third party rundown
message("The MABDI library depends on VTK and OpenGL. Checking for them now.")
find_package( VTK REQUIRED )
find_package( OpenGL REQUIRED )

# tell me about VTK and OpenGL
message( "Found them! Let's look inside. \n")
message( "VTK info" )
message( "  Variable VTK_USE_FILE to be used with include() \n    "                 ${VTK_USE_FILE} )
message( "  Variable VTK_INCLUDE_DIRS to be used with include_directories() \n    " ${VTK_INCLUDE_DIRS} )
message( "  Variable VTK_LIBRARY_DIRS to be used with link_directories() \n    "    ${VTK_LIBRARY_DIRS} )
message( "  Variable VTK_LIBRARIES to be used with target_link_libraries() \n    "  ${VTK_LIBRARIES} )
message("")
message( "OpenGL info" )
message( "  Variable OPENGL_INCLUDE_DIR to be used with include_directories() \n    "  ${OPENGL_INCLUDE_DIR} )
message( "  Variable OPENGL_LIBRARIES to be used with target_link_libraries() \n    "  ${OPENGL_LIBRARIES} )
message("")

message("Qt5 required for apps. Checking for it now.")
find_package( Qt5 REQUIRED COMPONENTS ${MABDI_QT5_MODULES} )
message("Found it! \n")

# ______________________________________________ third party tasks that will affect all targets

# just has to be called once
# calls include_directories() and link_directories() for us
include( ${VTK_USE_FILE} ) 

# this variable is empty on WIN32, but still calling
include_directories( ${OPENGL_INCLUDE_DIR} )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/lib/ )

# for qt
set( CMAKE_AUTOMOC ON )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

# ______________________________________________ generate the MABDI library

# should then be target_link_libraries( <target> ${MABDI} )
add_subdirectory( lib )

# ______________________________________________ add tests and apps

add_subdirectory( test )
add_subdirectory( src )

# ______________________________________________ save for later


# creating a bin directory will the executables will go
# unfortunately when running visual studio from the IDE the working directory
# of each project is not the same as EXECUTABLE_OUTPUT_PATH
#set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

# put a copy of the utilities folder in the bin folder
# we will also have to put a copy for each project because when running
# the project from the Visual Studio IDE the working directory is different
#foreach( buildConfigType ${CMAKE_CONFIGURATION_TYPES} )
#  file( COPY ${MABDI_FOLDER_UTILITIES} 
#    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/${buildConfigType}/util/ )
#endforeach( buildConfigType )

