#include <ag/agfio.h>
#include <ag/AGAPI.h>

int main()
{
  agfio io;
  map<string,string> options;
  options["MAIA"] = "rt.maia.xml";
  options["agann"] = "speaker";
  options["DTDvalidation"] = "false";
  //list<AGId> l = io.load("ATLAS", "sw2389-ms98-a-word.aif.xml", "", NULL, &options);
  list<AGId> l = io.load("ATLAS", "sw4634.aif.xml", "", NULL, &options);
  io.store("ATLAS", "sw.xml", *l.begin(), &options);
}



