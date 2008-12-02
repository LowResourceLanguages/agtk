#include <ag/agfio.h>
#include <ag/AGAPI.h>

int main()
{
  agfio io;
  map<string,string> signalInfo;
  map<string,string> options;

  signalInfo["uri"] = "VERVET/somewhere/V1-1.wav";
  signalInfo["mimeClass"] = "audio";
  signalInfo["mimeType"] = "wav";
  signalInfo["encoding"] = "16-bit linear";
  signalInfo["unit"] = "44.1kHz";

  options["header"] = "START,END,TYPE,DATE,TIME,";
  options["header"] += "CALLER,RECIPIENT,CONTEXT,CALL_TYPE,REMARKS";
  options["separator"] = ",";
  options["ann_type"] = "CSV";

  io.load("TF", "../../../demo/data/V1-1.ann", "A1", &signalInfo, &options);

  signalInfo["uri"] = "MEERKAT/somewhere/MK_DT1.wav";
  signalInfo["mimeClass"] = "audio";
  signalInfo["mimeType"] = "wav";
  signalInfo["encoding"] = "16-bit linear";
  signalInfo["unit"] = "32kHz";

  options["header"] = "START\tEND\tTYPE\tCALL-TYPE\tPACK\tCALLER-ID\t";
  options["header"] += "CONTEXT\tREC-DATE\tREC-TIME\tREC-QUALITY\tCOMMENTS";
  options["separator"] = "\t";
  options["ann_type"] = "TDF";

  io.load("TF", "../../../demo/data/MK_DT1.tsv", "A1", &signalInfo, &options);

  cout << toXML("A1") << endl;
}



