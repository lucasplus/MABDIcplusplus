cmake_minimum_required( VERSION 2.8.12 )

# variables I'm hard coding for now
set( VTK_DIR "C:/3rdparty/vtk_install_debug" )
set( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/3rdparty/qt_5_3_0/5.3/msvc2012_opengl" )
set( CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "Forcing only Debug" FORCE )

# name of solution
project( MABDI )

# ______________________________________________ define macros

# list children directories of a given directory
macro( get_sub_dir_list result curdir )
  file( GLOB children RELATIVE ${curdir} ${curdir}/* )
  set( dirlist "" )
  foreach( child ${children} )
    if( IS_DIRECTORY ${curdir}/${child} )
        set( dirlist ${dirlist} ${child} )
    endif()
  endforeach()
  set( ${result} ${dirlist} )
endmacro()

# tasks common to setting up a new project
macro( add_new_project_common_tasks projectName )
	# initialize the project
	project( ${projectName} )

	# get all the sources
	file( GLOB THIS_SOURCES "*.cpp" )
	file( GLOB THIS_HEADERS "*.h" )
	file( GLOB THIS_QT_FORMS "*.ui" )
	qt5_wrap_ui( THIS_QT_FORMS ${THIS_QT_FORMS} )

	# add all to single variable THIS_SOURCES_ALL only if it exists
	if( THIS_SOURCES )
		set( THIS_SOURCES_ALL ${THIS_SOURCES_ALL} ${THIS_SOURCES} )	
	endif( THIS_SOURCES )
	if( THIS_HEADERS )
		set( THIS_SOURCES_ALL ${THIS_SOURCES_ALL} ${THIS_HEADERS} )	
	endif( THIS_HEADERS )
	if( THIS_QT_FORMS )
		set( THIS_SOURCES_ALL ${THIS_SOURCES_ALL} ${THIS_QT_FORMS} )	
	endif( THIS_QT_FORMS )
	
	# make the project
	add_executable( ${projectName} ${THIS_SOURCES_ALL} )
  
  # qt5 modules to use
  qt5_use_modules( ${projectName} ${BUILD_QT5_MODULES} )

	# link to everything
	target_link_libraries( ${projectName} ${BUILD_TARGET_LINK_LIBRARIES} )
	
	# copy the utilities folder to the project folder
	file( COPY ${BUILD_UTILITIES_FOLDER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/util/ )
endmacro()

# ______________________________________________ find third party

# find those third party libraries 
set( BUILD_QT5_MODULES Core Widgets Gui )
find_package( Qt5 REQUIRED COMPONENTS ${BUILD_QT5_MODULES} )
find_package( VTK REQUIRED )
find_package( OpenGL REQUIRED )

# rolling everything up into 5 variables
# as a result; include(), add_definitions(), link_directories(), and target_link_libraries()
# will be called with the same inputs for each project. inefficient but simple
#
# include( ${BUILD_INCLUDE} )
# add_definitions( ${BUILD_ADD_DEFINITIONS} )
# include_directories( ${BUILD_INCLUDE_DIRECTORIES} )
# link_directories( ${BUILD_LINK_DIRECTORIES} )
# target_link_libraries( <executable_name> ${BUILD_TARGET_LINK_LIBRARIES} )
set( BUILD_INCLUDE ${BUILD_INCLUDE} 
	${VTK_USE_FILE} )
set( BUILD_INCLUDE_DIRECTORIES ${BUILD_INCLUDE_DIRECTORIES} 
	${VTK_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR} )
set( BUILD_LINK_DIRECTORIES ${BUILD_INCLUDE_DIRECTORIES} 
	${VTK_LIBRARY_DIRS} )
set( BUILD_TARGET_LINK_LIBRARIES ${BUILD_TARGET_LINK_LIBRARIES} 
	${VTK_LIBRARIES} 
  ${OPENGL_LIBRARIES} )

# calls to find_package can simply append to some variables
# in order to make cmake messages more useful, remove duplicates
list( REMOVE_DUPLICATES BUILD_INCLUDE ) 
list( REMOVE_DUPLICATES BUILD_INCLUDE_DIRECTORIES ) 
list( REMOVE_DUPLICATES BUILD_LINK_DIRECTORIES ) 
list( REMOVE_DUPLICATES BUILD_TARGET_LINK_LIBRARIES ) 
	
# tell me about my third party configuration
message( "\n BUILD_INCLUDE Everything that will go in the CMake include() funtion" )
foreach( MY_MEMBER ${BUILD_INCLUDE})
  message( "   " ${MY_MEMBER} )
endforeach( MY_MEMBER )
message( "\n BUILD_INCLUDE_DIRECTORIES Everything that will go in the CMake include_directories() funtion" )
foreach( MY_MEMBER ${BUILD_INCLUDE_DIRECTORIES})
  message( "   " ${MY_MEMBER} )
endforeach( MY_MEMBER )
message( "\n BUILD_LINK_DIRECTORIES Everything that will go in the CMake link_directories() funtion" )
foreach( MY_MEMBER ${BUILD_LINK_DIRECTORIES})
  message( "   " ${MY_MEMBER} )
endforeach( MY_MEMBER )	
message( "\n BUILD_TARGET_LINK_LIBRARIES Everything that will go in the CMake target_link_libraries() funtion" )
foreach( MY_MEMBER ${BUILD_TARGET_LINK_LIBRARIES})
  message( "   " ${MY_MEMBER} )
endforeach( MY_MEMBER )

# ______________________________________________ global configuring

# creating a bin directory will the executables will go
# unfortunately when running visual studio from the IDE the working directory
# of each project is not the same as EXECUTABLE_OUTPUT_PATH
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

# use my third party configuration
# target_link_libraries() has to be called for each target
include( ${BUILD_INCLUDE} )
include_directories( ${BUILD_INCLUDE_DIRECTORIES} )
link_directories( ${BUILD_LINK_DIRECTORIES} )

# tell CMake to run moc when needed ( a Qt thing )
# moc files are generated in the binary dir, tell CMake to look for includes there
set( CMAKE_AUTOMOC ON )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

# ______________________________________________ utilities folder

# create variable BUILD_UTILITIES_FOLDER containing path to my utilities folder
set( BUILD_UTILITIES_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/src/util/ )

# put a copy of the utilities folder in the bin folder
# we will also have to put a copy for each project because when running
# the project from the Visual Studio IDE the working directory is different
foreach( buildConfigType ${CMAKE_CONFIGURATION_TYPES} )
  file( COPY ${BUILD_UTILITIES_FOLDER} 
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/${buildConfigType}/util/ )
endforeach( buildConfigType )

# ______________________________________________ adding subdirectories

# the test/sandbox area
get_sub_dir_list( TEST_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/ )
message("\n")
foreach( MY_MEMBER ${TEST_DIRS})
  message( "  Adding test directory: " ${MY_MEMBER} )
	add_subdirectory( src/tests${MY_MEMBER} )
endforeach( MY_MEMBER )
message("\n")



 
