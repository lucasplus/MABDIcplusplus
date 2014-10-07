# ______________________________________________ add_new_executable_common_tasks

# Tasks common to setting up a new executable
# It is assumed this macro is called from a folder containing several projects,
# each project in its own folder. It is assumed that the name of the target is the same
# its specific folder. 
# THIS_TARGET - Name of the new target and also the name of the folder which has all the needed files
# THIS_DO_LINK_VTK - Should I link vtk to this target?
macro( add_new_executable_common_tasks THIS_TARGET THIS_DO_LINK_VTK )
  # initialize the project
	project( ${THIS_TARGET} )
   
	# get all the sources
	file( GLOB THIS_SOURCES  ${THIS_TARGET}/*.cpp )
	file( GLOB THIS_HEADERS  ${THIS_TARGET}/*.h )
  file( GLOB THIS_QT_FORMS ${THIS_TARGET}/*.ui )
   
	# add all to single variable THIS_SOURCES_ALL only if it exists
  set( THIS_SOURCES_ALL "" )
	if( THIS_SOURCES )
		set( THIS_SOURCES_ALL ${THIS_SOURCES_ALL} ${THIS_SOURCES} )	
	endif( THIS_SOURCES )
	if( THIS_HEADERS )
		set( THIS_SOURCES_ALL ${THIS_SOURCES_ALL} ${THIS_HEADERS} )	
	endif( THIS_HEADERS )
	if( THIS_QT_FORMS )
    qt5_wrap_ui( THIS_QT_FORMS ${THIS_QT_FORMS} )
		set( THIS_SOURCES_ALL ${THIS_SOURCES_ALL} ${THIS_QT_FORMS} )	
	endif( THIS_QT_FORMS )
   	
	# make the project
  add_executable( ${THIS_TARGET} ${THIS_SOURCES_ALL} )
   
  # qt5 modules to use
  qt5_use_modules( ${THIS_TARGET} ${MABDI_QT5_MODULES} )
   
	# link to libraries
  if( ${THIS_DO_LINK_VTK} )
    message("    Linking VTK")
    target_link_libraries( ${THIS_TARGET} ${VTK_LIBRARIES} ${OPENGL_LIBRARIES} )
  else()
    message("    Not Linking VTK")
    target_link_libraries( ${THIS_TARGET} ${MABDI} )
  endif()

  message("    THIS_PROJECT_NAME: " ${THIS_TARGET} )
  message("    CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR} )
  message("    PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR} )
  message("    PROJECT_BINARY_DIR: " ${PROJECT_BINARY_DIR} )
  message("    THIS_SOURCES_ALL: "  )
    foreach( MY_MEMBER ${THIS_SOURCES_ALL})
      message( "      " ${MY_MEMBER} )
    endforeach( MY_MEMBER )
  message("")
endmacro()

# ______________________________________________ get_sub_dir_list

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

# ______________________________________________ show_ls

# print members of a list variable on their own line
# show_ls( "${MY_VARIABLE}" )  # Has to be quoted, CMake is weird
macro( show_ls THIS_LIST )
  foreach( MY_MEMBER ${THIS_LIST} )
      message( "    " ${MY_MEMBER} )
  endforeach( MY_MEMBER )
endmacro()

