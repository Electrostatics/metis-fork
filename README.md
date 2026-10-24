# METIS fork

This is a fork of the [METIS](http://glaros.dtc.umn.edu/gkhome/metis/metis/download) graph partitioning software from the [Karypis group](https://cse.umn.edu/cs/george-karypis).
The current version of the forked source code is 5.1.0.
The fork is intended to provide a stable location for source code used by [APBS](https://github.com/Electrostatics/apbs) and other software packages that depend on METIS.

## PDB parser regression tests

Build the standalone GKlib tests with AddressSanitizer and UndefinedBehaviorSanitizer:

```sh
cmake -S GKlib -B build-parser \
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
  -DCMAKE_C_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer -g" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address,undefined"
cmake --build build-parser --target pdb_parser --parallel 2
ctest --test-dir build-parser --output-on-failure
```

The tests cover valid ATOM records, malformed record names, truncated fields,
and empty input. Both parsing passes bound record-name extraction and reject
malformed ATOM records through GKlib's error handler. If error termination is
disabled, the reader returns `NULL` after reporting the error. HETATM handling
is unchanged.
