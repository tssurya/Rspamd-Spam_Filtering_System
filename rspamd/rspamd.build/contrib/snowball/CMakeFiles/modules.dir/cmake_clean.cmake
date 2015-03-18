FILE(REMOVE_RECURSE
  "CMakeFiles/modules"
  "libstemmer/modules.h"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/modules.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
