# - Find efc
# Find the efc library
#
#  This module defines the following variables:
#     EFC_FOUND   - True if EFC_INCLUDE is found
#     EFC_INCLUDE - where to find header files
#     EFC_LIB     - the library files


find_path (EFC_INCLUDE
           NAMES "efc/Atomics"
           PATH_SUFFIXES include
           DOC "EFC include directory")

find_library (EFC_LIB
              NAMES efc
              DOC "EFC library file")

# handle the QUIETLY and REQUIRED arguments and set EFC_FOUND to TRUE if all listed variables are TRUE
include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (EFC DEFAULT_MSG EFC_INCLUDE EFC_LIB)

mark_as_advanced (EFC_INCLUDE EFC_LIB)
