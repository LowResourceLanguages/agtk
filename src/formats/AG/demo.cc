#include <ag/agfio.h>
#include <ag/AGAPI.h>

int main()
{
  agfio io;
  list<AGId> l = io.load("AG", "../../../demo/data/timit.xml");
  cout << toXML(*l.begin()) << endl;
  cout << "okay" << endl;
  map<string,string> opt;
  opt["encoding"] = "utf-8";
  opt["dtd"] = "ag.dtd";
  io.store("AG", "x.xml", *l.begin(), &opt);
}



