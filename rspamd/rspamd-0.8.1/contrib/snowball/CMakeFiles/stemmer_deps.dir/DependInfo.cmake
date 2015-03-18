# The set of languages for which implicit dependencies are needed:
SET(CMAKE_DEPENDS_LANGUAGES
  )
# The set of files for implicit dependencies of each language:

# Preprocessor definitions for this target.
SET(CMAKE_TARGET_DEFINITIONS
  "HAVE_CONFIG_H"
  )

# Targets to which this target links.
SET(CMAKE_TARGET_LINKED_INFO_FILES
  )

# The include file search paths:
SET(CMAKE_C_TARGET_INCLUDE_PATH
  "src/libutil"
  "src/libserver"
  "src/libmime"
  "/usr/local/include"
  "/usr/include/glib-2.0"
  "/usr/lib/i386-linux-gnu/glib-2.0/include"
  "/usr/include/gmime-2.6"
  "compat"
  "src"
  "src/ucl/include"
  "contrib/uthash"
  "contrib/http-parser"
  "contrib/libottery"
  "contrib/xxhash"
  "contrib/snowball/include"
  "contrib/siphash"
  "contrib/blake2"
  "src/rdns/include"
  "contrib/hiredis"
  "contrib/snowball/libstemmer"
  )
SET(CMAKE_CXX_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_Fortran_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_ASM_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
