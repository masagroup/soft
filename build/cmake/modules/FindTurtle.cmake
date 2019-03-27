# - Find Turtle
# Find the Turtle (http://turtle.sourceforge.net/) includes
# This module defines
#  TURTLE_INCLUDE_DIR, Turtle include directory, where to find turtle/mock.hpp, etc.
#  TURTLE_FOUND, If false, do not try to use Turtle.
#
# This module reads hints about search locations from variables:
#  TURTLE_DIR           - Preferred installation directory
#  TURTLE_INCLUDE_DIR   - Preferred include directory

if(NOT TURTLE_DIR)
    set(TURTLE_DIR "" CACHE PATH "Installation directory of Turtle")
endif()

find_path(TURTLE_INCLUDE_DIR
    NAMES turtle/mock.hpp turtle/sequence.hpp
    PATHS ${TURTLE_DIR} ${TURTLE_INCLUDE_DIR}
    PATH_SUFFIXES include
    )

# handle the QUIETLY and REQUIRED arguments and set TURTLE_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TURTLE DEFAULT_MSG TURTLE_INCLUDE_DIR)

mark_as_advanced(TURTLE_INCLUDE_DIR)