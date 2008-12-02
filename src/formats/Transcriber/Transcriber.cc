// Transcriber.cc: Transcriber loader/writer implementation
// Haejoong Lee, Xiaoyi Ma, Steven Bird
// Copyright (C) 2001,2002 Linguistic Data Consortium, U Penn.
// Web: http://www.ldc.upenn.edu/; Email: ldc@ldc.upenn.edu
// For license information, see the file `LICENSE' included
// with the distribution.


#include "Transcriber.h"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include "AGAPI.h"
#include "Identifiers.h"
#include "RE.h"
#include <string>
#include <map>
#include <vector>
#include <cstdio>
#include <fstream>
#include "agfXercesUtils.h"
using std::string;
using std::map;
using std::vector;
using std::ofstream;

static AGIds toAG(const AGSetId&, DOMDocument*);
static void toTrans(const string&, const AGIds&);

static string
next_id(string& ids)
{
  int i, j;
  string s;

  i = ids.find_first_not_of(" ");
  if (i == string::npos)
    return "";

  j = ids.find_first_of(" ", i);
  if (j == string::npos) {
    s = ids.substr(i);
    ids = "";
  }
  else {
    s = ids.substr(i, j-i);
    ids = ids.substr(j);
  }
  return s;
}


AGIds
Transcriber::load()
{
  // initialize xercesc
  try {
    xercesc_open();
  }
  catch (const agfioError& e) {
    throw LoadError(string("Transcriber:") + e.what());
  }

  DOMDocument* doc = agfDOMParse(annotationFile);
  if (doc == NULL)
    throw LoadError("Transcriber::load():parsing error at file " +
                    annotationFile);

  // conversion (trans --> ag)
  AGIds ids = toAG(agsetId, doc);
  xercesc_close();

  //agIds = agIds.empty() ? ids : agIds + " " + ids;
  agIds = ids;
  DeleteTimeline(timelineId);
  DeleteAG(agId);
  return ids;
}

void
Transcriber::store(const string& filename,
                   const AGIds& agIds,
                   map<string,string>* options)
{
  // agIds must have the same namespace (agset id)
  if (!agIds.empty()) {
    AGIds ids = agIds;
    AGId id = next_id(ids);
    string ns = Identifiers::getNamespace(id);
    while ((id=next_id(ids)) != "") {
      string ns1 = Identifiers::getNamespace(id);
      if (ns != ns1)
        throw StoreError("Transcriber::store(): ags must be in the same agset");
      ns = ns1;
    }
    toTrans(filename, agIds);
  }
  else
    toTrans(filename, this->agIds);
}


/////////////////////////////
// helper functions (load) //
/////////////////////////////

static void
trim(string& s)
{
  int a = s.find_first_not_of(" \t\n");
  if (a == string::npos) {
    s.erase();
    return;
  }
  int b = s.find_last_not_of(" \t\n");
  s = s.substr(a, b-a+1);
}

inline static string
get_fname(const XMLCh* ename, const XMLCh* fname)
{
  return trans(ename) + "/" + trans(fname);
}

static void
mk_mdata(DOMNode* e, map<string,string>& m)
{
  if (e == NULL) return;
  DOMNamedNodeMap* A = e->getAttributes();
  if (A == NULL) return;
  for (int i=0; i < A->getLength(); ++i) {
    DOMNode* a = A->item(i);
    m[get_fname(e->getNodeName(),a->getNodeName())] = trans(a->getNodeValue());
  }
}

static void
mk_mdata_Speaker(DOMDocument* doc, map<string,map<string,string> >& m)
{
  DOMNodeList* L = doc->getElementsByTagName(XMLString::transcode("Speaker"));
  if (L == NULL) return;
  for (int i=0; i < L->getLength(); ++i) {
    DOMElement* e = (DOMElement*) L->item(i);
    mk_mdata(e, m[trans(e->getAttribute(trans("id")))]);
  }
}

static void
mk_mdata_Topic(DOMDocument* doc, map<string,map<string,string> >& m)
{
  DOMNodeList* L = doc->getElementsByTagName(XMLString::transcode("Topic"));
  if (L == NULL) return;
  for (int i=0; i < L->getLength(); ++i) {
    DOMElement* e = (DOMElement*) L->item(i);
    mk_mdata(e, m[trans(e->getAttribute(trans("id")))]);
  }
}

static void
mk_mdata_Section(DOMNode* e,
                 map<string,string>& m,
                 map<string,map<string,string> >& r)
{
  mk_mdata(e, m);
  map<string,string>::iterator pos = m.find("Section/topic");
  if (pos != m.end()) {
    map<string,string> m1 = r[pos->second];
    //m.erase(pos);
    for (pos=m1.begin(); pos != m1.end(); ++pos)
      m.insert(*pos);
  }
}

static void
mk_mdata_Turn(DOMNode* e,
              map<string,string>& m,
              map<string,string>& s,
              map<string,map<string,string> >& r)
{
  mk_mdata(e, m);

  map<string,string>::iterator pos = m.find("Turn/speaker");
  if (pos != m.end()) {
    map<string,string> m1 = r[pos->second];
    //m.erase(pos);
    for (pos=m1.begin(); pos != m1.end(); ++pos)
      m.insert(*pos);
  }

  for (pos=s.begin(); pos != s.end(); ++pos)
    m.insert(*pos);
}

static void
mk_ag(const AGId& ag, DOMNode* turn)
{
  map<string,string> att;
  string turn_start, turn_end;

  // get signals and timeline
  SignalIds signals;
  TimelineId tl = GetTimelineId(ag);
  if (!tl.empty())
    signals = GetSignals(tl);

  // start and end time of turn
  
  //string turn_start = "";
  // get the first node
  DOMNode* e = turn->getFirstChild();
  if (e->getNodeType() == DOMNode::TEXT_NODE) {
    string s = trans(e->getNodeValue());
    trim(s);
    if (s.empty()) {
      e = e->getNextSibling();
      if (e == NULL)
	return;
    }
  }

  // assume that first node is "Sync"
  if (trans(e->getNodeName()) != "Sync")
    throw agfio::LoadError("Transcriber.cc:mk_ag():first node is not Sync");

  // first anchor
  AnchorId a = CreateAnchor(ag, signals);
  double offset =
    atof(trans(((DOMElement*)e)->getAttribute(trans("time"))).c_str());
  SetAnchorOffset(a, offset);

  AnnotationId an;
  string label;
  string last_non_text_tag, current;
  map<string,string> m;
  e = e->getNextSibling();
  while (e != NULL) {
    if (e->getNodeType() == DOMNode::TEXT_NODE) {
      string s = trans(e->getNodeValue());
      trim(s);
      if (s.empty()) {
	e = e->getNextSibling();
	continue;
      }
      label += s;
    }

    current = trans(e->getNodeName());
    if (current == "Sync")
    if (current == "Sync") {
      AnchorId b = CreateAnchor(ag, signals);
      offset =
        atof(trans(((DOMElement*)e)->getAttribute(trans("time"))).c_str());
      SetAnchorOffset(b, offset);
      an = CreateAnnotation(ag, a, b, "annotation");
      map<string,string>::iterator pos;
      for (pos=m.begin(); pos != m.end(); ++pos)
        SetFeature(an, pos->first, pos->second);
      SetFeature(an, "label", label);
      m.clear();
      label.erase();
      a = b;
    }
    else //if (last_non_text_tag 
      mk_mdata(e, m);
    e = e->getNextSibling();
  }

  if (last_non_text_tag != "Sync") {
    AnchorId b = CreateAnchor(ag, signals);
    offset =
      atof(trans(((DOMElement*)turn)->getAttribute(trans("endTime"))).c_str());
    SetAnchorOffset(b, offset);
    an = CreateAnnotation(ag, a, b, "annotation");
    map<string,string>::iterator pos;
    for (pos=m.begin(); pos != m.end(); ++pos)
      SetFeature(an, pos->first, pos->second);
    SetFeature(an, "label", label);
  }
}

static AGIds
toAG(const AGSetId& agsetId, DOMDocument* doc)
{
  map<string,string> tre;               // trans, episode
  map<string,map<string,string> > st;   // speaker, topic
  map<string,string> sec;               // section
  map<string,string> turn;              // turn

  // prepare metadata
  mk_mdata(doc->getDocumentElement(), tre);
  mk_mdata(doc->getDocumentElement()->getLastChild(), tre);
  mk_mdata_Speaker(doc, st);
  mk_mdata_Topic(doc, st);

  // create a timeline
  TimelineId tl;

  map<string,string>::iterator pos = tre.find("Trans/audio_filename");
  if (pos != tre.end()) {
    tl = CreateTimeline(agsetId);
    CreateSignal(tl, pos->second, "mimeClass", "mimeType",
                 "encoding", "unit", "");
  }

  for (pos=tre.begin(); pos != tre.end(); ++pos)
    SetFeature(agsetId, pos->first, pos->second);

  // make ags in an agset
  AGIds ids;
  DOMNodeList* secL = doc->getElementsByTagName(trans("Section"));
  if (secL == NULL) return "";

  for (int i=0; i < secL->getLength(); ++i) {
    DOMNode* e = secL->item(i);
    mk_mdata_Section(e, sec, st);
    DOMNodeList* turnL = e->getChildNodes();
    if (turnL == NULL) continue;

    for (int j=0; j < turnL->getLength(); ++j) {
      // create an ag
      AGId ag = CreateAG(agsetId, tl);
      e = turnL->item(j);
      mk_mdata_Turn(e, turn, sec, st);
      for (pos=turn.begin(); pos != turn.end(); ++pos)
        SetFeature(ag, pos->first, pos->second);
      ids += ag + " ";

      mk_ag(ag, e);
    }
  }

  trim(ids);
  return ids;
}

//////////////////////////////
// helper functions (store) //
//////////////////////////////

static map<string,map<string,string> >
att_process(const Id& id)
{
  map<string,map<string,string> > result;
  string f, fnames = GetFeatureNames(id);
  int slash_pos;
  while ((f=next_id(fnames)) != "") {
    slash_pos = f.find_first_of("/");
    result[f.substr(0,slash_pos)][f.substr(slash_pos+1)] = GetFeature(id,f);
  }
  return result;
}

static map<string,map<string,string> >
att_process_anno(const AnnotationId& id)
{
  map<string,map<string,string> > result;
  string f, fnames = GetFeatureNames(id);
  fnames.replace(fnames.find("label"), 5, "");
  int slash_pos;
  while ((f=next_id(fnames)) != "") {
    slash_pos = f.find_first_of("/");
    result[f.substr(0,slash_pos)][f.substr(slash_pos+1)] = GetFeature(id,f);
  }
  return result;
}

class id_comp
{
public:
  bool operator()(const map<string,string>& m1,
		  const map<string,string>& m2) const {
    map<string,string>::const_iterator p1 = m1.find("id");
    map<string,string>::const_iterator p2 = m2.find("id");
    return p1->second < p2->second;
  }
};

template<typename T>
class startOffset_comp
{
public:
  bool operator()(const map<string,string>& m1,
		  const map<string,string>& m2) const {
    map<string,string>::const_iterator p1 = m1.find("startOffset");
    map<string,string>::const_iterator p2 = m2.find("startOffset");
    double o1 = atof(p1->second.c_str());
    double o2 = atof(p2->second.c_str());
    return o1 < o2;
  }
};

class offset_comp
{
public:
  bool operator()(const AGId& id1, const AGId& id2) const {
    double so1 = atof(GetFeature(id1,"Section/startTime").c_str());
    double so2 = atof(GetFeature(id2,"Section/startTime").c_str());
    double to1 = atof(GetFeature(id1,"Turn/startTime").c_str());
    double to2 = atof(GetFeature(id2,"Turn/startTime").c_str());
    if (so1 < so2)
      return true;
    else if (so1 == so2)
      return to1 < to2;
    else
      return false;
  }
};

struct turn: public map<string,string>
{
  AGId agid;
  turn(map<string,string>& m, const AGId& id)
    : map<string,string>(m), agid(id) {}
};
typedef set<turn,startOffset_comp<turn> > turn_set;

struct section: public map<string,string>
{
  turn_set Turns;
  section(map<string,string>& m): map<string,string>(m) {}
  pair<turn_set::iterator,bool> insert(turn& t)
  { return Turns.insert(t); }
};
typedef set<section,startOffset_comp<section> > section_set;

static void
write_opentag(ostream& out, const string& tag, const map<string,string>& att)
{
  map<string,string>::const_iterator pos;

  out << "<" << tag << " ";
  for (pos=att.begin(); pos != att.end(); ++pos)
    out << pos->first << "=\"" << pos->second << "\" ";
  out << ">" << endl;
}

inline static void
write_opentag(ostream& out, const string& tag)
{
  out << "<" << tag << ">" << endl;
}

static void
write_singletag(ostream& out, const string& tag, const map<string,string>& att)
{
  map<string,string>::const_iterator pos;

  out << "<" << tag << " ";
  for (pos=att.begin(); pos != att.end(); ++pos)
    out << pos->first << "=\"" << pos->second << "\" ";
  out << "/>" << endl;
}

inline static void
write_closetag(ostream& out, const string& tag)
{
  out << "</" << tag << ">" << endl;
}

static void
write_graph(ostream& out, const AGId& agId)
{
  AnnotationIds ids = GetAnnotationSeqByOffset(agId);
  AnnotationId anno = next_id(ids);

  out << "<Sync time=\">" << GetStartOffset(anno) << "\"/>" << endl;

  while (!anno.empty()) {
    out << GetFeature(anno,"label") << endl;

    map<string,map<string,string> > M = att_process_anno(anno);
    map<string,map<string,string> >::iterator pos;

    for (pos=M.begin(); pos != M.end(); ++pos)
      write_singletag(out, pos->first, pos->second);
    
    out << "<Sync time=\">" << GetEndOffset(anno) << "\"/>" << endl;
    anno = GetOutgoingAnnotationSet(GetEndAnchor(anno));
  }
}
 
static void
toTrans(const string& filename, const AGIds& agIds)
{
  // openfile
  ofstream out(filename.c_str());
  if (!out)
    throw agfio::StoreError("Transcriber::store:can't open " +
			    filename + " for writing");
  
  // sort agIds by Section startTime and Turn startTime
  set<AGId,offset_comp> ids;
  AGIds tmp = agIds;
  AGId id;
  while ((id=next_id(tmp)) != "")
    ids.insert(id);
  
  set<map<string,string>,id_comp> Speakers, Topics;
  section_set Sections;
  
  set<AGId>::iterator pos;
  for (pos=ids.begin(); pos != ids.end(); ++pos) {
    map<string,map<string,string> > M = att_process(*pos);
    Speakers.insert(M["Speaker"]);
    Topics.insert(M["Topic"]);
    section sec(M["Section"]);
    turn aturn(M["Turn"], *pos);
    section_set::iterator p = Sections.find(sec);
    if (p == Sections.end()) {
      sec.insert(aturn);
      Sections.insert(sec);
    }
    else {
      ((section)*p).insert(aturn);
    }
  }
  
  out << "<?xml version=\"1.0\"?>" << endl;
  out << "<!DOCTYPE Trans SYSTEM \"trans-13.dtd\">" << endl;
  // open Trans
  map<string,map<string,string> > M =
    att_process(Identifiers::getNamespace(*(ids.begin())));
  write_opentag(out, "Trans", M["Trans"]);
  // write Speakers
  write_opentag(out, "Speakers");
  set<map<string,string>,id_comp>::iterator sti;
  for (sti=Speakers.begin(); sti != Speakers.end(); ++sti)
    write_singletag(out, "Speaker", *sti);
  write_closetag(out, "Speakers");
  // write Topics
  write_opentag(out, "Topics");
  for (sti=Topics.begin(); sti != Topics.end(); ++sti)
    write_singletag(out, "Topic", *sti);
  write_closetag(out, "Topics");
  // open Episode
  write_opentag(out, "Episode", M["Episode"]);
  
  // write Section, Turn
  set<section,startOffset_comp<section> >::iterator si;
  set<turn,startOffset_comp<turn> >::iterator ti;
  for (si=Sections.begin(); si != Sections.end(); ++si) {
    write_opentag(out, "Section", *si);
    for (ti=si->Turns.begin(); ti != si->Turns.end(); ++ti) {
      write_opentag(out, "Turn", *ti);
      write_graph(out, ti->agid);
      write_closetag(out, "Turn");
    }
    write_closetag(out, "Section");
  }
  
  // close Episode, Trans
  write_closetag(out, "Episode");
  write_closetag(out, "Trans");
  out.close();
}

