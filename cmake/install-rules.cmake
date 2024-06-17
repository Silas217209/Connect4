install(
    TARGETS Connect4_exe
    RUNTIME COMPONENT Connect4_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
