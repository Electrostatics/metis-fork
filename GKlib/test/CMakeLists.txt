# Where the header files reside
#include_directories(../)

# Build program.
add_executable(strings strings.c)
add_executable(gksort gksort.c)
add_executable(fis fis.c)
add_executable(rw rw.c)
add_executable(gkgraph gkgraph.c)
add_executable(pdb_parser pdb_parser.c)
foreach(prog strings gksort fis rw gkgraph pdb_parser)
  target_link_libraries(${prog} GKlib)
endforeach(prog)

foreach(case valid six-character long-token truncated empty)
  add_test(NAME pdb-parser-${case} COMMAND pdb_parser ${case})
endforeach(case)
