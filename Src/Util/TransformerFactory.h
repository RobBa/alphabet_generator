/**
 * @file TransformerFactory.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Factory for transformer objects.
 * @version 0.1
 * @date 2021-02-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "TransformerBase.h"
#include "BastaTransformer.h"

#include "GlobalParameters.h"

class TransformerFactory{
public:
  static TransformerBase* createTransformer();
};