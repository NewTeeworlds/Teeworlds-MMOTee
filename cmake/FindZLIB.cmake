if(NOT PREFER_BUNDLED_LIBS)
  set(CMAKE_MODULE_PATH ${ORIGINAL_CMAKE_MODULE_PATH})
  find_package(ZLIB)
  set(CMAKE_MODULE_PATH ${OWN_CMAKE_MODULE_PATH})
  if(ZLIB_FOUND)
    set(ZLIB_BUNDLED OFF)
    set(ZLIB_DEP)
  endif()
endif()

if(NOT ZLIB_FOUND)
  set(ZLIB_BUNDLED ON)
  set(ZLIB_SRC_DIR src/engine/external/zlib)
  set_src(ZLIB_SRC GLOB ${ZLIB_SRC_DIR}
  adler32.c   
  crc32.h    
  infback.c  
  inffixed.h  
  inftrees.c  
  trees.h    
  zconf.h  
  zutil.h
  compress.c  
  deflate.c  
  inffast.c  
  inflate.c   
  inftrees.h  
  uncompr.c  
  zlib.h
  crc32.c     
  deflate.h  
  inffast.h  
  inflate.h   
  trees.c 
  zutil.c
  )
  add_library(zlib EXCLUDE_FROM_ALL OBJECT ${ZLIB_SRC})
  set(ZLIB_INCLUDEDIR ${ZLIB_SRC_DIR})
  target_include_directories(zlib PRIVATE ${ZLIB_INCLUDEDIR})

  set(ZLIB_DEP $<TARGET_OBJECTS:zlib>)
  set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDEDIR})
  set(ZLIB_LIBRARIES)

  list(APPEND TARGETS_DEP zlib)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(ZLIB DEFAULT_MSG ZLIB_INCLUDEDIR)
endif()
