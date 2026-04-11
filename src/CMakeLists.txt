file(GLOB SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/sqlite/sqlite3.c"
)

add_executable(cyberguard ${SOURCES})

target_include_directories(cyberguard PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party/sqlite
)

target_compile_features(cyberguard PRIVATE cxx_std_17)