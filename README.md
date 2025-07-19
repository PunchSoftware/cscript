# CScript

Inspired by [hscript](https://github.com/HaxeFoundation/hscript). A light dependency that allows for scripting in C without the need for recompilation.

**NOTE: Under heavy, active development**

## Todo

+ !!! For variables it relies on type checking and having discovered a type. Instead we will set the atom in the table and retrieve an atom that tracks kind of
construct.
+ Working on a proper scheme, AST is generally setup but need to add functions, variables, etc.
    + Following that work on interpretation and the later stuff
+ Extend keywords to match that of C
+ Consider having a dynamic type system
+ Perhaps we forego pointers altogether, but that could be interesting...
