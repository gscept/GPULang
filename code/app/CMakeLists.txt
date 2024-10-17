#-------------------------------------------------------------------------------
# GPULang app
#-------------------------------------------------------------------------------


fips_begin_app(gpulang_app windowed)
fips_deps(gpulang_util gpulang_reflection glfw3)

target_include_directories(gpulang_app PUBLIC
	${GPULANG_ROOT}
	${GPULANG_ROOT}/code/util
	${GPULANG_ROOT}/api
	${GPULANG_ROOT}/api/lowlevel
	${VULKAN_INCLUDE_DIR}
)

ADD_DEFINITIONS(-DSHADER_FOLDER="${GPULANG_ROOT}/code/app/shaders")


IF(WIN32)
    add_definitions(-D__WIN32__)
ELSEIF(UNIX AND NOT APPLE)
    add_definitions(-D__linux__)
ELSEIF(APPLE)
    add_definitions(-D__APPLE__)
ENDIF()

fips_files(
	main.cc
)

fips_end_app()
