# Design Notes
No C++ exceptions (emscripten limitation)
* emscripten.org portability guidelines: "Code that compiles but might run slowly"  
"C++ Exceptions. In JavaScript such code generally makes the JavaScript engine turn off various optimizations.
For that reason exceptions are turned off by default in -O1 and above. 
To re-enable them, run *emcc* with ```-sDISABLE_EXCEPTION_CATCHING=0```"

Piece id (unsigned char, max 256):
 * 0: K
 * 1: Q
 * 2: R
 * 3: B
 * 4: N
 * 5: P
 * 6: Empty

Team id (unsigned char, max 256):
 * 0: No one owns Empty Piece
 * 1: white
 * 2: black

 Each piece has "traits" defined in a JSON
