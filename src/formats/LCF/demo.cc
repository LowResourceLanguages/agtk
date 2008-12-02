#include <ag/agfio.h>
#include <ag/AGAPI.h>

int main()
{
  agfio io;
  map<string,string> signalInfo;
  signalInfo["uri"] = "LDC-LCF00/speech_data/en_4065.sph";
  signalInfo["mimeClass"] = "audio";
  signalInfo["mimeType"] = "NIST sphere";
  signalInfo["encoding"] = "8-bit mu-law";
  signalInfo["unit"] = "8kHz";

  list<AGId> l = io.load("LCF", "../../../demo/data/en_4065.txt", "A1", &signalInfo);
  string tmp = *l.begin();
  tmp = tmp.substr(0,tmp.find(":"));
  cout << toXML(tmp) << endl;
}



