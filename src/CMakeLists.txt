file( GLOB SRC *.cpp )
file( GLOB HDR *.h )
#
add_library( DNSCache ${SRC} ${HDR} )
target_include_directories( DNSCache PUBLIC ${CMAKE_CURRENT_LIST_DIR} )
