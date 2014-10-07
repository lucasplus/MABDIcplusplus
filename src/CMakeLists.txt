
get_sub_dir_list( TEST_DIRS ${CMAKE_CURRENT_LIST_DIR} )
foreach( MY_MEMBER ${TEST_DIRS})
  message( "  APP directory: " ${MY_MEMBER} )
  add_new_executable_common_tasks( ${MY_MEMBER} FALSE )
endforeach( MY_MEMBER )

