find_package(GTest REQUIRED)
#
enable_testing()
#
file( GLOB SRC *.cpp )
file( GLOB HDR *.hpp )
#
add_executable( DNSCacheTest ${SRC} ${HDR})
add_dependencies( DNSCacheTest DNSCache)
#
target_link_libraries( DNSCacheTest PRIVATE DNSCache GTest::gtest GTest::gtest_main )
#
gtest_discover_tests( DNSCacheTest )
