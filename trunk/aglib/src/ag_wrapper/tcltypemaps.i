%include utils.i

%typemap(in) string {
    $1.assign(Tcl_GetString($input));
}

%typemap(in) const string&, const AnnotationId&, const AGId&, const AGIds&, const AnnotationType&, const Id& {
    static string s;
    s.assign(Tcl_GetString($input));
    $1 = &s;
}

%typemap(out) string {
#if TCL_MAJOR_VERSION < 8 || (TCL_MAJOR_VERSION==8 && TCL_MINOR_VERSION < 4)
    Tcl_SetStringObj($result, (char*) $1.c_str(), $1.size());
#else
    Tcl_SetStringObj($result, $1.c_str(), $1.size());
#endif
}

%typemap(out) list<string>, list<AnnotationId> {
    list<string>::iterator pos = $1.begin();
    for (int i=0; pos != $1.end(); ++i, ++pos)
        Tcl_ListObjAppendElement
            (interp, $result, Tcl_NewStringObj(pos->c_str(), pos->size()));
}

%typemap(out) set<string> {
    set<string>::iterator pos = $1.begin();
    for (int i=0; pos != $1.end(); ++i, ++pos)
        Tcl_ListObjAppendElement
            (interp, $result, Tcl_NewStringObj(pos->c_str(), pos->size()));
}

%typemap(out) map<string,string> {
    Tcl_UnsetVar(interp, "__tcl_ag_wrapper_temp__", 0);
    for (map<string,string>::iterator pos=$1.begin(); pos!=$1.end(); ++pos) {
        Tcl_ListObjAppendElement(interp, $result,
                                 Tcl_NewStringObj(pos->first.c_str(),
                                                  pos->first.size()));
        Tcl_ListObjAppendElement(interp, $result,
                                 Tcl_NewStringObj(pos->second.c_str(),
                                                  pos->second.size()));
    }
}

%typemap(in) list<string> {
    try {
        tlist2list(interp, $input, $1);
    }
    catch (const string& msg) {
        raise_error(msg);
    }
}

%typemap(in) list<string>&, list<string>*, list<string>* const {
    static list<string> l;
    try {
        tlist2list(interp, $input, l);
        $1 = &l;
    }
    catch (const string& msg) {
        raise_error(msg);
    }
}

%typemap(in) set<string> {
    try {
	tlist2set(interp, $input, $1);
    }
    catch (const string& msg) {
        raise_error(msg);
    }
}

%typemap(in) set<string>&, set<string>*, set<string>* const {
    static set<string> s;
    try {
        tlist2set(interp, $input, s);
        $1 = &s;
    }
    catch (const string& msg) {
        raise_error(msg);
    }
}

%typemap(in) map<string,string> {
    try {
        array2map(interp, $input, $1);
    }
    catch (const string& msg) {
        raise_error(msg);
    }
}

%typemap(in) map<string,string>&, map<string,string>* {
    static map<string,string> m;
    try {
        array2map(interp, $input, m);
        $1 = &m;
    }
    catch (const string& msg) {
	raise_error(msg);
    }
}

%header %{
void
tlist2list(Tcl_Interp* interp, Tcl_Obj* lst, list<string>& l)
{
    int num_elts;           // number of elemtnts
    Tcl_Obj** elts;         // list of elements
    if (Tcl_ListObjGetElements(interp,lst,&num_elts,&elts) ==
            TCL_ERROR) {
        Tcl_FreeResult(interp);
        throw string("failed to get list elements");
    }

    l.clear();
    for (int i=0; i < num_elts; ++i) {
        l.push_back(Tcl_GetString(elts[i]));
    }

    Tcl_FreeResult(interp);
}

void
tlist2set(Tcl_Interp* interp, Tcl_Obj* lst, set<string>& s)
{
    int num_elts;           // number of elemtnts
    Tcl_Obj** elts;         // list of elements
    if (Tcl_ListObjGetElements(interp,lst,&num_elts,&elts) ==
            TCL_ERROR) {
        Tcl_FreeResult(interp);
        throw string("failed to get list elements");
    }

    s.clear();
    for (int i=0; i < num_elts; ++i) {
        s.insert(Tcl_GetString(elts[i]));
    }

    Tcl_FreeResult(interp);
}

void
array2map(Tcl_Interp* interp, Tcl_Obj* array, map<string,string>& m)
{
    string script = "array get ";     // tcl script to get element names
    script += Tcl_GetString(array);

    // run the script and get the result as a list object
#if TCL_MAJOR_VERSION < 8 || (TCL_MAJOR_VERSION==8 && TCL_MINOR_VERSION < 4)
    Tcl_Eval(interp, (char*) script.c_str());
#else
    Tcl_Eval(interp, script.c_str());
#endif
    Tcl_Obj* list_obj = Tcl_GetObjResult(interp);

    // get element names in an objest list
    int num_elts;       // number of elemtnts
    Tcl_Obj** elts;     // serialized array
    if (Tcl_ListObjGetElements(interp,list_obj,&num_elts,&elts) == TCL_ERROR) {
        Tcl_FreeResult(interp);
        throw string("failed to access an array");
    }

    // set the result map
    m.clear();
    for (int i=0; i < num_elts; i+=2) {
        m[Tcl_GetString(elts[i])] = Tcl_GetString(elts[i+1]);
    }

    Tcl_FreeResult(interp);
}

bool
check_array(Tcl_Interp* interp, Tcl_Obj* obj)
{
    string script = "array exist ";
    script += Tcl_GetString(obj);
#if TCL_MAJOR_VERSION < 8 || (TCL_MAJOR_VERSION==8 && TCL_MINOR_VERSION < 4)
    Tcl_Eval(interp, (char*) script.c_str());
#else
    Tcl_Eval(interp, script.c_str());
#endif
    return (strcmp(Tcl_GetStringResult(interp),"1")==0) ? true : false;
}
%}
