# MSVC PrecompiledHeader 
MACRO( MSVC_SET_PCH Target PrecompiledHeader PrecompiledSource)
  IF( MSVC )
    SET(PrecompiledBinary "\$(IntDir)\$(TargetName).pch")
    SET(Sources ${${SourcesVar}})
    GET_FILENAME_COMPONENT(PrecompiledBasename
       ${PrecompiledHeader} NAME)
    GET_SOURCE_FILE_PROPERTY(OLD_COMPILE_FLAGS ${PrecompiledSource} COMPILE_FLAGS)
    IF( OLD_COMPILE_FLAGS STREQUAL "OLD_COMPILE_FLAGS-NOTFOUND" )
      SET(OLD_COMPILE_FLAGS "")
    ENDIF(  )
    GET_TARGET_PROPERTY(OLD_TAGET_COMPILE_FLAGS ${Target} COMPILE_FLAGS)
    IF( OLD_TATGET_COMPILE_FLAGS STREQUAL "OLD_TARGET_COMPILE_FLAGS-NOTFOUND" )
      SET(OLD_TARGET_COMPILE_FLAGS "")
    ENDIF(  )

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
      PROPERTIES
        COMPILE_FLAGS
          "${OLD_COMPILE_FLAGS} /Yc\"${PrecompiledBasename}\" /Fp\"${PrecompiledBinary}\""
        OBJECT_OUTPUTS "${PrecompiledBinary}") 
    SET_TARGET_PROPERTIES(${Target}
      PROPERTIES
        COMPILE_FLAGS
          "${OLD_TAGET_COMPILE_FLAGS} /Yu\"${PrecompiledBasename}\"")
  ENDIF( MSVC )
ENDMACRO( MSVC_SET_PCH )
