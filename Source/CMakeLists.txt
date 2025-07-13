file(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(TilePainter ${SRC_FILES})

target_include_directories(TilePainter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../Build/_deps/sdlengine-src/Include
)

target_link_libraries(TilePainter
    SDLEngine
)