# TODO: figure out how to get clang-tidy to ignore gtest
# currently removing those files from compile_command.json manually works
run-clang-tidy -config-file .clang-tidy
