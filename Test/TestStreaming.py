import argparse
import subprocess
import time
from os import path


def spawn_converter(converter_ini):
  """[Spawns a new instance of the AlphabetConverter with the given ini.]

  Args:
      converter_ini ([str]): [Path to the .ini-file of the AlphabetConverter.]
  """

  process = subprocess.Popen(["./AlphabetConverter", "converter_ini"], cwd="../bin/", stderr=subprocess.PIPE)
  return process


def stream_file(infile_path, outfile_path, delay, converter_ini):
  """[Streams the file for the delay (in ms) amount. Each delay another line is written from
      infile to outfile]

  Args:
      infile_path ([str]): [Path to the input file]
      outfile_path ([str]): [Path to the output file]
      delay ([float]): [Delay in ms]
  """

  converter_started = False

  infile = open(infile_path, 'r')
  if path.exists(outfile_path):
    print("Output-file already exists. Append to the end.")
    outfile = open(outfile_path, 'a')
  else:
    print("Output-file does not exist. Create a new file: ", outfile_path)
    outfile = open(outfile_path, 'w')
  
  line = infile.readline()
  while line:
    time.sleep(delay)
    outfile.write(line)
    line = infile.readline()

    if converter_ini and not converter_started:
      process = spawn_converter(converter_ini)
      converter_started = True

  infile.close()
  outfile.close()

  if converter_started:
    print("Errors is AlphabetConverter: ", process.stderr)
  print("Streaming terminated successfully.")

if __name__ == "__main__":
  """[Main entry point to program. Parses arguments and starts main loop as subroutine (stream_file()).]

  Raises:
      Exception: [Specified infile-path file does not exist.]
      Exception: [Specified .ini-file for converter does not exist.]
  """

  parser = argparse.ArgumentParser(description='Emulate streaming of file.')
  parser.add_argument('infile', type=str, help='The path of the input-file.')
  parser.add_argument('outfile', type=str, help='The path of the output-file. WARNING: Text will be appended if file already exists.')

  parser.add_argument('--delay', type=float, default=500.0, help='The delay in ms between each new line that will be written.')
  parser.add_argument('--converter_ini', type=str, default=None, help='The path to the .ini-file of the AlphabetConverter. If given,\
                                                                       an instance of the alphabet converter will be launched in a subprocess, \
                                                                       and a conversion will start.')

  args = parser.parse_args()
  infile_path = args.infile
  outfile_path = args.outfile

  delay = args.delay / 1000
  converter_ini = args.converter_ini

  if not path.exists(infile_path):
    raise Exception("Invalid path to input file: ", infile_path)
  elif converter_ini and not path.exists(converter_ini):
    raise Exception("Invalid path to .ini-file (--converter_ini): ", converter_ini)

  stream_file(infile_path, outfile_path, delay, converter_ini)