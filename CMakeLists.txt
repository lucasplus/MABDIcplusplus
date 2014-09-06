cmake_minimum_required( VERSION 2.8.12 )

# variables I'm hard coding for now
set( VTK_DIR "C:/3rdparty/vtk_install_debug" )
set( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/3rdparty/qt_5_3_0/5.3/msvc2012_opengl" )
set( CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "Forcing only Debug" FORCE )

# name of solution
project( MABDI )

# ______________________________________________ project variables

# path folders in my project
set( MABDI_FOLDER_UTILITIES ${CMAKE_CURRENT_SOURCE_DIR}/util/ )
set( MABDI_FOLDER_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/ )
set( MABDI_FOLDER_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/ )
set( MABDI_FOLDER_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/ )

# for configuring the util.h file
set( MABDI_UTIL_STL_ENVIRONMENT ${MABDI_FOLDER_UTILITIES}stl/environment/ )
set( MABDI_UTIL_LOG_CONFIG_FILE ${MABDI_FOLDER_UTILITIES}LogConfig.conf )

# the qt5 widgets we need 
set( MABDI_QT5_MODULES Core Widgets Gui )

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
	add_executable( ${projectName} ${THIS_SOURCES_ALL} ${MABDI_LIB_FILES} )
  
  # qt5 modules to use
  qt5_use_modules( ${projectName} ${MABDI_QT5_MODULES} )

	# link to everything
	target_link_libraries( ${projectName} ${MABDI_TARGET_LINK_LIBRARIES} )
	
	# copy the utilities folder to the project folder
	file( COPY ${MABDI_FOLDER_UTILITIES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/util/ )
endmacro()

# ______________________________________________ find third party

# find those third party libraries 
find_package( Qt5 REQUIRED COMPONENTS ${MABDI_QT5_MODULES} )
find_package( VTK REQUIRED )
find_package( OpenGL REQUIRED )

# rolling everything up into 5 variables
# as a result; include(), add_definitions(), link_directories(), and target_link_libraries()
# will be called with the same inputs for each project. inefficient but simple
#
# include( ${MABDI_INCLUDE} )
# add_definitions( ${MABDI_ADD_DEFINITIONS} )
# include_directories( ${MABDI_INCLUDE_DIRECTORIES} )
# link_directories( ${MABDI_LINK_DIRECTORIES} )
# target_link_libraries( <executable_name> ${MABDI_TARGET_LINK_LIBRARIES} )
set( MABDI_INCLUDE ${MABDI_INCLUDE} 
	${VTK_USE_FILE} )
set( MABDI_INCLUDE_DIRECTORIES ${MABDI_INCLUDE_DIRECTORIES} 
	${VTK_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR} )
set( MABDI_LINK_DIRECTORIES ${MABDI_INCLUDE_DIRECTORIES} 
	${VTK_LIBRARY_DIRS} )
set( MABDI_TARGET_LINK_LIBRARIES ${MABDI_TARGET_LINK_LIBRARIES} 
	${VTK_LIBRARIES} 
  ${OPENGL_LIBRARIES} )

# calls to find_package can simply append to some variables
# in order to make cmake messages more useful, remove duplicates
list( REMOVE_DUPLICATES MABDI_INCLUDE ) 
list( REMOVE_DUPLICATES MABDI_INCLUDE_DIRECTORIES ) 
list( REMOVE_DUPLICATES MABDI_LINK_DIRECTORIES ) 
list( REMOVE_DUPLICATES MABDI_TARGET_LINK_LIBRARIES ) 
	
# tell me about my third party configuration
message( "\n MABDI_INCLUDE Everything that will go in the CMake include() funtion" )
foreach( MY_MEMBER ${MABDI_INCLUDE})
  message( "   " ${MY_MEMBER} )
endforeach( MY_MEMBER )
message( "\n MABDI_INCLUDE_DIRECTORIES Everything that will go in the CMake include_directories() funtion" )
foreach( MY_MEMBER ${MABDI_INCLUDE_DIRECTORIES})
  message( "   " ${MY_MEMBER} )
endforeach( MY_MEMBER )
message( "\n MABDI_LINK_DIRECTORIES Everything that will go in the CMake link_directories() funtion" )
foreach( MY_MEMBER ${MABDI_LINK_DIRECTORIES})
  message( "   " ${MY_MEMBER} )
endforeach( MY_MEMBER )	
message( "\n MABDI_TARGET_LINK_LIBRARIES Everything that will go in the CMake target_link_libraries() funtion" )
foreach( MY_MEMBER ${MABDI_TARGET_LINK_LIBRARIES})
  message( "   " ${MY_MEMBER} )
endforeach( MY_MEMBER )

# ______________________________________________ files related to MABDI library

# simply glob all my library implementation files for right now
file( GLOB MABDI_LIB_FILES ${MABDI_FOLDER_LIB} "*.cpp" "*.hpp" "*.h" )

# ______________________________________________ global configuring

# configure utility header
configure_file( ${MABDI_FOLDER_INCLUDE}util.h.in ${MABDI_FOLDER_INCLUDE}util.h )

# creating a bin directory will the executables will go
# unfortunately when running visual studio from the IDE the working directory
# of each project is not the same as EXECUTABLE_OUTPUT_PATH
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

# use my third party configuration
# target_link_libraries() has to be called for each target
include( ${MABDI_INCLUDE} )
include_directories( ${MABDI_INCLUDE_DIRECTORIES} )
link_directories( ${MABDI_LINK_DIRECTORIES} )

# include path to where my interface files are to my library
include_directories( ${MABDI_FOLDER_INCLUDE} )

# tell CMake to run moc when needed ( a Qt thing )
# moc files are generated in the binary dir, tell CMake to look for includes there
set( CMAKE_AUTOMOC ON )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

# ______________________________________________ utilities folder

# put a copy of the utilities folder in the bin folder
# we will also have to put a copy for each project because when running
# the project from the Visual Studio IDE the working directory is different
foreach( buildConfigType ${CMAKE_CONFIGURATION_TYPES} )
  file( COPY ${MABDI_FOLDER_UTILITIES} 
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/${buildConfigType}/util/ )
endforeach( buildConfigType )

# ______________________________________________ adding subdirectories

# the test/sandbox area
get_sub_dir_list( TEST_DIRS ${MABDI_FOLDER_TESTS} )
message("\n")
foreach( MY_MEMBER ${TEST_DIRS})
  message( "  Adding test directory: " ${MY_MEMBER} )
	add_subdirectory( tests${MY_MEMBER} )
endforeach( MY_MEMBER )
message("\n")

