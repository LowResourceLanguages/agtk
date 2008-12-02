#include <ag/agfio.h>
#include <ag/AGAPI.h>

int main()
{
  agfio io;
  list<AGId> l = io.load("TreeBank", "../../../demo/data/wsj_0001.mrg", "A1");
  cout << toXML(*l.begin()) << endl;
}



