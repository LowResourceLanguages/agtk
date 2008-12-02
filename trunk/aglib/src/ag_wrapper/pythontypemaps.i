#define __version__ "2.0"

%include utils.i

%typemap(in) string {
    $1.assign(PyString_AsString($input));
}

%typemap(in) const string& {
    static string s;
    s.assign(PyString_AsString($input));
    $1 = &s;
}

%typemap(out) string {
    $result = PyString_FromString($1.c_str());
}

%typemap(out) list<string>, list<AnnotationId> {
    list<string>::iterator pos = $1.begin();
    $result = PyList_New($1.size());
    for (int i=0; pos != $1.end(); ++i, ++pos)
        PyList_SET_ITEM($result, i, PyString_FromString(pos->c_str()));
}

%typemap(out) set<string> {
    set<string>::iterator pos = $1.begin();
    $result = PyList_New($1.size());
    for (int i=0; pos != $1.end(); ++i, ++pos)
        PyList_SET_ITEM($result, i, PyString_FromString(pos->c_str()));
}

%typemap(out) map<string,string> {
    $result = PyDict_New();
    for (map<string,string>::iterator pos=$1.begin(); pos!=$1.end(); ++pos)
        PyDict_SetItemString($result, (char*) pos->first.c_str(),
                             PyString_FromString(pos->second.c_str()));
}

%typemap(in) list<string> {
    try {
        plist2list($input, $1);
    }
    catch (const string& msg) {
        raise_error(msg);
    }
}

%typemap(in) list<string>&, list<string>*, list<string>* const {
    static list<string> l;
    try {
        plist2list($input, l);
        $1 = &l;
    }
    catch (const string& msg) {
        raise_error(msg);
    }
}

%typemap(in) set<string> {
    try {
        plist2set($input, $1);
    }
    catch (const string& msg) {
        raise_error(msg);
    }
}

%typemap(in) set<string>&, set<string>*, set<string>* const {
    static set<string> s;
    try {
        plist2set($input, s);
        $1 = &s;
    }
    catch (const string& msg) {
        raise_error(msg);
    }
}

%typemap(in) map<string,string> {
    try {
        dic2map($input, m);
    }
    catch (const string& msg) {
        raise_error(msg);
    }
}

%typemap(in) map<string,string>&, map<string,string>* {
    static map<string,string> m;
    try {
        dic2map($input, m);
        $1 = &m;
    }
    catch (const string& msg) {
        raise_error(msg);
    }
}

%header %{
void
plist2list(PyObject* lst, list<string>& l)
{
    //if (! PyList_Check(lst))
    //    throw string("expecting a list");
    l.clear();
    for (int i=0; i < PyList_Size(lst); ++i) {
        PyObject* item = PyList_GetItem(lst, i);
	//if (! PyString_Check(item))
	//    throw string("found a non-string list element");
        l.push_back(PyString_AsString(item));
    }
}

void
plist2set(PyObject* lst, set<string>& s)
{
    //if (! PyList_Check(lst))
    //    throw string("expecting a list");
    s.clear();
    for (int i=0; i < PyList_Size(lst); ++i) {
        PyObject* item = PyList_GetItem(lst, i);
	//if (! PyString_Check(item))
	//    throw string("found a non-string list element");
        s.insert(PyString_AsString(item));
    }
}

void
dic2map(PyObject* dic, map<string,string>& m)
{
    //if (! PyDict_Check(dic))
    //    throw string("expecting a dictionary");
    PyObject *key, *value;
    int pos = 0;
    m.clear();
    while (PyDict_Next(dic,&pos,&key,&value)) {
        //if (!PyString_Check(key) || !PyString_Check(value))
        //    throw string("found a non-string dictionary key or value);
        m[PyString_AsString(key)] = PyString_AsString(value);
    }
}
%}
