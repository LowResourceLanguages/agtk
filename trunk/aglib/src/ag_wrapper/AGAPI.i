// AGAPI.i: SWIG interface for AG API
// Haejoong Lee
// Copyright (C) 2001-2003 Linguistic Data Consortium, U Penn.
// Web: http://www.ldc.upenn.edu/; Email: ldc@ldc.upenn.edu
// For license information, see the file `LICENSE' included
// with the distribution.


%header %{
#include <ag/AGAPI.h>
#include <ag/AGException.h>
#include <ag/agfio.h>
%}

/***********************************************************
** Exception Handler ***************************************
***********************************************************/

%include exception.i
%exception {
        try {
                $function
        }
        catch (AGException& err) {
                string message = "AGException\x01";
                message += err.error();
                SWIG_exception(SWIG_RuntimeError, (char*) message.c_str());
        }
        catch (const agfio::LoadError& err) {
                string message = "LoadError\x01";
                message += err.what();
                SWIG_exception(SWIG_RuntimeError, (char*) message.c_str());
        }
        catch (const agfio::StoreError& err) {
                string message = "StoreError\x01";
                message += err.what();
                SWIG_exception(SWIG_RuntimeError, (char*) message.c_str());
        }
}


/***********************************************************
** Typemaps ************************************************
***********************************************************/

#ifdef SWIGJAVA
%include javatypemaps.i
#endif

#ifdef SWIGTCL8
%include tcltypemaps.i
#endif

#ifdef SWIGPYTHON
%include pythontypemaps.i
#endif

#ifdef SWIGPERL
%include perltypemaps.i
#endif

/***********************************************************
** Functions to Wrap ***************************************
***********************************************************/

string CreateAGSet(string);
bool ExistsAGSet(string);
void DeleteAGSet(string);
string CreateAG(string,string);
bool ExistsAG(string);
void DeleteAG(string);
set<string> GetAGIds(string);
string CreateTimeline(string);
bool ExistsTimeline(string);
void DeleteTimeline(string);
string CreateSignal(string,string,string,string,string,string,string);
bool ExistsSignal(string);
void DeleteSignal(string);
set<string> GetSignals(string);
string GetSignalMimeClass(string);
string GetSignalMimeType(string);
string GetSignalEncoding(string);
string GetSignalXlinkType(string);
string GetSignalXlinkHref(string);
string GetSignalUnit(string);
string GetSignalTrack(string);
string CreateAnnotation(string,string,string,string);
bool ExistsAnnotation(string);
void DeleteAnnotation(string);
string CopyAnnotation(string);
list<string> SplitAnnotation(string);
list<string> NSplitAnnotation(string,short);
string GetAnnotationType(string);
string GetAnnotationInfo(string);
string GetStartAnchor(string);
string GetEndAnchor(string);
void SetStartAnchor(string,string);
void SetEndAnchor(string,string);
double GetStartOffset(string);
double GetEndOffset(string);
void SetStartOffset(string,double);
void SetEndOffset(string,double);
set<string> GetAnnotationTypes(string);
set<string> GetAnnotationFeatureNames(string,const string&);
set<string> GetAnnotationFeatureNames(string);
void SetFeature(string,string,string);
bool ExistsFeature(string,string);
void DeleteFeature(string,string);
string GetFeature(string,string);
void UnsetFeature(string,string);
set<string> GetFeatureNames(string);
void SetFeatures(string,map<string,string>&);
map<string,string> GetFeatures(string);
void UnsetFeatures(string);
string CreateAnchor(string,double,string,set<string>&);
string CreateAnchor(string,set<string>&);
string CreateAnchor(string);
bool ExistsAnchor(string);
void DeleteAnchor(string);
void SetAnchorOffset(string,double);
double GetAnchorOffset(string);
void SetOffsetUnit(string,string);
string GetOffsetUnit(string);
void SetAnchorSignalIds(string,set<string>&);
set<string> GetAnchorSignalIds(string);
bool GetAnchored(string);
void UnsetAnchorOffset(string);
string SplitAnchor(string);
set<string> GetIncomingAnnotationSet(string,const string&);
set<string> GetIncomingAnnotationSet(string);
set<string> GetOutgoingAnnotationSet(string,const string&);
set<string> GetOutgoingAnnotationSet(string);
list<string> GetAnchorSet(string);
list<string> GetAnchorSetByOffset(string,double,double);
list<string> GetAnchorSetByOffset(string,double);
set<string> GetAnchorSetNearestOffset(string,double);
set<string> GetAnnotationSet(const string&,const string&);
set<string> GetAnnotationSet(const string&);
set<string> GetAnnotationSetByFeature(string,string,string,const string&);
set<string> GetAnnotationSetByFeature(string,string,string);
list<string> GetAnnotationSetByOffset(string,double,const string&);
list<string> GetAnnotationSetByOffset(string,double);
list<string> GetAnnotationSeqByOffset(string,double,double,const string&);
list<string> GetAnnotationSeqByOffset(string,double,double);
list<string> GetAnnotationSeqByOffset(string,double);
list<string> GetAnnotationSeqByOffset(string);
string GetAnnotationByOffset(string,double,const string&);
string GetAnnotationByOffset(string,double);
string GetAGSetId(string);
string GetAGSetId();
string GetAGId(string);
string GetTimelineId(string);
string toXML(string);
%wrapper %{
#ifdef AGDB
%}
bool LoadFromDB(string,string);
bool StoreToDB(string,string);
list<string> StoreSQLs(string);
%wrapper %{
#endif
%}
bool SPrecedes(const string&,const string&);
list<string> Load(const string&,const string&,const string&,map<string,string>*,map<string,string>*);
list<string> Load(const string&,const string&,const string&,map<string,string>*);
list<string> Load(const string&,const string&,const string&);
list<string> Load(const string&,const string&);
string Store(const string&,const string&,const string&,map<string,string>*);
string Store(const string&,const string&,const string&);
string Store2(const string&,const string&,list<string>* const,map<string,string>*);
string Store2(const string&,const string&,list<string>* const);
bool CheckAnchorOffsetTotal(string,string);
bool CheckAnchorOffsetBounded(string,string);
bool CheckFeatureExists(string,string,string);
bool CheckFeatureIsAnnotationId(string,string,string);
bool CheckLinear(string,string);
bool CheckConnected(string,string);
bool CheckCoextensive(string,string,string);
bool CheckSpan(string,string,string);


%include agtree.i
