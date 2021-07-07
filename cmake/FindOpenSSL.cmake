# out
# - OPENSSL_CRYPTO_LIBRARY - (libcrypto)
# - OPENSSL_SSL_LIBRARY - (libssl)
# - OPENSSL_LIBRARY - (libcrypto + libssl)

if(NOT CMAKE_CROSSCOMPILING)
  find_package(PkgConfig QUIET)
  pkg_check_modules(PC_SSL libssl)
  pkg_check_modules(PC_CRYPTO libcrypto)
endif()

# libiraries
set_extra_dirs_lib(OPENSSL openssl)
find_library(OPENSSL_SSL_LIBRARY
  NAMES "libssl"
  HINTS ${PC_SSL_LIBDIR} ${PC_SSL_LIBRARY_DIRS} ${HINTS_OPENSSL_LIBDIR}
  PATHS ${PATHS_OPENSSL_LIBDIR}
  ${CROSSCOMPILING_NO_CMAKE_SYSTEM_PATH})
find_library(OPENSSL_CRYPTO_LIBRARY
  NAMES "libcrypto"
  HINTS ${PC_CRYPTO_LIBDIR} ${PC_CRYPTO_LIBRARY_DIRS} ${HINTS_OPENSSL_LIBDIR}
  PATHS ${PATHS_OPENSSL_LIBDIR}
  ${CROSSCOMPILING_NO_CMAKE_SYSTEM_PATH})

# includes
set_extra_dirs_include(OPENSSL openssl "${OPENSSL_CRYPTO_LIBRARY}")
find_path(OPENSSL_INCLUDEDIR 
  NAMES "openssl/conf.h"
  HINTS ${PC_CRYPTO_INCLUDEDIR} ${PC_CRYPTO_INCLUDE_DIR} ${HINTS_OPENSSL_INCLUDEDIR} 
  PATHS ${PATHS_OPENSSL_INCLUDEDIR}
  ${CROSSCOMPILING_NO_CMAKE_SYSTEM_PATH})

# mark found
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenSSL DEFAULT_MSG OPENSSL_CRYPTO_LIBRARY OPENSSL_INCLUDEDIR)
mark_as_advanced(OPENSSL_CRYPTO_LIBRARY OPENSSL_INCLUDEDIR)

# so that we have the same variables for all searches
set(OPENSSL_LIBRARY)
set(OPENSSL_INCLUDE_DIRS)

if(OPENSSL_FOUND)
  # bundled
  set(OPENSSL_LIBRARY ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})
  set(OPENSSL_INCLUDE_DIRS ${OPENSSL_INCLUDEDIR})
  is_bundled(OPENSSL_BUNDLED "${OPENSSL_LIBRARY}")
  
  # copy dll files
  set(OPENSSL_COPY_FILES)
  if(TARGET_OS AND TARGET_OS STREQUAL "windows")
    if(TARGET_BITS EQUAL 32)
      set(OPENSSL_COPY_FILES "${EXTRA_OPENSSL_LIBDIR}/libcrypto-1_1.dll" "${EXTRA_OPENSSL_LIBDIR}/libssl-1_1.dll")
    elseif(TARGET_BITS EQUAL 64)
      set(OPENSSL_COPY_FILES "${EXTRA_OPENSSL_LIBDIR}/libcrypto-1_1-x64.dll" "${EXTRA_OPENSSL_LIBDIR}/libssl-1_1-x64.dll")
	endif()
	list(APPEND OPENSSL_COPY_FILES "${EXTRA_OPENSSL_LIBDIR}/ssleay32.dll" "${EXTRA_OPENSSL_LIBDIR}/libeay32.dll")
  endif()
else()
  # search original module path
  set(CMAKE_MODULE_PATH ${ORIGINAL_CMAKE_MODULE_PATH})
  find_package(OpenSSL QUIET)
  set(CMAKE_MODULE_PATH ${OWN_CMAKE_MODULE_PATH})
  
  # https://cmake.org/cmake/help/v3.6/module/FindOpenSSL.html
  if(OPENSSL_FOUND)
    set(OPENSSL_LIBRARY ${OPENSSL_LIBRARIES})
    set(OPENSSL_INCLUDE_DIRS ${OPENSSL_INCLUDE_DIR})
  else()
    set(OPENSSL_SSL_LIBRARY "")
    set(OPENSSL_CRYPTO_LIBRARY "")
  endif()
endif()