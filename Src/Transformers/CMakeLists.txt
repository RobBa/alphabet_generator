add_library(TransformerLib STATIC
            ${CMAKE_CURRENT_SOURCE_DIR}/TransformerBase.cpp    
            ${CMAKE_CURRENT_SOURCE_DIR}/TransformerFactory.cpp    

            ${CMAKE_CURRENT_SOURCE_DIR}/BastaTransformer/StreamingBastaTransformer.cpp 
            ${CMAKE_CURRENT_SOURCE_DIR}/BastaTransformer/StreamingBastaFeatures.cpp 
            ${CMAKE_CURRENT_SOURCE_DIR}/PairwiseBastaTransformer/PairwiseBastaTransformer.cpp 
            ${CMAKE_CURRENT_SOURCE_DIR}/PairwiseBastaTransformer/PairwiseBastaFeatures.cpp 
            )