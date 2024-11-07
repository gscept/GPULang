add_subdirectory(api)
add_subdirectory(compiler)
add_subdirectory(util)

IF(BUILD_GPULANG_APP)
add_subdirectory(app)
ENDIF()

IF(BUILD_GPULANG_TEST)
add_subdirectory(test)
ENDIF()

