CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
PROJECT( dokuro )

SET( DOKURO_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )
SET( DOKURO_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test )
SET( DOKURO_RES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res )
SET( DOKURO_EXT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext )
SET( DOKURO_EXE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/exe )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin" )

AUX_SOURCE_DIRECTORY( ${DOKURO_SRC_DIR} DOKURO_SRC_FILES )
AUX_SOURCE_DIRECTORY( ${DOKURO_TEST_DIR} DOKURO_TEST_FILES )
AUX_SOURCE_DIRECTORY( ${DOKURO_EXE_DIR} DOKURO_EXE_FILES )

#########################################################
# Add Irrlight engine
ADD_SUBDIRECTORY( ${DOKURO_EXT_DIR}/irrlicht )

INCLUDE_DIRECTORIES( ${IRRLICHT_INC_DIR} )

# Add LibOVR
ADD_SUBDIRECTORY( ${DOKURO_EXT_DIR}/libovr )

INCLUDE_DIRECTORIES( ${LIBOVR_INC_DIR} )

# Add Google Test
ADD_SUBDIRECTORY( ${DOKURO_EXT_DIR}/googletest )

INCLUDE_DIRECTORIES( ${DOKURO_EXT_DIR}/googletest/include )
#########################################################

# MSVC PrecompiledHeader 
MACRO( MSVC_SET_PCH Target PrecompiledHeader PrecompiledSource)
  IF( MSVC )
    SET(PrecompiledBinary "\$(IntDir)\$(TargetName).pch")
    SET(Sources ${${SourcesVar}})
    GET_FILENAME_COMPONENT(PrecompiledBasename
       ${PrecompiledHeader} NAME)

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
      PROPERTIES
        COMPILE_FLAGS
          "/Yc\"${PrecompiledBasename}\" /Fp\"${PrecompiledBinary}\""
        OBJECT_OUTPUTS "${PrecompiledBinary}") 
    SET_TARGET_PROPERTIES(${Target}
      PROPERTIES
        COMPILE_FLAGS
          "/Yu\"${PrecompiledBasename}\"")
  ENDIF( MSVC )
ENDMACRO( MSVC_SET_PCH )

# Set Include Path
INCLUDE_DIRECTORIES( ${DOKURO_SRC_DIR} )

IF( MSVC )
	LIST( APPEND DOKURO_LINK_LIBS irrlicht_shared LibOVR Setupapi Winmm	)
ELSE()
	LIST( APPEND DOKURO_LINK_LIBS irrlicht_static )
ENDIF()

IF( APPLE )
	LIST( APPEND DOKURO_RES_FILES ${DOKURO_RES_DIR}/OSX/MainMenu.nib )
	SET( DOKURO_EXE_OPTION MACOSX_BUNDLE )
ELSE(  )
ENDIF(  )

FILE( GLOB DOKURO_INC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h )
FILE( GLOB DOKURO_TEST_INC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/test/*.h )

ADD_LIBRARY( dokuro 
	${DOKURO_SRC_FILES}
	${DOKURO_RES_FILES}
	${DOKURO_INC_FILES} )
	
MSVC_SET_PCH( dokuro
    "${DOKURO_SRC_DIR}/stdafx.h"
    "${DOKURO_SRC_DIR}/stdafx.cpp")

ADD_EXECUTABLE( dokuro_exe 
	${DOKURO_EXE_OPTION}
	${DOKURO_EXE_FILES} )

TARGET_LINK_LIBRARIES( dokuro_exe
	${DOKURO_LINK_LIBS}
	${IRRLICHT_LINK_LIBS}
	dokuro )


ADD_EXECUTABLE( dokuro_test
	${DOKURO_TEST_INC_FILES}
	${DOKURO_TEST_FILES} )

IF( NOT MSVC )
	SET_TARGET_PROPERTIES( dokuro
	PROPERTIES
	COMPILE_FLAGS "-std=c++11 -stdlib=libc++" )
ENDIF(  )

TARGET_LINK_LIBRARIES( dokuro_test 
	${DOKURO_LINK_LIBS} 
	${IRRLICHT_LINK_LIBS} 
	gtest
	dokuro )

MSVC_SET_PCH( dokuro_test
    "${DOKURO_TEST_DIR}/test_stdafx.h"
    "${DOKURO_TEST_DIR}/test_stdafx.cpp")
