include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Transformers"
                    "${CMAKE_CURRENT_SOURCE_DIR}/Transformers/TestTransformer"
                    "${CMAKE_CURRENT_SOURCE_DIR}/Windows"
                    "${CMAKE_CURRENT_SOURCE_DIR}/Util"
                    )

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Transformers")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Windows")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Util")

add_executable(AlphabetConverter main.cpp)

target_link_libraries(AlphabetConverter TransformerLib TransformerUtilityLib WindowLib GlobalUtilityLib)