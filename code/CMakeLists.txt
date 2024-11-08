add_subdirectory(api)
add_subdirectory(util)

IF(BUILD_GPULANG_COMPILER)
add_subdirectory(compiler)
ENDIF()

IF(BUILD_GPULANG_APP)
add_subdirectory(app)
ENDIF()

IF(BUILD_GPULANG_TEST)
add_subdirectory(test)
ENDIF()

