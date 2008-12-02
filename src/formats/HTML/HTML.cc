#include "HTML.h"
#include <ag/AGAPI.h>
#include <ag/Utilities.h>
#include <ag/RE.h>
#include <fstream>
using namespace std;

inline static void
print_anchor_table(ostream& out, const AGId& ag)
{
  list<AnchorId> anchors = GetAnchorSet(ag);
  list<AnchorId>::iterator anchor = anchors.begin();
  out << "<table border=\"1\"><tr><td>" << endl;
  out << "<table>" << endl;
  for (; anchor != anchors.end(); ++anchor) {
    out << "<tr><td><a name=\"" << *anchor << "\"/>" << *anchor << "</td>";
    if (GetAnchored(*anchor))
      out << "<td>" << GetAnchorOffset(*anchor) << "</td>";
    else
      out << "<td></td>" << endl;
    out << "<td>" << GetOffsetUnit(*anchor) << "</td>";
    set<SignalId> sigs = GetAnchorSignalIds(*anchor);
    set<SignalId>::iterator sig = sigs.begin();
    out << "<td>";
    for (; sig != sigs.end(); ++sig)
      out << "<a href=\"#" << *sig << "\">" << *sig << "</a>";
    out << "</td></tr>" << endl;
  }
  out << "</table>" << endl;
  out << "</td></tr></table>" << endl;
}

inline static void
print_feature_table(ostream& out, const Id& id)
{
  static RE idpat("^[^:]+:[^:]+:[^:]+$");
  map<FeatureName,FeatureValue> features = GetFeatures(id);
  if (features.empty()) return;
  map<FeatureName,FeatureValue>::iterator feature = features.begin();
  out << "<table>" << endl;
  for (; feature != features.end(); ++feature) {
    out << "<tr valign=\"top\"><td bgcolor=\"#ccccff\" align=\"right\">"
	<< feature->first << "</td>";
    string toks = feature->second;
    string tok;
    out << "<td bgcolor=\"#eeeeee\">";
    while ((tok=Utilities::next_tok(toks)) != "") {
      if (idpat.match(tok))
	out << "<a href=\"#" << tok << "\">" << tok << "</a> ";
      else
	out << tok << " ";
    }
    out << "</td></tr>" << endl;
  }
  out << "</table>" << endl;
}

inline static void
print_annotation_table(ostream& out, const AGId& ag)
{
  set<AnnotationType> types = GetAnnotationTypes(ag);
  set<AnnotationType>::iterator type = types.begin();
  out << "<table border=\"1\"><tr><td>" << endl;
  for (; type != types.end(); ++type) {
    list<AnnotationId> anns = GetAnnotationSeqByOffset(ag, 0.0, 0.0, *type);
    list<AnnotationId>::iterator ann = anns.begin();
    for (; ann != anns.end(); ++ann) {
      out << "<p><a name=\"" << *ann << "\"/>" << *ann;
      AnchorId s = GetStartAnchor(*ann);
      AnchorId e = GetEndAnchor(*ann);
      out << " (" << *type << ", "
	  << "<a href=\"#" << s << "\">" << s << "</a>, "
	  << "<a href=\"#" << e << "\">" << e << "</a>)" << endl;
      print_feature_table(out, *ann);
      out << "</p>" << endl;
    }
  }
  out << "</td></tr></table>" << endl;
}

inline static void
print_timeline(ostream& out, const TimelineId& tl)
{
  out << "<a name=\"" << tl << "\"/>"
      << "<h2>" << tl << "</h2>" << endl;
  print_feature_table(out, tl);
  out << "<table border=\"1\"><tr><td>" << endl;
  set<SignalId> sigs = GetSignals(tl);
  set<SignalId>::iterator sig = sigs.begin();
  for (; sig != sigs.end(); ++sig) {
    out << "<a name=\"" << *sig << "\"/>"
	<< "<h3>" << *sig << "</h3>" << endl
	<< "<p><table border=\"1\">" << endl
      
	<< "<tr><td>mimeClass</td><td>"
	<< GetSignalMimeClass(*sig)
	<< "</td></tr>" << endl
      
	<< "<tr><td>mimeType</td><td>"
	<< GetSignalMimeType(*sig)
	<< "</td></tr>" << endl
      
	<< "<tr><td>encoding</td><td>"
	<< GetSignalEncoding(*sig)
	<< "</td></tr>" << endl
      
	<< "<tr><td>xlink:type</td><td>"
	<< GetSignalXlinkType(*sig)
	<< "</td></tr>" << endl
      
	<< "<tr><td>xlink:href</td><td>"
	<< GetSignalXlinkHref(*sig)
	<< "</td></tr>" << endl
      
	<< "<tr><td>track</td><td>"
	<< GetSignalTrack(*sig)
	<< "</td></tr>" << endl
      
	<< "</table></p>" << endl;
    print_feature_table(out, *sig);
  }
  out << "</td></tr></table>" << endl;
}
    
string
HTML::store(const string& filename,
	    const Id& id,               // AGSetId
	    map<string,string>* options)   // ignored
  throw (agfio::StoreError)
{
  ofstream out(filename.c_str());
  if (! out.good())
    throw agfio::StoreError("can't open " + filename);

  if (id.find(':') != string::npos)
    throw agfio::StoreError("expecting AGSetId for the second arg. "
			    "Something else was given");

  out << "<html>" << endl
      << "<head>" << endl
      << "<title>" << id << "</title>" << endl
      << "</head>" << endl
      << "<body>" << endl;

  out << "<h1>" << id << "</h1>" << endl;
  print_feature_table(out, id);

  set<AGId> ags = GetAGIds(id);
  set<AGId>::iterator ag = ags.begin();

  out << "<p>AGs: ";
  for (; ag != ags.end(); ++ag)
    out << "<a href=\"#" << *ag << "\">" << *ag << "</a> " << endl;
  out << "</p>" << endl;

  out << "<table border=\"1\" cellpadding=\"10\"><tr><td>" << endl;
  for (ag=ags.begin(); ag != ags.end(); ++ag)
    print_timeline(out, GetTimelineId(*ag));

  for (ag=ags.begin(); ag != ags.end(); ++ag) {
    TimelineId tl = GetTimelineId(*ag);
    out << "<a name=\"" << *ag << "\"/>"
	<< "<h2>" << *ag << " ("
	<< "<a href=\"#" << tl << "\">" << tl << "</a>)</h2>" << endl;
    print_feature_table(out, *ag);
    out << "<table border=\"1\" cellpadding=\"10\"><tr><td>" << endl;
    print_anchor_table(out, *ag);
    print_annotation_table(out, *ag);
    out << "</td></tr></table>" << endl;
  }

  out << "</td></tr></table>" << endl;
  out << "</body>" << endl
      << "</html>" << endl;

  return "";
}
    
