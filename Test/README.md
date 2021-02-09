## Content

This folder contains scripts for some testing. 

## Files

- TestStreaming.py: A python script to test the streaming behavior. It takes an existing file, 
  e.g. a netflow-text file, and slowly writes line by line a new file with a specified path. An
  instance of the AlphabetConverter can be launched immediately in a subprocess via giving a
  valid path to a .ini-file of the Alphabet-Converter. WARNING: Make sure you hit the right path. 
  The script currently does not check every possible input scenario. Example run from this 
  directory: 

  '''
  python TestStreaming.py ../Data/exampleStreaming.txt ./output.txt
  '''

  or 

  '''
  python TestStreaming.py ../Data/exampleStreaming.txt ./output.txt --converter_ini ../Init/example.ini 
  '''

  . For help, run 

  '''
  python TestStreaming.py --help
  '''

  Hint: When observing the file you'll see that lines will not appear line by line, but rather in
  batches of a few lines. This is due to the buffering of the software and does not impede it's 
  functionality.

- TestStreaming.cpp: Same as TestStreaming.py, but with hardcoded paths and less versatile.