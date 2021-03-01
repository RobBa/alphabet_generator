## Structure

This project is modularly structured. The ROOT/Util folder has the global variables, which are 
set by the ini-files in the Ini/-directory, and it has the parser for the ini-files. The 
example.ini file is a commented example of how to write the .ini-files. 

The Src-folder has the main files. We call a concrete instance of an alphabet generator a 
transformer, since it transforms raw data into an alphabet. All transformers must derive 
from the abstract class TransformerBase. An example (including comments) is given via the 
StreamingBastaTransformer. 

Each transformer has a window that groups data together, for example a window of fixed size
and stride, grouping netflows together. The Windows are in the Window-directory in Src. Also,
each Transformer has a set of Transformer-parameters, which can vary depending on the transformer
being used. They are currently in the TransformerParameters.h file, which resides in the Util-dir 
in Src. 

The main file in the Src-dir is then really simple and self explanatory.

In short, we have 4 main components, out of which 3 are generic and can be personalized:
- The global parameters
- The transformer instance, which holds:
    - One window instance
    - One instance of its specific parameters