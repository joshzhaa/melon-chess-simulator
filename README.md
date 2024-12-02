# Design Goals
Piece behavior is configured through data (JSON) instead of through source code
 * enables user to edit piece behavior without recompiling program

No C++ exceptions (emscripten limitation)
 * emscripten.org portability guidelines: "Code that compiles but might run slowly"  
 "C++ Exceptions. In JavaScript such code generally makes the JavaScript engine turn off various optimizations.
 For that reason exceptions are turned off by default in -O1 and above. 
 To re-enable them, run *emcc* with ```-sDISABLE_EXCEPTION_CATCHING=0```"

# Design Notes
Piece id (unsigned char, max 256):
 * 0: Empty
 * 1: K
 * 2: Q
 * 3: R
 * 4: B
 * 5: N
 * 6: P

Team id (unsigned char, max 256):
 * 0: No one (owns Empty Piece)
 * 1: white
 * 2: black

Each piece has "Traits" defined in a JSON
A Trait is global for each Piece's id

Board layout: stacked 2D ```std::vector```
standardly, white is ```std::vector[0]``` and ```std::vector[1]```
standardly, black is ```std::vector[6]``` and ```std::vector[7]```
Basically, the board is 1st quadrant Z2, which is convenient for conceptualizing ```melon::math::Vector```
However, it makes printing the 2D ```std::vector``` the usual way misleading, as the usual way implies the board is 4th quadrant Z2.
Verdict: printing is less important than the keeping the board in the 1st quadrant

# Dependencies
1. clang (dev: clangd, clang-format, clang-tidy)
2. CMake
3. nlohmann/json
4. GoogleTest

# How To
cmake lines to copy and paste
```
cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=TRUE -S . -B Release
cmake --build build -j4
```
