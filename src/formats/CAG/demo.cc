#include <ag/agfio.h>
#include <ag/AGAPI.h>

int main()
{
  agfio io;
  AGSetId agsetId = CreateAGSet("TT");
  TimelineId tlId = CreateTimeline(agsetId);
  AGId agId = CreateAG(agsetId, tlId);
  AnchorId a1, a2, a3;
  AnnotationId n1, n2;
  a1 = CreateAnchor(agId);
  a2 = CreateAnchor(agId);
  a3 = CreateAnchor(agId);
  n1 = CreateAnnotation(agId, a1, a2, "TYPE1");
  SetFeature(n1, "label", "a");
  n2 = CreateAnnotation(agId, a2, a3, "TYPE2");
  SetFeature(n2, "label", "dog");

  io.store("CAG", "cag.txt", agId);
  DeleteAGSet(agsetId);

  io.load("CAG", "cag.txt");
  cout << toXML(agsetId) << endl;
}



