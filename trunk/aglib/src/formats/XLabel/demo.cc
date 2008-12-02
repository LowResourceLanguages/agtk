#include <ag/agfio.h>
#include <ag/AGAPI.h>

int main()
{
  agfio io;
  map<string,string> options;
  options["ann_type"] = "lba";
  list<AGId> l =
    io.load("XLabel", "../../../demo/data/f1as01p1.lba", "A1", NULL, &options);
  cout << toXML(*l.begin()) << endl;
}



