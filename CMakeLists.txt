CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

PROJECT( robinson )

SET( Robinson_Hdr
  src/matrix.h
  src/sfs.h
)

SET( Robinson_Src
  src/matrix.cpp
  src/sfs.cpp
  src/main.cpp
)

FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

ADD_EXECUTABLE( robinson main.cpp )
TARGET_LINK_LIBRARIES( robinson ${Boost_LIBRARIES} )
