#include <ag/agfio.h>
#include <ag/AGAPI.h>

int main()
{
  agfio io;

  map<string,string> signalInfo;
  signalInfo["mimeClass"] = "audio";
  signalInfo["mimeType"] = "sph";
  signalInfo["encoding"] = "16 bit linear";
  signalInfo["unit"] = "16kHz";
  signalInfo["uri"] = "LDC96S36/pubdata/f1a/radio/s01/f1as01p1.sph";

  list<AGId> l = io.load("BU", "../../../demo/data/f1as01p1", "A1", &signalInfo);
  cout << toXML(*l.begin()) << endl;
}



