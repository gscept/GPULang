#-------------------------------------------------------------------------------
# antlr4
#-------------------------------------------------------------------------------


ADD_DEFINITIONS(-DANTLR4CPP_STATIC)
FILE(GLOB SRC "src/*.cpp")
FILE(GLOB HEAD "src/*.h")
SOURCE_GROUP("antlr4" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "src/atn/*.cpp")
FILE(GLOB HEAD "src/atn/*.h")
SOURCE_GROUP("antlr4\\atn" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "src/dfa/*.cpp")
FILE(GLOB HEAD "src/dfa/*.h")
SOURCE_GROUP("antlr4\\dfs" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "src/tree/*.cpp")
FILE(GLOB HEAD "src/tree/*.h")
SOURCE_GROUP("antlr4\\tree" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "src/tree/pattern/*.cpp")
FILE(GLOB HEAD "src/tree/pattern/*.h")
SOURCE_GROUP("antlr4\\tree\\pattern" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "src/tree/xpath/*.cpp")
FILE(GLOB HEAD "src/tree/xpath/*.h")
SOURCE_GROUP("antlr4\\tree\\xpath" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "src/support/*.cpp")
FILE(GLOB HEAD "src/support/*.h")
SOURCE_GROUP("antlr4\\support" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "src/misc/*.cpp")
FILE(GLOB HEAD "src/misc/*.h")
SOURCE_GROUP("antlr4\\misc" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

ADD_LIBRARY(antlr4 STATIC ${CODE})
target_include_directories(antlr4 PUBLIC ${CMAKE_CURRENT_BINARY_DIR} src)
