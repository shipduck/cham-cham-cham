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
