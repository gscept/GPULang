add_subdirectory(api)
add_subdirectory(compiler)
add_subdirectory(util)
add_subdirectory(test)

IF(BUILD_COMPILER)
add_subdirectory(application)
ENDIF()