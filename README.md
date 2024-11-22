# Design Notes
No C++ exceptions (emscripten limitation)
* emscripten.org portability guidelines: "Code that compiles but might run slowly"  
"C++ Exceptions. In JavaScript such code generally makes the JavaScript engine turn off various optimizations.
For that reason exceptions are turned off by default in -O1 and above. 
To re-enable them, run *emcc* with ```-sDISABLE_EXCEPTION_CATCHING=0```"
