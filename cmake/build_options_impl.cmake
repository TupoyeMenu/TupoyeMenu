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

# Optimizations
if(MSVC)
    if(OPTIMIZE)
      set(CMAKE_MODULE_LINKER_FLAGS
          "${CMAKE_MODULE_LINKER_FLAGS} /LTCG /OPT:REF,ICF /GUARD:NO")

      set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
          "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /O2 /Oi /Ot /Oy /Ob3 /sdl- /GL /GF /GS- /Gw")
      string(REPLACE "/Ob1" "/Ob3" CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
    endif()
endif()
