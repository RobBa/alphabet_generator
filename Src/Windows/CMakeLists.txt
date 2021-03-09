add_library(WindowLib STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/WindowBase.cpp   
  ${CMAKE_CURRENT_SOURCE_DIR}/StochasticWindow.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/FixedSizeWindow.cpp    
)