#include <ag/agfio.h>
#include <ag/AGAPI.h>

int main()
{
  agfio io;
  map<string,string> signalInfo;
  signalInfo["uri"] = "BAS/speach_data/g071a000.wav";
  signalInfo["mimeClass"] = "audio";
  signalInfo["mimeType"] = "wav";
  signalInfo["encoding"] = "wav";
  signalInfo["unit"] = "16kHz";
  list<AGId> l = io.load("BAS", "../../../demo/data/g071a000.par", "A1", &signalInfo);
  cout << toXML(*l.begin()) << endl;
}



