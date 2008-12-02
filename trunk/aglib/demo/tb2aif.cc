// tb2aif.cc: Treebank to AIF converter
// Haejoong Lee, Xiaoyi Ma, Steven Bird
// Copyright (C) 2001-2003 Linguistic Data Consortium, U Penn.
// Web: http://www.ldc.upenn.edu/; Email: ldc@ldc.upenn.edu
// For license information, see the file `LICENSE' included
// with the distribution.


#include <AGAPI.h>
#include <AGException.h>
#include <TreeBank.h>
#include <TB_UPenn_WSJ_EscapeHandler.h>
#include <fstream>
#include "RE.h"

static RE headtail("^[ \t]*([^ \t]+)(.*)$");

using namespace std;

void usage(const string& pname)
{
  cerr << "Usage: " << pname << " [option] <input> <output>" << endl
       << "    options:" << endl
       << "        -f   overwrite existing files" << endl
       << "    input  : input Treebank file" << endl
       << "    output : prefix of output files. e.g. \"wsj_0103\"" << endl
       << "             will generate wsj_0103.aif and wsj_0103.txt" << endl;
}  

int main(int argc, char* argv[])
{
  if (argc < 3) {
    usage(argv[0]);
    exit(1);
  }

  if (argc == 4 && string(argv[1]) != "-f") {
    cerr << "Unknown option " << argv[1] << endl;
    usage(argv[0]);
    exit(1);
  }

  string inputname, aifname, txtname;
  ofstream aifout, txtout;

  if (argc == 3) {
    inputname = argv[1];
    aifname = argv[2] + string(".aif");
    txtname = argv[2] + string(".txt");
  }
  else {
    inputname = argv[2];
    aifname = argv[3] + string(".aif");
    txtname = argv[3] + string(".txt");
  }

  if (argc == 3) {
    bool file_exist = false;
    string ans;

    aifout.open(aifname.c_str(), ios::app);
    txtout.open(txtname.c_str(), ios::app);

    if (!aifout) {
      cerr << "fail to open output file " << aifname << endl;
      exit(EXIT_FAILURE);
    }
    if (!txtout) {
      cerr << "fail to open output file " << txtname << endl;
      exit(EXIT_FAILURE);
    }
    if ((long) aifout.tellp() != 0) {
      cerr << "file " << aifname << " already exists" << endl;
      file_exist = true;
    }
    if ((long) txtout.tellp() != 0) {
      cerr << "file " << txtname << " already exists" << endl;
      file_exist = true;
    }

    if (file_exist) {
      cerr << "overwrite? y/n ";
      cin >> ans;
      if (ans == "y") {
	aifout.close();
	txtout.close();
	aifout.open(aifname.c_str());
	txtout.open(txtname.c_str());
      }
      else
	exit(EXIT_FAILURE);
    }
  }
  else {
    aifout.open(aifname.c_str());
    txtout.open(txtname.c_str());
    if (!aifout) {
      cerr << "fail to open output file " << aifname << endl;
      exit(EXIT_FAILURE);
    }
    if (!txtout) {
      cerr << "fail to open output file " << txtname << endl;
      exit(EXIT_FAILURE);
    }
  }

  TB_UPenn_WSJ_EscapeHandler eh;
  agfio* loader = new TreeBank(eh);
  AGSetId agset = "Treebank";
  AGId ag;

  ag = loader->load(inputname, agset);
  aifout << toXML(ag);

  string ancrs = GetAnchorSet(ag);
  string annos;
  AnchorId ancr;
  AnnotationId anno;

  while (headtail.match(ancrs)) {
    ancr = headtail.get_matched(1);   // we could use sstream instead of RE
    ancrs = headtail.get_matched(2);  // we don't use it for compatibility
    try {
      annos = GetOutgoingAnnotationSet(ancr);
    }
    catch (AGException& e) {
      break;
    }
    while (headtail.match(annos)) {
      anno = headtail.get_matched(1);
      annos = headtail.get_matched(2);
      if (anno != "" && GetAnnotationType(anno) == "wrd") {
	try {
	  txtout << GetFeature(anno, "label") << " ";
	}
	catch (AGException& e) {
	  // do nothing -- don't print it
	}
	break;
      }
    }
  }
  delete loader;
  return 0;
}
