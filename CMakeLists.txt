cmake_minimum_required(VERSION 3.25)
PROJECT( FXAskPass VERSION 0.1 DESCRIPTION "Fox Ask Password client" )

# The project settings
SET( BIN_NAME "fxaskpass" )                 # Executable binnary filename
SET( CMAKE_INSTALL_PREFIX "/usr" )          # Install target directory
SET( LIB_FOX   "/opt/foxlib/1.7/84" )       # Fox toolkit path
SET( CMAKE_CXX_STANDARD 11 )                # C++ Standard
SET( CMAKE_CXX_STANDARD_REQUIRED True )     # C++ Standard
#add_compile_options(-Wall -Wextra -pedantic -Werror)
add_compile_options(-Wall -pedantic)

# Configure project
#CONFIGURE_FILE( MakeConfig.h.in MakeConfig.h )
FILE( GLOB SOURCES src/*.cpp )                          # List of implementation files
INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}/include" )

# Add directories for project dependecies
INCLUDE_DIRECTORIES( "${LIB_FOX}/include/" )
LINK_DIRECTORIES(    "${LIB_FOX}/lib/" )

# Add to the executable
ADD_EXECUTABLE( ${BIN_NAME} ${SOURCES} )
TARGET_LINK_LIBRARIES( ${BIN_NAME} FOX-1.7 )

# Install build project
#INSTALL( TARGETS ${BIN_NAME} RUNTIME DESTINATION bin )
#INSTALL( DIRECTORY data DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/morfeus )		# Globalni data
#INSTALL( FILES integration/${PROJECT_NAME}.png DESTINATION ${CMAKE_INSTALL_PREFIX}/share/pixmaps/ )
#INSTALL( FILES integration/${PROJECT_NAME}.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/ )


### END ###