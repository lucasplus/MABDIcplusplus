
set( CMAKE_AUTOMOC OFF )
set( CMAKE_INCLUDE_CURRENT_DIR OFF )

# grab all Mabdi library files
file( GLOB MABDI_LIB_FILES "*.cpp" "*.hpp" "*.h" )

# generate a static library 
# static means it will be resolved by the caller at compile time
add_library( MABDI STATIC ${MABDI_LIB_FILES} )

# dependencies of MABDI library 
# TODO: Not sure if this is done right
target_link_libraries( MABDI ${VTK_LIBRARIES} ${OPENGL_LIBRARIES} )

# tell me what files were used in the MABDI library 
message( "MABDI LIBRARY creating library with files:" )
show_ls( "${MABDI_LIB_FILES}" )

message( "  CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR} )
message( "  CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR} )
message("")

set( CMAKE_AUTOMOC ON )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
