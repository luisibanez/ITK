project(ITKTIFF)

include_directories(BEFORE ${ITKTIFF_SOURCE_DIR})
include_directories(BEFORE ${ITKTIFF_BINARY_DIR})

# library object files common to compression and decompression
set(common_SRCS
  tif_aux.c
  tif_close.c
  tif_codec.c
  tif_compress.c
  tif_color.c
  tif_dir.c
  tif_dirinfo.c
  tif_dirread.c
  tif_dirwrite.c
  tif_dumpmode.c
  tif_error.c
  tif_extension.c
  tif_fax3.c
  tif_flush.c
  tif_getimage.c
  tif_luv.c
  tif_lzw.c
  tif_next.c
  tif_open.c
  tif_packbits.c
  tif_predict.c
  tif_print.c
  tif_read.c
  tif_strip.c
  tif_swab.c
  tif_tile.c
  tif_version.c
  tif_warning.c
  tif_write.c
  tif_zip.c
  tif_jpeg.c
  )

add_definitions(-DJPEG_SUPPORT -DZIP_SUPPORT -D_HPUX_SOURCE)

if(WIN32)
  if(BUILD_SHARED_LIBS)
    set(TIFFDLL 1)
  else(BUILD_SHARED_LIBS)
    set(TIFFSTATIC 1)
  endif(BUILD_SHARED_LIBS)
endif(WIN32)

if(NOT UNIX)
  add_definitions( -DTIF_PLATFORM_CONSOLE )
  set(common_SRCS ${common_SRCS} tif_win32.c tif_fax3sm.c)
  if(NOT MINGW)
    set(common_SRCS ${common_SRCS} itktiff.def)
  endif(NOT MINGW)
else (NOT UNIX)
  set(TIFF_RIGHT_VERSION 1)

  set(common_SRCS ${common_SRCS} tif_unix.c tif_fax3sm.c)
  set_source_files_properties(tif_fax3sm.c GENERATED)
  add_executable(itkmkg3states mkg3states.c)
  if(CYGWIN)
    set(EXE_EXTENSION .exe)
  endif(CYGWIN)
  add_custom_command(
    OUTPUT ${ITKTIFF_BINARY_DIR}/tif_fax3sm.c
    COMMAND ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/itkmkg3states${EXE_EXTENSION}
    ARGS -c const ${ITKTIFF_BINARY_DIR}/tif_fax3sm.c
    DEPENDS itkmkg3states
    )
endif (NOT UNIX)

include (${CMAKE_ROOT}/Modules/CheckIncludeFiles.cmake)
include(${CMAKE_ROOT}/Modules/CheckSymbolExists.cmake)
include(${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)

# Check if header file exists and add it to the list.
check_include_files("assert.h" HAVE_ASSERT_H)
check_include_files("dlfcn.h" HAVE_DLFCN_H)
check_include_files("fcntl.h" HAVE_FCNTL_H)
check_include_files("inttypes.h" HAVE_INTTYPES_H)
check_include_files("limits.h" HAVE_LIMITS_H)
check_include_files("malloc.h" HAVE_MALLOC_H)
check_include_files("memory.h" HAVE_MEMORY_H)
check_include_files("stdint.h" HAVE_STDINT_H)
check_include_files("stdlib.h" HAVE_STDLIB_H)
check_include_files("string.h" HAVE_STRING_H)
check_include_files("strings.h" HAVE_STRINGS_H)
check_include_files("sys/stat.h" HAVE_SYS_STAT_H)
check_include_files("sys/time.h" HAVE_SYS_TIME_H)
check_include_files("sys/types.h" HAVE_SYS_TYPES_H)
check_include_files("unistd.h" HAVE_UNISTD_H)
check_include_files("windows.h" HAVE_WINDOWS_H)
check_include_files("strings.h" HAVE_STRINGS_H)
check_include_files("ieeefp.h" HAVE_IEEEFP_H)

check_function_exists(floor HAVE_FLOOR)
check_function_exists(getopt HAVE_GETOPT)
check_function_exists(isascii HAVE_ISASCII)
check_function_exists(memmove HAVE_MEMMOVE)
check_function_exists(memset HAVE_MEMSET)
check_function_exists(mmap HAVE_MMAP)
check_function_exists(pow HAVE_POW)
check_function_exists(sqrt HAVE_SQRT)
check_function_exists(strcasecmp HAVE_STRCASECMP)
check_function_exists(strchr HAVE_STRCHR)
check_function_exists(strrchr HAVE_STRRCHR)
check_function_exists(strstr HAVE_STRSTR)
check_function_exists(strtol HAVE_STRTOL)
check_function_exists(areroul HAVE_STRTOUL)


include(CheckTypeSize)

check_type_size("signed int" SIZEOF_SIGNED_INT)
check_type_size("unsigned int" SIZEOF_UNSIGNED_INT)
check_type_size("signed short" SIZEOF_SIGNED_SHORT)
check_type_size("unsigned short" SIZEOF_UNSIGNED_SHORT)
check_type_size("signed long" SIZEOF_SIGNED_LONG)
check_type_size("unsigned long" SIZEOF_UNSIGNED_LONG)
check_type_size("signed long long" SIZEOF_SIGNED_LONG_LONG)
check_type_size("unsigned long long" SIZEOF_UNSIGNED_LONG_LONG)
check_type_size("unsigned char *" SIZEOF_UNSIGNED_CHAR_P)


configure_file(${ITKTIFF_SOURCE_DIR}/tiffDllConfig.h.in
  ${ITKTIFF_BINARY_DIR}/tiffDllConfig.h)
configure_file(${ITKTIFF_SOURCE_DIR}/tif_config.h.in
  ${ITKTIFF_BINARY_DIR}/tif_config.h)

add_library(itktiff ${common_SRCS})
target_link_libraries(itktiff itkjpeg ${ITK_ZLIB_LIBRARIES} )

if(UNIX)
  target_link_libraries(itktiff -lm)
endif(UNIX)

# Apply user-defined properties to the library target.
if(ITK_LIBRARY_PROPERTIES)
  set_target_properties(itktiff PROPERTIES ${ITK_LIBRARY_PROPERTIES})
endif(ITK_LIBRARY_PROPERTIES)

if(NOT ITK_INSTALL_NO_LIBRARIES)
  install(TARGETS itktiff
    RUNTIME DESTINATION ${ITK_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${ITK_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${ITK_INSTALL_LIB_DIR_CM24} COMPONENT Development)
endif(NOT ITK_INSTALL_NO_LIBRARIES)

if(ITK_USE_FLAT_DIRECTORY_INSTALL)
  set( ITK_INSTALL_INCLUDE_DIR_CM24_UTILITIES_TIFF ${ITK_INSTALL_INCLUDE_DIR_CM24})
else(ITK_USE_FLAT_DIRECTORY_INSTALL)
  set( ITK_INSTALL_INCLUDE_DIR_CM24_UTILITIES_TIFF ${ITK_INSTALL_INCLUDE_DIR_CM24}/Utilities/itktiff)
endif(ITK_USE_FLAT_DIRECTORY_INSTALL)


if(NOT ITK_INSTALL_NO_DEVELOPMENT)
  install(FILES
    ${ITKTIFF_BINARY_DIR}/tiffDllConfig.h
    ${ITKTIFF_SOURCE_DIR}/tiffvers.h
    ${ITKTIFF_SOURCE_DIR}/itk_tiff_mangle.h
    ${ITKTIFF_SOURCE_DIR}/tiff.h
    ${ITKTIFF_SOURCE_DIR}/tiffio.h
    DESTINATION ${ITK_INSTALL_INCLUDE_DIR_CM24_UTILITIES_TIFF}
    COMPONENT Development)
endif(NOT ITK_INSTALL_NO_DEVELOPMENT)

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing")
endif(CMAKE_COMPILER_IS_GNUCXX)
