# CScript

Inspired by [hscript](https://github.com/HaxeFoundation/hscript). Not a library of babylon in terms of use cases, however, this does allow for scripting without the need for recompilation.

## Todo

+ Working on a proper scheme, AST is generally setup but need to add functions, variables, etc.
    + Following that work on interpretation and the later stuff
+ Allow global variables to be accessed, either set or get, from outside the script.
+ We want to delay interpretation until that function is specifically called by user in C code.
+ Extend keywords to match that of C
+ Consider having a dynamic type system
+ Perhaps we forego pointers altogether, but that could be interesting...
