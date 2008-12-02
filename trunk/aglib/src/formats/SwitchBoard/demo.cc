#include <ag/agfio.h>
#include <ag/AGAPI.h>

int main()
{
  agfio io;
  map<string,string> signalInfo;
  signalInfo["uri"] = "LDC93S7/somewhere/sw2001.wav";
  signalInfo["mimeClass"] = "audio";
  signalInfo["mimeType"] = "wav";
  signalInfo["encoding"] = "wav";
  signalInfo["unit"] = "16kHz";

  list<AGId> l = io.load("SwitchBoard", "../../../demo/data/sw2001.mrk", "A1", &signalInfo);
  cout << toXML(*l.begin()) << endl;
}



