%module ag
%include std_string.i

namespace std {
%typemap(in) list<string> {
    if (SvROK($input)) {    // ROK: expecting a reference, not array
        AV *av = (AV*)SvRV($input);
        if (SvTYPE(av) != SVt_PVAV)
            SWIG_croak("Type error in argument $argnum, of $symname. "
                       "Expected an array of string");
        array_to_list(av, $1);
    }
    else {
        SWIG_croak("Type error in argument $argnum of $sysname. "
                   "Expected an array of string");
    }
}

%typemap(in) list<string>& (list<string> temp),
             list<string>* (list<string> temp),
             list<string>* const (list<string> temp) {
    if (SvROK($input)) {    // ROK: expecting a reference, not array
        AV *av = (AV*)SvRV($input);
        if (SvTYPE(av) != SVt_PVAV)
            SWIG_croak("Type error in argument $argnum, of $symname. "
                       "Expected an array of string");
        array_to_list(av, temp);
        $1 = &temp;
    }
    else {
        SWIG_croak("Type error in argument $argnum of $sysname. "
                   "Expected an array of string");
    }
}

%typemap(out) list<string> {
    AV* arr = newAV();
    list_to_array($1, arr);
    $result = newRV_noinc((SV*)arr);
    sv_2mortal($result);    // don't know why
    argvi++;    // seems to signal the existence of a return value
}

%typemap(in) set<string> {
    if (SvROK($input)) {    // ROK: expecting a reference, not array
        AV *av = (AV*)SvRV($input);
        if (SvTYPE(av) != SVt_PVAV)
            SWIG_croak("Type error in argument $argnum, of $symname. "
                       "Expected a reference to an array of string");
        array_to_set(av, $1);
    }
    else {
        SWIG_croak("Type error in argument $argnum of $sysname. "
                   "Expected a reference to an array of string");
    }
}

%typemap(in) set<string>& (set<string> temp),
             set<string>* (set<string> temp),
             set<string>* const (set<string> temp) {
    if (SvROK($input)) {    // ROK: expecting a reference, not array
        AV *av = (AV*)SvRV($input);
        if (SvTYPE(av) != SVt_PVAV)
            SWIG_croak("Type error in argument $argnum, of $symname. "
                       "Expected a reference to an array of string");
        array_to_set(av, temp);
        $1 = &temp;
    }
    else {
        SWIG_croak("Type error in argument $argnum of $sysname. "
                   "Expected a reference to an array of string");
    }
}

%typemap(out) set<string> {
    AV* arr = newAV();
    set_to_array($1, arr);
    $result = newRV_noinc((SV*)arr);
    sv_2mortal($result);    // don't know why
    argvi++;    // seems to signal the existence of a return value
}

%typemap(in) map<string,string> {
    if (SvROK($input)) {    // expecting a reference
        HV* hv = (HV*)SvRV($input);
        if (SvTYPE(hv) != SVt_PVHV)
            SWIG_croak("Type error in argument $argnum, of $symname. "
                       "Expected a reference to a hash of string");
        hash_to_map(hv, $1);
    }
    else {
        SWIG_croak("Type error in argument $argnum of $sysname. "
                   "Expected a reference to a hash of string");
    }
}

%typemap(in) map<string,string>&  (map<string,string> temp),
             map<string,string>* (map<string,string> temp) {
    if (SvROK($input)) {    // expecting a reference
        HV* hv = (HV*)SvRV($input);
        if (SvTYPE(hv) != SVt_PVHV)
            SWIG_croak("Type error in argument $argnum, of $symname. "
                       "Expected a reference to a hash of string");
        hash_to_map(hv, temp);
        $1 = &temp;
    }
    else {
        SWIG_croak("Type error in argument $argnum of $sysname. "
                   "Expected a reference to a hash of string");
    }
}

%typemap(out) map<string,string> {
    HV* hv = newHV();
    map_to_hash($1, hv);
    $result = newRV_noinc((SV*)hv);
    sv_2mortal($result);
    argvi++;
}

}


%header %{
static void array_to_list(AV* av, list<string>& output)
{
    SV** tv;
    I32 len = av_len(av) + 1;
    for (I32 i=0; i<len; ++i) {
        tv = av_fetch(av, i, 0);
        const char *ptr = SvPV_nolen(*tv);
        output.push_back(ptr);
    }
}

static void list_to_array(list<string>& input, AV* av)
{
    list<string>::iterator pos = input.begin();
    for (; pos != input.end(); ++pos)
        av_push(av, newSVpv(pos->c_str(),pos->size()));
}

static void array_to_set(AV* av, set<string>& output)
{
    SV** tv;
    I32 len = av_len(av) + 1;
    for (I32 i=0; i<len; ++i) {
        tv = av_fetch(av, i, 0);
        const char *ptr = SvPV_nolen(*tv);
        output.insert(ptr);
    }
}

static void set_to_array(set<string>& input, AV* av)
{
    set<string>::iterator pos = input.begin();
    for (; pos != input.end(); ++pos)
        av_push(av, newSVpv(pos->c_str(),pos->size()));
}

static void hash_to_map(HV* hv, map<string,string>& output)
{
    hv_iterinit(hv);
    HE* entry;
    STRLEN l;
    while (entry=hv_iternext(hv)) {
        output[HePV(entry,l)] = SvPV_nolen(HeVAL(entry));
    }
}

static void map_to_hash(map<string,string>& input, HV* hv)
{
    map<string,string>::iterator pos = input.begin();
    for (; pos!=input.end(); ++pos) {
        hv_store(hv,
                 pos->first.c_str(), pos->first.size(),
                 newSVpv(pos->second.c_str(),pos->second.size()),
                 0);
    }
}

%}

using namespace std;

