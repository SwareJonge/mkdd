# Compiler Flags
This file contains some documtation about the compiler flags that are used for each library, this might get some updates since i can't say for certain this is correct.
## General Flags
`-lang=c++,-fp fmadd, -fp_contract on, -Cpp_exceptions off, -rostr, -RTTI off, -char signed, -enum int, -use_lmw_stmw on, -common on, -inline auto**`

## Debug
### JSystem
General Flags + `-O4,s` or `-O4,p`, varies per subLibarary
### Game
General Flags + `-O4,s` sometimes combined with `-inline off`

## Release
### JSystem
General Flags + `-opt level=4, nopeep, schedule`
### Game
General Flags + `-O4,s`