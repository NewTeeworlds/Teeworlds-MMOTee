cmake_minimum_required(VERSION 2.8.12...3.19.1)
if(CMAKE_VERSION VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_VERSION})
endif()

file(STRINGS src/game/version.h VERSION_LINE
  LIMIT_COUNT 1
  REGEX "^#define GAME_VERSION "
)

set(PROJECT_NAME "Mmotee")
if(VERSION_LINE MATCHES "\"([0-9]+)\\.([0-9]+)\\.([0-9]+)\"")
  set(VERSION_MAJOR ${CMAKE_MATCH_1})
  set(VERSION_MINOR ${CMAKE_MATCH_2})
  set(VERSION_PATCH ${CMAKE_MATCH_3})
elseif(VERSION_LINE MATCHES "\"([0-9]+)\\.([0-9]+)\"")
  set(VERSION_MAJOR ${CMAKE_MATCH_1})
  set(VERSION_MINOR ${CMAKE_MATCH_2})
  set(VERSION_PATCH "0")
else()
  message(FATAL_ERROR "Couldn't parse version from src/game/version.h")
endif()

# Extra support for CMake pre-3.0
if(NOT POLICY CMP0048)
  set(PROJECT_VERSION_MAJOR ${VERSION_MAJOR})
  set(PROJECT_VERSION_MINOR ${VERSION_MINOR})
  set(PROJECT_VERSION_PATCH ${VERSION_PATCH})
  if(VERSION_PATCH STREQUAL "0")
    set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR})
  else()
    set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
  endif()
endif()

project(${PROJECT_NAME} VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(ORIGINAL_CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})
set(ORIGINAL_CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES})
set(ORIGINAL_CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES})
set(OWN_CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${OWN_CMAKE_MODULE_PATH})

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(TARGET_BITS "32")
else()
  set(TARGET_BITS "64")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(TARGET_OS "windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(TARGET_OS "linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  message(SEND_ERROR "MacOS support is off for better times..")
endif()

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckSymbolExists)

check_symbol_exists(__i386 "" TARGET_ARCH_X86_i386)
if(TARGET_ARCH_X86_i386)
  set(TARGET_ARCH x86)
else()
  set(TARGET_ARCH)
endif()

set(AUTO_DEPENDENCIES_DEFAULT OFF)
if(TARGET_OS STREQUAL "windows")
  set(AUTO_DEPENDENCIES_DEFAULT ON)
endif()

option(PREFER_BUNDLED_LIBS "Prefer bundled libraries over system libraries" ${AUTO_DEPENDENCIES_DEFAULT})
option(DEV "Don't generate stuff necessary for packaging" OFF)

# Set version if not explicitly set
if(NOT VERSION)
  set(VERSION ${PROJECT_VERSION})
endif()

set(OpenGL_GL_PREFERENCE LEGACY)

# Set the default build type to Release
if(NOT(CMAKE_BUILD_TYPE))
  if(NOT(DEV))
    set(CMAKE_BUILD_TYPE Release)
  else()
    set(CMAKE_BUILD_TYPE Debug)
  endif()
endif()

if(NOT(CMAKE_BUILD_TYPE MATCHES "^(Release|Debug|RelWithDebInfo|MinSizeRel)$"))
  message(WARNING "Unknown CMAKE_BUILD_TYPE, should be one of Release, Debug, RelWithDebInfo or MinSizeRel")
endif()

set(DBG $<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>)

if(CMAKE_VERSION VERSION_LESS 3.0)
  configure_file(src/game/version.h vd.h)
else()
  set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    src/game/version.h
  )
endif()

set(SERVER_EXECUTABLE ${PROJECT_NAME}-Server CACHE STRING "Name of the built server executable")

########################################################################
# Compiler flags
########################################################################
function(add_c_compiler_flag_if_supported VARIABLE FLAG)
  if(ARGC GREATER 2)
    set(CHECKED_FLAG "${ARGV2}")
  else()
    set(CHECKED_FLAG "${FLAG}")
  endif()
  string(REGEX REPLACE "[^A-Za-z0-9]" "_" CONFIG_VARIABLE "FLAG_SUPPORTED${CHECKED_FLAG}")
  check_c_compiler_flag("${CHECKED_FLAG}" ${CONFIG_VARIABLE})
  if(${CONFIG_VARIABLE})
    if(${VARIABLE})
      set("${VARIABLE}" "${${VARIABLE}};${FLAG}" PARENT_SCOPE)
    else()
      set("${VARIABLE}" "${FLAG}" PARENT_SCOPE)
    endif()
  endif()
endfunction()

function(add_cxx_compiler_flag_if_supported VARIABLE FLAG)
  if(ARGC GREATER 2)
    set(CHECKED_FLAG "${ARGV2}")
  else()
    set(CHECKED_FLAG "${FLAG}")
  endif()
  string(REGEX REPLACE "[^A-Za-z0-9]" "_" CONFIG_VARIABLE "FLAG_SUPPORTED${CHECKED_FLAG}")
  check_cxx_compiler_flag("${CHECKED_FLAG}" ${CONFIG_VARIABLE})
  if(${CONFIG_VARIABLE})
    if(${VARIABLE})
      set("${VARIABLE}" "${${VARIABLE}} ${FLAG}" PARENT_SCOPE)
    else()
      set("${VARIABLE}" "${FLAG}" PARENT_SCOPE)
    endif()
  endif()
endfunction()

# Force compiler colors on when using ninja. Ninja filters the colors out when
# it's not printing to a terminal on its own.
if(CMAKE_GENERATOR STREQUAL "Ninja")
  add_c_compiler_flag_if_supported(OUR_FLAGS -fdiagnostics-color=always)
  add_c_compiler_flag_if_supported(OUR_FLAGS -fcolor-diagnostics)
endif()

if(NOT MSVC)
  # Protect the stack pointer.
  # -fstack-protector-all doesn't work on MinGW.
  add_c_compiler_flag_if_supported(OUR_FLAGS -fstack-protector-all)

  # Inaccurate floating point numbers cause problems on mingw-w64-gcc when
  # compiling for x86, might cause problems elsewhere. So don't store floats
  # in registers but keep them at higher accuracy.
  if(TARGET_ARCH STREQUAL "x86")
    add_c_compiler_flag_if_supported(OUR_FLAGS -ffloat-store)
  endif()

  # Don't insert timestamps into PEs to keep the build reproducible.
  if(TARGET_OS STREQUAL "windows")
    add_c_compiler_flag_if_supported(OUR_FLAGS_LINK -Wl,--no-insert-timestamp)
  endif()

  if(TARGET_OS STREQUAL "mac")
    add_c_compiler_flag_if_supported(OUR_FLAGS -stdlib=libc++)
  endif()

  add_c_compiler_flag_if_supported(OUR_FLAGS_OWN -Wall)
  if(CMAKE_VERSION VERSION_GREATER 3.3 OR CMAKE_VERSION VERSION_EQUAL 3.3)
    add_c_compiler_flag_if_supported(OUR_FLAGS_OWN
      $<$<COMPILE_LANGUAGE:C>:-Wdeclaration-after-statement>
      -Wdeclaration-after-statement
    )
  endif()

  add_c_compiler_flag_if_supported(OUR_FLAGS_OWN -Wextra)
  add_c_compiler_flag_if_supported(OUR_FLAGS_OWN -Wno-unused-parameter)
  add_c_compiler_flag_if_supported(OUR_FLAGS_OWN -Wno-missing-field-initializers)
  add_c_compiler_flag_if_supported(OUR_FLAGS_OWN -Wformat=2) # Warn about format strings.
  add_c_compiler_flag_if_supported(OUR_FLAGS_OWN -Wno-nullability-completeness) # Mac OS build on github
  add_c_compiler_flag_if_supported(OUR_FLAGS_OWN -Wduplicated-cond)
  add_c_compiler_flag_if_supported(OUR_FLAGS_OWN -Wduplicated-branches)
  add_c_compiler_flag_if_supported(OUR_FLAGS_OWN -Wlogical-op)
  add_c_compiler_flag_if_supported(OUR_FLAGS_OWN -Wrestrict)
  add_c_compiler_flag_if_supported(OUR_FLAGS_OWN -Wno-cast-function-type)

  # TODO: Enable for C++ code except gtest
  #add_cxx_compiler_flag_if_supported(OUR_FLAGS_OWN "-Wuseless-cast")
  add_cxx_compiler_flag_if_supported(OUR_CXX_FLAGS_OWN -Wno-deprecated-copy)
  add_cxx_compiler_flag_if_supported(OUR_CXX_FLAGS_OWN -Wno-ignored-qualifiers)
  add_cxx_compiler_flag_if_supported(OUR_CXX_FLAGS_OWN -Wno-cast-function-type)
  add_cxx_compiler_flag_if_supported(OUR_CXX_FLAGS_OWN -std=gnu++11)

  if(OUR_CXX_FLAGS_OWN)
    if(CMAKE_CXX_FLAGS)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OUR_CXX_FLAGS_OWN}")
    else()
      set(CMAKE_CXX_FLAGS ${OUR_CXX_FLAGS_OWN})
    endif()
  endif()
endif()

if(NOT MSVC)
  check_c_compiler_flag("-O2;-Wp,-Werror;-D_FORTIFY_SOURCE=2" DEFINE_FORTIFY_SOURCE) # Some distributions define _FORTIFY_SOURCE by themselves.
endif()

########################################################################
# COMMON FUNCTIONS
########################################################################
function(set_glob VAR GLOBBING EXTS DIRECTORY) # ...
  set(GLOBS)
  foreach(ext ${EXTS})
    list(APPEND GLOBS "${DIRECTORY}/*.${ext}")
  endforeach()
  file(${GLOBBING} GLOB_RESULT ${GLOBS})
  list(SORT GLOB_RESULT)
  set(FILES)
  foreach(file ${ARGN})
    list(APPEND FILES "${PROJECT_SOURCE_DIR}/${DIRECTORY}/${file}")
  endforeach()

  if(NOT FILES STREQUAL GLOB_RESULT)
    message(AUTHOR_WARNING "${VAR} does not contain every file from directory ${DIRECTORY}")
    set(LIST_BUT_NOT_GLOB)
    if(POLICY CMP0057)
      cmake_policy(SET CMP0057 NEW)
      foreach(file ${FILES})
        if(NOT file IN_LIST GLOB_RESULT)
          list(APPEND LIST_BUT_NOT_GLOB ${file})
        endif()
      endforeach()
      if(LIST_BUT_NOT_GLOB)
        message(AUTHOR_WARNING "Entries only present in ${VAR}: ${LIST_BUT_NOT_GLOB}")
      endif()
      set(GLOB_BUT_NOT_LIST)
      foreach(file ${GLOB_RESULT})
        if(NOT file IN_LIST FILES)
          list(APPEND GLOB_BUT_NOT_LIST ${file})
        endif()
      endforeach()
      if(GLOB_BUT_NOT_LIST)
        message(AUTHOR_WARNING "Entries only present in ${DIRECTORY}: ${GLOB_BUT_NOT_LIST}")
      endif()
      if(NOT LIST_BUT_NOT_GLOB AND NOT GLOB_BUT_NOT_LIST)
        message(AUTHOR_WARNING "${VAR} is not alphabetically sorted")
      endif()
    endif()
  endif()

  set(${VAR} ${FILES} PARENT_SCOPE)
endfunction()

function(set_src VAR GLOBBING DIRECTORY) # ...
  set_glob(${VAR} ${GLOBBING} "c;cpp;h" ${DIRECTORY} ${ARGN})
  set(${VAR} ${${VAR}} PARENT_SCOPE)
endfunction()

########################################################################
# INITIALIZE TARGET LISTS
########################################################################
set(TARGETS_OWN)
set(TARGETS_DEP)
set(TARGETS_LINK)

########################################################################
# DEPENDENCIES
########################################################################
function(set_extra_dirs_lib VARIABLE NAME)
  set("PATHS_${VARIABLE}_LIBDIR" PARENT_SCOPE)
  set("HINTS_${VARIABLE}_LIBDIR" PARENT_SCOPE)
  if(PREFER_BUNDLED_LIBS)
    set(TYPE HINTS)
  else()
    set(TYPE PATHS)
  endif()
  if(TARGET_BITS AND TARGET_OS)
    set(DIR "libraries/${NAME}/${TARGET_OS}/lib${TARGET_BITS}")
    set("${TYPE}_${VARIABLE}_LIBDIR" "${DIR}" PARENT_SCOPE)
    set("EXTRA_${VARIABLE}_LIBDIR" "${DIR}" PARENT_SCOPE)
  endif()
endfunction()

function(set_extra_dirs_include VARIABLE NAME LIBRARY)
  set("PATHS_${VARIABLE}_INCLUDEDIR" PARENT_SCOPE)
  set("HINTS_${VARIABLE}_INCLUDEDIR" PARENT_SCOPE)
  is_bundled(IS_BUNDLED "${LIBRARY}")
  if(IS_BUNDLED)
    set("HINTS_${VARIABLE}_INCLUDEDIR" "libraries/${NAME}/include" "libraries/${NAME}/include/${TARGET_OS}" PARENT_SCOPE)
  endif()
endfunction()

if(CMAKE_CROSSCOMPILING)
  set(CROSSCOMPILING_NO_CMAKE_SYSTEM_PATH NO_CMAKE_SYSTEM_PATH)
else()
  set(CROSSCOMPILING_NO_CMAKE_SYSTEM_PATH)
endif()

function(is_bundled VARIABLE PATH)
  if(PATH)
    string(FIND "${PATH}" "${PROJECT_SOURCE_DIR}" LOCAL_PATH_POS)
    if(LOCAL_PATH_POS EQUAL 0 AND TARGET_BITS AND TARGET_OS)
      set("${VARIABLE}" ON PARENT_SCOPE)
    else()
      set("${VARIABLE}" OFF PARENT_SCOPE)
    endif()
  else()
    set("${VARIABLE}" OFF PARENT_SCOPE)
  endif()
endfunction()

if(NOT CMAKE_CROSSCOMPILING)
  # Check for PkgConfig once so all the other `find_package` calls can do it quietly.
  find_package(PkgConfig)
endif()
find_package(ZLIB)
find_package(OpenSSL)
find_package(ICU)
find_package(MySQL)
find_package(PythonInterp 3)
find_package(Threads)

if(TARGET_OS AND TARGET_OS STREQUAL "linux")
  find_package(Boost 1.36.0)
endif()

message(STATUS "******** ${PROJECT_NAME} ********")
message(STATUS "Target OS: ${TARGET_OS} ${TARGET_BITS}bit")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Dependencies:")
function(show_dependency_status OUTPUT_NAME NAME)
  if(${NAME}_FOUND)
    if(${NAME}_BUNDLED)
      message(STATUS " * ${OUTPUT_NAME} not found (using bundled version)")
    else()
      message(STATUS " * ${OUTPUT_NAME} found")
    endif()
  else()
    message(STATUS " * ${OUTPUT_NAME} not found")
  endif()
endfunction()

show_dependency_status("Curl" CURL)
show_dependency_status("MySQL" MYSQL)
show_dependency_status("OpenSSL" OPENSSL)
show_dependency_status("PythonInterp" PYTHONINTERP)
show_dependency_status("Zlib" ZLIB)
show_dependency_status("ICU" ICU)

if(NOT(PYTHONINTERP_FOUND))
  message(SEND_ERROR "You must install Python to compile the ${PROJECT_NAME}")
endif()
if(NOT(MYSQL_FOUND))
  message(SEND_ERROR "You must install MySQL to compile the ${PROJECT_NAME}")
endif()
if(NOT(ICU_FOUND))
  message(SEND_ERROR "You must install ICU to compile Teeworlds")
endif()

#if(NOT(OPUS_FOUND))
#  message(SEND_ERROR "You must install Opus to compile")
#endif()
#if(NOT(OPUSFILE_FOUND))
#  message(SEND_ERROR "You must install Opusfile to compile")
#endif()

if(TARGET_OS STREQUAL "windows")
  set(PLATFORM_LIBS ws2_32) # Windows sockets
elseif(TARGET_OS STREQUAL "linux")
  set(PLATFORM_LIBS rt) # clock_gettime for glibc < 2.17
else()
  set(PLATFORM_LIBS)
endif()

########################################################################
# DEPENDENCY COMPILATION
########################################################################
set_src(DEP_JSON_SRC GLOB src/engine/external/json-parser json.c json.h)
add_library(json EXCLUDE_FROM_ALL OBJECT ${DEP_JSON_SRC})
set_src(DEP_MD5_SRC GLOB src/engine/external/md5 md5.c md5.h)
add_library(md5 EXCLUDE_FROM_ALL OBJECT ${DEP_MD5_SRC})

list(APPEND TARGETS_DEP json md5)
set(DEP_JSON $<TARGET_OBJECTS:json>)
set(DEP_MD5 $<TARGET_OBJECTS:md5>)

########################################################################
# COPY DATA AND DLLS
########################################################################
file(COPY server_lang DESTINATION .)

set(COPY_FILES
  ${ICU_COPY_FILES}
  ${MYSQL_COPY_FILES}
  ${CURL_COPY_FILES}
  ${OPENSSL_COPY_FILES}
  ${OPUSFILE_COPY_FILES}
)
file(COPY ${COPY_FILES} DESTINATION .)

########################################################################
# CODE GENERATION
########################################################################
function(chash output_file)
  add_custom_command(OUTPUT ${output_file}
    COMMAND ${PYTHON_EXECUTABLE} scripts/cmd5.py ${ARGN}
      > "${PROJECT_BINARY_DIR}/${output_file}"
    DEPENDS scripts/cmd5.py ${ARGN}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
endfunction()

function(generate_source output_file script_parameter)
  add_custom_command(OUTPUT ${output_file}
    COMMAND ${PYTHON_EXECUTABLE} datasrc/compile.py ${script_parameter}
      > "${PROJECT_BINARY_DIR}/${output_file}"
    DEPENDS
      datasrc/compile.py
      datasrc/content.py
      datasrc/datatypes.py
      datasrc/network.py
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
endfunction()

function(generate_source7 output_file script_parameter)
  add_custom_command(OUTPUT ${output_file}
    COMMAND ${PYTHON_EXECUTABLE} -m datasrc.seven.compile ${script_parameter}
      > "${PROJECT_BINARY_DIR}/${output_file}"
    DEPENDS
      datasrc/seven/compile.py
      datasrc/seven/content.py
      datasrc/seven/datatypes.py
      datasrc/seven/network.py
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
endfunction()

function(generate_maps output_file)
  add_custom_command(OUTPUT ${output_file}
    COMMAND ${PYTHON_EXECUTABLE} datasrc/crosscompile.py
      > "${PROJECT_BINARY_DIR}/${output_file}"
    DEPENDS
      datasrc/compile.py
      datasrc/content.py
      datasrc/datatypes.py
      datasrc/network.py
      datasrc/seven/compile.py
      datasrc/seven/content.py
      datasrc/seven/datatypes.py
      datasrc/seven/network.py
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
endfunction()

file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/src/game/generated/")
execute_process(COMMAND git rev-parse --git-dir
  ERROR_QUIET
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
  OUTPUT_VARIABLE PROJECT_GIT_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
  RESULT_VARIABLE PROJECT_GIT_DIR_ERROR
)
if(NOT PROJECT_GIT_DIR_ERROR)
  set(GIT_REVISION_EXTRA_DEPS
    ${PROJECT_GIT_DIR}/index
    ${PROJECT_GIT_DIR}/logs/HEAD
  )
endif()
add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/src/game/generated/git_revision.cpp
  COMMAND ${PYTHON_EXECUTABLE}
    scripts/git_revision.py
    > ${PROJECT_BINARY_DIR}/src/game/generated/git_revision.cpp
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  DEPENDS
    ${GIT_REVISION_EXTRA_DEPS}
    scripts/git_revision.py
)

chash("src/game/generated/nethash.cpp"
  "src/engine/shared/protocol.h"
  "src/game/tuning.h"
  "src/game/gamecore.cpp"
  "${PROJECT_BINARY_DIR}/src/game/generated/protocol.h"
)
generate_source("src/game/generated/protocol.cpp" "network_source")
generate_source("src/game/generated/protocol.h" "network_header")
generate_source("src/game/generated/server_data.cpp" "server_content_source")
generate_source("src/game/generated/server_data.h" "server_content_header")

########################################################################
# SHARED
########################################################################
# Sources
file(GLOB_RECURSE BASE "src/base/*.cpp" "src/base/*.c" "src/base/*.h")
file(GLOB ENGINE_INTERFACE "src/engine/*.h")
file(GLOB ENGINE_SHARED "src/engine/shared/*.cpp" "src/engine/shared/*.h")
file(GLOB GAME_SHARED "src/game/*.cpp" "src/game/*.h")

set(ENGINE_GENERATED_SHARED 
  src/game/generated/nethash.cpp 
  src/game/generated/protocol.cpp 
  src/game/generated/protocol.h
)
set(GAME_GENERATED_SHARED
  src/game/generated/git_revision.cpp
  src/game/generated/nethash.cpp
  src/game/generated/protocol.h
)

set(DEPS ${DEP_JSON} ${DEP_MD5} ${ZLIB_DEP})

# Libraries
set(LIBS ${OPENSSL_CRYPTO_LIBRARY} ${ZLIB_LIBRARIES} ${PLATFORM_LIBS} ${CMAKE_THREAD_LIBS_INIT})

# Targets  
add_library(engine-shared EXCLUDE_FROM_ALL OBJECT ${ENGINE_INTERFACE} ${ENGINE_GENERATED_SHARED} ${ENGINE_SHARED} ${BASE})
add_library(game-shared EXCLUDE_FROM_ALL OBJECT ${GAME_SHARED} ${GAME_GENERATED_SHARED})
target_compile_definitions(engine-shared PRIVATE SERVER_EXEC="${PROJECT_NAME}")
list(APPEND TARGETS_OWN engine-shared game-shared)

########################################################################
# SERVER
########################################################################

# Sources
file(GLOB_RECURSE ENGINE_SERVER "src/engine/server/*.cpp" "src/engine/server/*.h")
file(GLOB_RECURSE GAME_SERVER "src/game/server/*.cpp" "src/game/server/*.h")
file(GLOB_RECURSE TEEOTHER_SERVER "src/teeuniverses/*.cpp" "src/teeuniverses/*.h")
set(GAME_GENERATED_SERVER "src/game/generated/server_data.cpp" "src/game/generated/server_data.h")
set(SERVER_SRC ${ENGINE_SERVER} ${GAME_SERVER} ${TEEOTHER_SERVER} ${GAME_GENERATED_SERVER})

set(SERVER_ICON)
if(TARGET_OS STREQUAL "windows")
  set(SERVER_ICON "other/icons/Teeworlds-Server.rc")
endif()

# Target
set(TARGET_SERVER ${SERVER_EXECUTABLE})
add_executable(${TARGET_SERVER}
  ${DEPS}
  ${SERVER_SRC}
  ${SERVER_ICON}
  $<TARGET_OBJECTS:engine-shared>
  $<TARGET_OBJECTS:game-shared>
)

# Libraries
set(LIBS_SERVER
  ${LIBS}
  ${MYSQL_LIBRARIES}
  ${ICU_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

# Includes
if(NOT(Boost_FOUND))
  set(Boost_INCLUDE_DIRS "libraries/boost/include")
endif()
set(INCLUDES_SERVER
  ${MYSQL_INCLUDE_DIRS}
  ${ICU_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

target_include_directories(${TARGET_SERVER} PRIVATE ${INCLUDES_SERVER})
target_link_libraries(${TARGET_SERVER} ${LIBS_SERVER})
target_compile_definitions(${TARGET_SERVER} PRIVATE GAMEMODE_NAME="${PROJECT_NAME}")
list(APPEND TARGETS_OWN ${TARGET_SERVER})
list(APPEND TARGETS_LINK ${TARGET_SERVER})

########################################################################
# VARIOUS TARGETS
########################################################################
set_src(MASTERSRV_SRC GLOB src/mastersrv mastersrv.cpp mastersrv.h)	

set(TARGET_MASTERSRV mastersrv)
add_executable(${TARGET_MASTERSRV} EXCLUDE_FROM_ALL ${MASTERSRV_SRC} $<TARGET_OBJECTS:engine-shared> ${DEPS})	
target_link_libraries(${TARGET_MASTERSRV} ${LIBS})	

list(APPEND TARGETS_OWN ${TARGET_MASTERSRV})
list(APPEND TARGETS_LINK ${TARGET_MASTERSRV})
add_custom_target(everything DEPENDS ${TARGETS_OWN})

########################################################################
# INSTALLATION
########################################################################
function(escape_regex VAR STRING)
  string(REGEX REPLACE "([][^$.+*?|()\\\\])" "\\\\\\1" ESCAPED "${STRING}")
  set(${VAR} ${ESCAPED} PARENT_SCOPE)
endfunction()

function(escape_backslashes VAR STRING)
  string(REGEX REPLACE "\\\\" "\\\\\\\\" ESCAPED "${STRING}")
  set(${VAR} ${ESCAPED} PARENT_SCOPE)
endfunction()

function(max_length VAR)
  set(MAX_LENGTH 0)
  foreach(str ${ARGN})
    string(LENGTH ${str} LENGTH)
    if(LENGTH GREATER MAX_LENGTH)
      set(MAX_LENGTH ${LENGTH})
    endif()
  endforeach()
  set(${VAR} ${MAX_LENGTH} PARENT_SCOPE)
endfunction()

# Tries to generate a list of regex that matches everything except the given
# parameters.
function(regex_inverted VAR)
  max_length(MAX_LENGTH ${ARGN})
  math(EXPR UPPER_BOUND "${MAX_LENGTH}-1")

  set(REMAINING ${ARGN})
  set(RESULT)

  foreach(i RANGE ${UPPER_BOUND})
    set(TEMP ${REMAINING})
    set(REMAINING)
    foreach(str ${TEMP})
      string(LENGTH ${str} LENGTH)
      if(i LESS LENGTH)
        list(APPEND REMAINING ${str})
      endif()
    endforeach()

    set(ADDITIONAL)
    foreach(outer ${REMAINING})
      string(SUBSTRING ${outer} 0 ${i} OUTER_PREFIX)
      set(CHARS "")
      foreach(inner ${REMAINING})
        string(SUBSTRING ${inner} 0 ${i} INNER_PREFIX)
        if(OUTER_PREFIX STREQUAL INNER_PREFIX)
          string(SUBSTRING ${inner} ${i} 1 INNER_NEXT)
          set(CHARS "${CHARS}${INNER_NEXT}")
        endif()
      endforeach()
      escape_regex(OUTER_PREFIX_ESCAPED "${OUTER_PREFIX}")

      list(APPEND ADDITIONAL "${OUTER_PREFIX_ESCAPED}([^${CHARS}]|$)")
    endforeach()
    list(REMOVE_DUPLICATES ADDITIONAL)
    list(APPEND RESULT ${ADDITIONAL})
  endforeach()
  set(${VAR} ${RESULT} PARENT_SCOPE)
endfunction()

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_GENERATOR TGZ TXZ)
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_STRIP_FILES TRUE)
set(CPACK_COMPONENTS_ALL portable)
set(CPACK_SOURCE_GENERATOR ZIP TGZ TBZ2 TXZ)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})

if(TARGET_OS AND TARGET_BITS)
  if(TARGET_OS STREQUAL "windows")
    set(CPACK_SYSTEM_NAME "win${TARGET_BITS}")
    set(CPACK_GENERATOR ZIP)
  elseif(TARGET_OS STREQUAL "linux")
    # Let compiler tell its arch
    # Both gcc and clang support -dumpmachine
    execute_process(
      COMMAND ${CMAKE_C_COMPILER} -dumpmachine
      OUTPUT_VARIABLE ARCHITECTURE_TUPLE
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT ARCHITECTURE_TUPLE)
      # If you're really using a weird compiler, then assume Intel here.
      message(WARNING "Your compiler doesn't support -dumpmachine, this is weird")
      if(TARGET_BITS EQUAL 32)
        set(ARCHITECTURE "x86")
      elseif(TARGET_BITS EQUAL 64)
        set(ARCHITECTURE "x86_64")
      endif()
    else()
      string(REGEX MATCH "^[^-]*" ARCHITECTURE "${ARCHITECTURE_TUPLE}")
      if(ARCHITECTURE MATCHES "i.86")
        set(ARCHITECTURE "x86")
      endif()
    endif()
    set(CPACK_SYSTEM_NAME "linux_${ARCHITECTURE}")
  endif()
endif()

set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME})
set(CPACK_ARCHIVE_PORTABLE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME})
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-src)
set(CPACK_SOURCE_FILES
  CMakeLists.txt
  README.md
  cmake/
  datasrc/
  libraries/
  license.txt
  other/
  scripts/
  src/
  storage.cfg
)
set(CPACK_SOURCE_IGNORE_FILES
  "\\\\.pyc$"
  "/\\\\.git"
  "/__pycache__/"
)

regex_inverted(CPACK_SOURCE_FILES_INVERTED ${CPACK_SOURCE_FILES})
escape_regex(PROJECT_SOURCE_DIR_ESCAPED ${PROJECT_SOURCE_DIR})

foreach(str ${CPACK_SOURCE_FILES_INVERTED})
  escape_backslashes(STR_ESCAPED "${PROJECT_SOURCE_DIR_ESCAPED}/${str}")
  list(APPEND CPACK_SOURCE_IGNORE_FILES "${STR_ESCAPED}")
endforeach()

set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME ${PROJECT_NAME})

set(CPACK_TARGETS
  ${TARGET_SERVER}
)

set(CPACK_FILES
  license.txt
  storage.cfg
  ${COPY_FILES}
)
if(TARGET_OS STREQUAL "windows")
  list(APPEND CPACK_FILES other/config_directory.bat)
endif()

if(NOT DEV)
  include(GNUInstallDirs)
  install(TARGETS ${TARGET_SERVER} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT server)
  install(TARGETS ${TARGETS_TOOLS} DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME} COMPONENT tools)
endif()

if(DEV)
  # Don't generate CPack targets.
elseif(CMAKE_VERSION VERSION_LESS 3.6 OR CMAKE_VERSION VERSION_EQUAL 3.6)
  message(WARNING "Cannot create CPack targets, CMake version too old. Use CMake 3.6 or newer.")
else()
  set(EXTRA_ARGS DESTINATION ${CPACK_PACKAGE_FILE_NAME} COMPONENT portable EXCLUDE_FROM_ALL)
  install(TARGETS ${CPACK_TARGETS} ${EXTRA_ARGS})
  install(DIRECTORY ${CPACK_DIRS} ${EXTRA_ARGS})
  install(FILES ${CPACK_FILES} ${EXTRA_ARGS})
endif()

set(PACKAGE_TARGETS)
foreach(ext zip tar.gz tar.xz)
  set(TAR_MODE c)
  set(TAR_EXTRA_ARGS)
  string(REPLACE . _ EXT_SLUG ${ext})

  set(TMPDIR pack_${CPACK_PACKAGE_FILE_NAME}_${EXT_SLUG}/${CPACK_PACKAGE_FILE_NAME})

  set(COPY_FILE_COMMANDS)
  set(COPY_DIR_COMMANDS)
  set(COPY_TARGET_COMMANDS)
  set(STRIP_TARGET_COMMANDS)
  foreach(file ${CPACK_FILES})
    list(APPEND COPY_FILE_COMMANDS COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/${file} ${TMPDIR}/)
  endforeach()
  foreach(dir ${CPACK_DIRS})
    get_filename_component(NAME ${dir} NAME)
    list(APPEND COPY_DIR_COMMANDS COMMAND ${CMAKE_COMMAND} -E copy_directory ${dir} ${TMPDIR}/${NAME})
  endforeach()
  foreach(target ${CPACK_TARGETS})
    list(APPEND COPY_TARGET_COMMANDS COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${target}> ${TMPDIR}/)
  endforeach()

  if(ext STREQUAL zip)
    set(TAR_EXTRA_ARGS --format=zip)
  elseif(ext STREQUAL tar.gz)
    set(TAR_MODE cz)
  elseif(ext STREQUAL tar.xz)
    set(TAR_MODE cJ)
  endif()
  add_custom_command(OUTPUT ${CPACK_PACKAGE_FILE_NAME}.${ext}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${TMPDIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${TMPDIR}
    ${COPY_FILE_COMMANDS}
    ${COPY_DIR_COMMANDS}
    ${COPY_TARGET_COMMANDS}
    ${STRIP_TARGET_COMMANDS}
    COMMAND ${CMAKE_COMMAND} -E chdir pack_${CPACK_PACKAGE_FILE_NAME}_${EXT_SLUG} ${CMAKE_COMMAND} -E tar ${TAR_MODE} ../${CPACK_PACKAGE_FILE_NAME}.${ext} ${TAR_EXTRA_ARGS} -- ${CPACK_PACKAGE_FILE_NAME}/
    DEPENDS ${CPACK_TARGETS}
  )
  add_custom_target(package_${EXT_SLUG} DEPENDS ${CPACK_PACKAGE_FILE_NAME}.${ext})
  list(APPEND PACKAGE_TARGETS package_${EXT_SLUG})
endforeach()

set(PACKAGE_DEFAULT tar_xz)
if(TARGET_OS STREQUAL "windows")
  set(PACKAGE_DEFAULT zip)
elseif(TARGET_OS STREQUAL "mac")
  set(PACKAGE_DEFAULT dmg)
endif()
add_custom_target(package_default DEPENDS package_${PACKAGE_DEFAULT})
add_custom_target(package_all DEPENDS ${PACKAGE_TARGETS})

# Unset these variables, they might do something in the future of CPack.
unset(CPACK_SOURCE_FILES)
unset(CPACK_SOURCE_FILES_INVERTED)
unset(CPACK_TARGETS)
unset(CPACK_DIRS)
unset(CPACK_FILES)

include(CPack)

########################################################################
# COMPILER-SPECIFICS
########################################################################

# In the future (CMake 3.8.0+), use source_group(TREE ...)
macro(source_group_tree dir)
  file(GLOB ents RELATIVE ${PROJECT_SOURCE_DIR}/${dir} ${PROJECT_SOURCE_DIR}/${dir}/*)
  foreach(ent ${ents})
    if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${dir}/${ent})
      source_group_tree(${dir}/${ent})
    else()
      string(REPLACE "/" "\\" group ${dir})
      source_group(${group} FILES ${PROJECT_SOURCE_DIR}/${dir}/${ent})
    endif()
  endforeach()
endmacro()
source_group_tree(src)


set(TARGETS ${TARGETS_OWN} ${TARGETS_DEP})

foreach(target ${TARGETS})
  if(MSVC)
    target_compile_options(${target} PRIVATE $<$<NOT:${DBG}>:/MT> $<${DBG}:/MTd>) # Use static CRT
    target_compile_options(${target} PRIVATE /MP) # Use multiple cores
    target_compile_options(${target} PRIVATE /EHsc) # Only catch C++ exceptions with catch.
    target_compile_options(${target} PRIVATE /GS) # Protect the stack pointer.
    target_compile_options(${target} PRIVATE /wd4996) # Use of non-_s functions.
  endif()
  if(OUR_FLAGS_LINK)
    target_link_libraries(${target} ${OUR_FLAGS_LINK})
  endif()
  if(OUR_FLAGS)
    target_compile_options(${target} PRIVATE ${OUR_FLAGS})
  endif()
  if(DEFINE_FORTIFY_SOURCE)
    if(MINGW)
      target_compile_definitions(${target} PRIVATE $<$<NOT:$<CONFIG:Debug>>:_FORTIFY_SOURCE=0>) # Currently broken in MinGW, see https://sourceforge.net/p/mingw-w64/discussion/723798/thread/b9d24f041f/
    else()
      target_compile_definitions(${target} PRIVATE $<$<NOT:$<CONFIG:Debug>>:_FORTIFY_SOURCE=2>) # Detect some buffer overflows.
    endif()
  endif()
endforeach()

foreach(target ${TARGETS_LINK})
  if(MSVC)
    set_property(TARGET ${target} APPEND PROPERTY LINK_FLAGS /SAFESEH:NO) # Disable SafeSEH because the shipped libraries don't support it (would cause error LNK2026 otherwise).
  endif()
  if((MINGW OR TARGET_OS STREQUAL "linux") AND PREFER_BUNDLED_LIBS)
    # Statically link the standard libraries with on MinGW/Linux so we don't
    # have to ship them as DLLs.
    target_link_libraries(${target} -static-libgcc)
    target_link_libraries(${target} -static-libstdc++)
  endif()
endforeach()

foreach(target ${TARGETS_OWN})
  if(CMAKE_VERSION VERSION_GREATER 3.1 OR CMAKE_VERSION VERSION_EQUAL 3.1)
    set_property(TARGET ${target} PROPERTY CXX_STANDARD 11)
    set_property(TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON)
  endif()

  if(MSVC)
    target_compile_options(${target} PRIVATE /wd4244) # Possible loss of data (float -> int, int -> float, etc.).
    target_compile_options(${target} PRIVATE /wd4267) # Possible loss of data (size_t - int on win64).
    target_compile_options(${target} PRIVATE /wd4800) # Implicit conversion of int to bool.
  endif()
  if(OUR_FLAGS_OWN)
    target_compile_options(${target} PRIVATE ${OUR_FLAGS_OWN})
  endif()
  target_include_directories(${target} PRIVATE ${PROJECT_BINARY_DIR}/src)
  target_include_directories(${target} PRIVATE src)
  target_compile_definitions(${target} PRIVATE $<$<CONFIG:Debug>:CONF_DEBUG>)
  target_include_directories(${target} PRIVATE ${ZLIB_INCLUDE_DIRS})
  if(OPENSSL_FOUND)
    target_compile_definitions(${target} PRIVATE CONF_OPENSSL)
	target_include_directories(${target} PRIVATE ${OPENSSL_INCLUDE_DIRS})
  endif()
  if(VERSION)
    target_compile_definitions(${target} PRIVATE GAME_RELEASE_VERSION="${VERSION}")
  endif()
endforeach()

foreach(target ${TARGETS_DEP})
  if(MSVC)
    target_compile_options(${target} PRIVATE /W0)
  endif()
  if(OUR_FLAGS_DEP)
    target_compile_options(${target} PRIVATE ${OUR_FLAGS_DEP})
  endif()
endforeach()
