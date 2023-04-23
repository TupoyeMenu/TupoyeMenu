option(GEN_PDB "Generate PDB. Can cause some Anti-Cheat problems Default: OFF" OFF)
option(ENABLE_CRASHES "Enable crash options. Default: OFF" OFF)
option(ENABLE_SOCIALCLUB "Enable Social Club hooking. Default: OFF" OFF)
option(ENABLE_ASI_LOADER "Enable shitty built in ASI Loader. Default: ON" ON)

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
