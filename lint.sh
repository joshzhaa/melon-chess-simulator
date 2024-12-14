# TODO: figure out how to get clang-tidy to ignore gtest
# currently filtering for .cpp works because google uses .cc but it's pretty jank
run-clang-tidy '.*\.cpp' -config-file .clang-tidy -use-color true
