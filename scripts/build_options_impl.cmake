if(GEN_PDB)
    target_compile_options(TupoyeMenu PRIVATE "$<$<CONFIG:Release>:/Z7>")
    target_link_options(TupoyeMenu PRIVATE "$<$<CONFIG:Release>:/DEBUG>")
    target_link_options(TupoyeMenu PRIVATE "$<$<CONFIG:Release>:/OPT:REF>")
    target_link_options(TupoyeMenu PRIVATE "$<$<CONFIG:Release>:/OPT:ICF>")
endif()

if(ENABLE_CRASHES)
    target_compile_definitions(TupoyeMenu PRIVATE ENABLE_CRASHES)
endif()

if(ENABLE_SOCIALCLUB)
    target_compile_definitions(TupoyeMenu PRIVATE ENABLE_SOCIALCLUB)
endif()

if(ENABLE_ASI_LOADER)
    target_compile_definitions(TupoyeMenu PRIVATE ENABLE_ASI_LOADER)
endif()

if(ENABLE_LUA)
    target_compile_definitions(TupoyeMenu PRIVATE ENABLE_LUA)
    target_link_libraries(TupoyeMenu PRIVATE lua_static)
endif()
