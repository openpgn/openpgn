configure_file(sample.h.in sample.h NEWLINE_STYLE UNIX)
configure_file(conf.h.in conf.h NEWLINE_STYLE UNIX)

file(GLOB SAMPLES samples/*.pgn)
file(GLOB CONFS   *.in)

add_executable(OpenPGNTest ${SAMPLES} ${CONFS} main.c)
target_include_directories(OpenPGNTest PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(OpenPGNTest PUBLIC OpenPGN)

add_test(NAME coverage COMMAND OpenPGNTest "${CMAKE_CURRENT_LIST_DIR}/sample.pgn")