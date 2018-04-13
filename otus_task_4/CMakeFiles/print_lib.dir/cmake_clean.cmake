file(REMOVE_RECURSE
  "libprint_lib.pdb"
  "libprint_lib.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/print_lib.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
