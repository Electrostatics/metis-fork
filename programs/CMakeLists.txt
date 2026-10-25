# These programs use internal metis data structures.
include_directories(../libmetis)
link_directories(/home/karypis/local/lib)
# Build program.
add_executable(gpmetis gpmetis.c cmdline_gpmetis.c io.c stat.c)
add_executable(ndmetis ndmetis.c cmdline_ndmetis.c io.c smbfactor.c)
add_executable(mpmetis mpmetis.c cmdline_mpmetis.c io.c stat.c)
add_executable(m2gmetis m2gmetis.c cmdline_m2gmetis.c io.c)
add_executable(graphchk graphchk.c io.c)
add_executable(cmpfillin cmpfillin.c io.c smbfactor.c)
foreach(prog gpmetis ndmetis mpmetis m2gmetis graphchk cmpfillin)
  target_link_libraries(${prog} metis)
#  target_link_libraries(${prog} metis profiler)
endforeach(prog)

if(METIS_BUILD_TESTS)
  add_executable(test_readers tests/test_readers.c io.c)
  target_include_directories(test_readers PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(test_readers metis)
  add_executable(test_mesh_alloc tests/test_mesh_alloc.c)
  target_link_libraries(test_mesh_alloc metis)
  add_test(NAME mesh-allocation-failure COMMAND test_mesh_alloc)
  foreach(reader graph mesh)
    foreach(case valid weighted overflow negative truncated header-range
        weight-range index-limit negative-weight)
      add_test(NAME reader-${reader}-${case}
        COMMAND ${CMAKE_COMMAND}
          -DPROGRAM=$<TARGET_FILE:test_readers> -DREADER=${reader} -DCASE=${case}
          -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/check_reader.cmake)
    endforeach()
  endforeach()
endif()

if(METIS_INSTALL)
  install(TARGETS gpmetis ndmetis mpmetis m2gmetis graphchk cmpfillin
    RUNTIME DESTINATION bin)
endif()

# Try to find subversion revision.
set(SVNREV "")
file(TO_NATIVE_PATH ${PROJECT_SOURCE_DIR}/.svn svn_dir)
if(IS_DIRECTORY ${svn_dir})
  include(FindSubversion)
  if(Subversion_FOUND)
    Subversion_WC_INFO(${PROJECT_SOURCE_DIR} metis)
    set(SVNREV ${metis_WC_REVISION})
  endif(Subversion_FOUND)
endif()
add_definitions(-DSVNINFO="${SVNREV}")
