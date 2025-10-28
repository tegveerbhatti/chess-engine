cmake_minimum_required(VERSION 3.10)
project(chess_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(chess_engine
    src/main.cpp
    src/board.cpp
    src/fen.cpp
    src/utils.cpp
    src/move.cpp
    src/movegen.cpp
    src/make_move.cpp
    src/eval.cpp
    src/search.cpp
    src/uci.cpp
)

# Tests
enable_testing()

add_executable(test_fen tests/test_fen.cpp src/board.cpp src/fen.cpp)
add_test(NAME FenTest COMMAND test_fen)

add_executable(test_movegen tests/test_movegen.cpp src/board.cpp src/fen.cpp src/movegen.cpp src/move.cpp src/make_move.cpp)
add_test(NAME MovegenTest COMMAND test_movegen)

add_executable(test_make_unmake tests/test_make_unmake.cpp src/board.cpp src/fen.cpp src/movegen.cpp src/move.cpp src/make_move.cpp)
add_test(NAME MakeUnmakeTest COMMAND test_make_unmake)

add_executable(test_eval tests/test_eval.cpp src/board.cpp src/fen.cpp src/eval.cpp)
add_test(NAME EvalTest COMMAND test_eval)

add_executable(test_search tests/test_search.cpp src/board.cpp src/fen.cpp src/search.cpp src/movegen.cpp src/make_move.cpp src/eval.cpp src/move.cpp)
add_test(NAME SearchTest COMMAND test_search)

add_executable(test_performance tests/test_performance.cpp src/board.cpp src/fen.cpp src/search.cpp src/movegen.cpp src/make_move.cpp src/eval.cpp src/move.cpp)
# Note: Performance test is not added to ctest as it's for benchmarking, not validation
