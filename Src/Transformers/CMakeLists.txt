add_library(TransformerLib STATIC
            ${CMAKE_CURRENT_SOURCE_DIR}/TransformerBase.cpp    
            ${CMAKE_CURRENT_SOURCE_DIR}/BastaTransformer/BastaTransformer.cpp 
            ${CMAKE_CURRENT_SOURCE_DIR}/BastaTransformer/BastaFeatures.cpp 
            )