if(NOT CMAKE_CROSSCOMPILING)
  find_package(PkgConfig QUIET)
  pkg_check_modules(PC_ICUUC icu-uc)
  pkg_check_modules(PC_ICUIN icu-i18n)
endif()

# TODO: idk why but encounter dependencies so we first check the PkgConfig path
set_extra_dirs_lib(ICU icu)
find_library(ICUUC_LIBRARY
  NAMES "icuuc"
  HINTS ${PC_ICUUC_LIBDIR} ${PC_ICUUC_LIBRARY_DIRS} ${HINTS_ICU_LIBDIR}
  PATHS ${PATHS_ICU_LIBDIR}
  ${CROSSCOMPILING_NO_CMAKE_SYSTEM_PATH})

find_library(ICUIN_LIBRARY
  NAMES "icuin" "icui18n"
  HINTS ${PC_ICUIN_LIBDIR} ${PC_ICUIN_LIBRARY_DIRS} ${HINTS_ICU_LIBDIR}
  PATHS ${PATHS_ICU_LIBDIR}
  ${CROSSCOMPILING_NO_CMAKE_SYSTEM_PATH})

set_extra_dirs_include(ICU icu "${ICUUC_LIBRARY}")
find_path(ICU_INCLUDEDIR 
  NAMES "unicode/udata.h"
  HINTS ${PC_ICUUC_INCLUDEDIR} ${PC_ICUUC_INCLUDE_DIR} ${HINTS_ICU_INCLUDEDIR} 
  PATHS ${PATHS_ICU_INCLUDEDIR}
  ${CROSSCOMPILING_NO_CMAKE_SYSTEM_PATH})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ICU DEFAULT_MSG ICUUC_LIBRARY ICU_INCLUDEDIR)
mark_as_advanced(ICUUC_LIBRARY ICU_INCLUDEDIR)

if(ICU_FOUND)
  set(ICU_LIBRARIES ${ICUUC_LIBRARY} ${ICUIN_LIBRARY})
  set(ICU_INCLUDE_DIRS ${ICU_INCLUDEDIR})
  is_bundled(ICU_BUNDLED "${ICU_LIBRARIES}")
  
  set(ICU_COPY_FILES)
  if(TARGET_OS AND TARGET_OS STREQUAL "windows")
   set(ICU_COPY_FILES "${EXTRA_ICU_LIBDIR}/icudt68.dll" "${EXTRA_ICU_LIBDIR}/icuin68.dll" "${EXTRA_ICU_LIBDIR}/icuuc68.dll")
  endif()
endif()
