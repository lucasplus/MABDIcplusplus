
get_sub_dir_list( TEST_DIRS ${CMAKE_CURRENT_LIST_DIR} )
foreach( MY_MEMBER ${TEST_DIRS})
  message( "  TEST directory: " ${MY_MEMBER} )
  add_new_executable_common_tasks( ${MY_MEMBER} TRUE )
endforeach( MY_MEMBER )

# copy the utilities folder to the project folder
file( COPY ${MABDI_FOLDER_UTILITIES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/util/ )
