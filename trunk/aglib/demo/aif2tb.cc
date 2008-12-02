// aif2tb.cc: AIF to Treebank converter
// Haejoong Lee, Xiaoyi Ma, Steven Bird
// Copyright (C) 2001-2003 Linguistic Data Consortium, U Penn.
// Web: http://www.ldc.upenn.edu/; Email: ldc@ldc.upenn.edu
// For license information, see the file `LICENSE' included
// with the distribution.


#include <AIF.h>
#include <TreeBank.h>
#include <RE.h>
#include <fstream>

void usage(const string& pname)
{
  cerr << "Usage: " << pname << " [option] <input> <output>" << endl
       << "    options:" << endl
       << "        -f   overwrite existing files" << endl
       << "    input  : input AIF file" << endl
       << "    output : output Treebank file" << endl;
}  

int main(int argc, char* argv[])
{
  if (argc > 4 || argc < 3) {
    usage(argv[0]);
    exit(1);
  }

  string inputname, tbname;
  ofstream tbout;
  bool force = false;

  if (argc == 4) {
    if (string(argv[1]) != "-f") {
      cerr << "Unknown option " << argv[1] << endl;
      usage(argv[0]);
      exit(EXIT_FAILURE);
    }
    else {
      force = true;
      inputname = argv[2];
      tbname = argv[3];
    }
  }
  else if (argc == 3) {
    inputname = argv[1];
    tbname = argv[2];
  }
  else {
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (!force) {
    bool file_exist = false;
    string ans;

    tbout.open(tbname.c_str(), ios::app);
    if (!tbout) {
      cerr << "fail to open output file " << tbname << endl;
      exit(EXIT_FAILURE);
    }
    if ((long) tbout.tellp() != 0) {
      cerr << "file " << tbname << " already exists" << endl;
      file_exist = true;
    }
    if (file_exist) {
      cerr << "overwrite? y/n ";
      cin >> ans;
      if (ans == "y")
	tbout.close();
      else
	exit(EXIT_FAILURE);
    }
  }

  agfio* loader = new AIF();
  agfio* writer = new TreeBank();

  AGIds ids = loader->load(inputname);
  writer->store(tbname, ids);

  delete loader, writer;
  return 0;
}
