fips_add_subdirectory(api)
fips_add_subdirectory(compiler)
fips_add_subdirectory(util)
fips_add_subdirectory(test)

IF(BUILD_COMPILER)
fips_add_subdirectory(application)
ENDIF()