#include <ag/agfio.h>
#include <ag/AGAPI.h>

int main()
{
  agfio io;
  list<AGId> l = io.load("TIMIT", "sa1", "A1", NULL, NULL);
  cout << toXML(*l.begin()) << endl;
}



