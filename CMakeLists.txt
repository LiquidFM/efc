project (efc)

# Project header
project_cxx_library_header_default ()

# 3rdparty
list (APPEND ${PROJECT_NAME}_LIBS brolly)

# Sources
add_subdirectory (src)

# Target
add_library (efc STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries (efc ${${PROJECT_NAME}_LIBS})

# Version
build_version_file (efc 0.0.1)

# Documentation
add_documentation ("efc" 0.0.1 "Exceptions Free Containers library")
