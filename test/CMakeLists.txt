# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(Connect4Tests LANGUAGES CXX)

# ---- Tests ----

add_executable(Connect4_test source/Connect4_test.cpp)
target_link_libraries(Connect4_test PRIVATE Connect4_lib)
target_compile_features(Connect4_test PRIVATE cxx_std_17)

add_test(NAME Connect4_test COMMAND Connect4_test)

# ---- End-of-file commands ----

add_folders(Test)
