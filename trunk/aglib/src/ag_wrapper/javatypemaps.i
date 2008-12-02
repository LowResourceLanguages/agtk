%include utils.i
%include "std_string.i"

%{
#include <stdio.h>
#include <string>
#include <map>
#include <set>
#include <list>
using namespace std;
%}

%typemap(jni) set<string>,set<string>&,set<string>*,set<string>* const "jobject"
%typemap(jtype) set<string>,set<string>&,set<string>*,set<string>* const "java.util.Set"
%typemap(jstype) set<string>,set<string>&,set<string>*,set<string>* const "java.util.Set"
%typemap(jni) list<string>,list<string>&,list<string>*,list<string>* const "jobject"
%typemap(jtype) list<string>,list<string>&,list<string>*,list<string>* const  "java.util.List"
%typemap(jstype) list<string>,list<string>&,list<string>*,list<string>* const  "java.util.List"
%typemap(jni) map<string,string>, map<string,string>&, map<string,string>* "jobject"
%typemap(jtype) map<string,string>, map<string,string>&, map<string,string>* "java.util.Map"
%typemap(jstype) map<string,string>, map<string,string>&, map<string,string>* "java.util.Map"

%typemap(out) list<string> {
    list2jlist(jenv, $1, $result);
}

%typemap(out) set<string> {
    set2jset(jenv, $1, $result);    
}

%typemap(out) map<string,string> {
    map2jmap(jenv, $1, $result);
}

%typemap(in) list<string> {
    try {
        jlist2list(jenv, $input, $1);
    }
    catch (const string& msg) {
        raise_error(msg);
    }
}

%typemap(in) list<string>&, list<string>*, list<string>* const {
    static list<string> l;
    try {
        jlist2list(jenv, $input, l);
        $1 = &l;
    }
    catch (const string& msg) {
        raise_error(msg);
    }
}

%typemap(in) set<string> {
    try {
        jset2set(jenv, $input, $1);
    }
    catch (const string& msg) {
        raise_error(msg);
    }
}

%typemap(in) set<string>&, set<string>*, set<string>* const {
    static set<string> s;
    try {
        jset2set(jenv, $input, s);
        $1 = &s;
    }
    catch (const string& msg) {
        raise_error(msg);
    }
}

%typemap(in) map<string,string> {
    jmap2map(jenv, $input, $1);
}

%typemap(in) map<string,string>*, map<string,string>& {
    static map<string, string> m;
    jmap2map(jenv, $input, m);
    $1 = &m;
}

%header %{
inline static void
list2jlist(JNIEnv* jenv, list<string>& l, jobject& llObj)
{
    jclass ll = jenv->FindClass("java/util/ArrayList");
    jmethodID ll_constructor = jenv->GetMethodID(ll, "<init>", "()V");
    jmethodID ll_add = jenv->GetMethodID(ll, "add", "(Ljava/lang/Object;)Z");

    llObj = jenv->NewObject(ll, ll_constructor);

    // iterate through set and insert into the java object
    for(list<string>::iterator i=l.begin(); i!=l.end(); ++i)
	jenv->CallBooleanMethod(llObj, ll_add,
                                jenv->NewStringUTF(i->c_str()));
}

inline static void
set2jset(JNIEnv* jenv, set<string>& s, jobject& ssObj)
{
    jclass ss = jenv->FindClass("java/util/HashSet");
    jmethodID ss_constructor = jenv->GetMethodID(ss, "<init>", "()V");
    jmethodID ss_add = jenv->GetMethodID(ss, "add", "(Ljava/lang/Object;)Z");

    ssObj = jenv->NewObject(ss, ss_constructor);

    // iterate through set and insert into the java object
    for(set<string>::iterator i=s.begin(); i!=s.end(); ++i)
	jenv->CallBooleanMethod(ssObj, ss_add,
                                jenv->NewStringUTF(i->c_str()));
}

static void
map2jmap(JNIEnv* jenv, map<string,string>& m, jobject& zzObj)
{
    jclass zz = jenv->FindClass("java/util/HashMap");
    jmethodID zz_constructor = jenv->GetMethodID(zz, "<init>", "()V");
    jmethodID zz_put = jenv->GetMethodID(zz, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    zzObj = jenv->NewObject(zz, zz_constructor);

    for (map<string,string>::iterator pos=m.begin(); pos!=m.end(); ++pos)
	jenv->CallObjectMethod(zzObj, zz_put,
                               jenv->NewStringUTF(pos->first.c_str()),
                               jenv->NewStringUTF(pos->second.c_str()));
}

static void
jlist2list(JNIEnv* jenv, jobject& zzObj, list<string>& l)
{
    if (zzObj == NULL) return;

    jclass ii = jenv->FindClass("java/util/Iterator");
    jmethodID ii_hasNext = jenv->GetMethodID(ii, "hasNext", "()Z");
    jmethodID ii_next = jenv->GetMethodID(ii, "next", "()Ljava/lang/Object;");

    jclass zz = jenv->GetObjectClass(zzObj);
    jmethodID zz_iterator =
        jenv->GetMethodID(zz, "iterator", "()Ljava/util/Iterator;");

    jobject iiObj = jenv->CallObjectMethod(zzObj, zz_iterator);

    l.clear();
    while (jenv->CallBooleanMethod(iiObj, ii_hasNext)) {
        jstring item = (jstring) jenv->CallObjectMethod(iiObj, ii_next);
	const char* str = jenv->GetStringUTFChars(item, NULL);
        l.push_back(str);
        jenv->ReleaseStringUTFChars(item, str);
    }
}

static void
jset2set(JNIEnv* jenv, jobject& zzObj, set<string>& s)
{
    if (zzObj == NULL) return;

    jclass ii = jenv->FindClass("java/util/Iterator");
    jmethodID ii_hasNext = jenv->GetMethodID(ii, "hasNext", "()Z");
    jmethodID ii_next = jenv->GetMethodID(ii, "next", "()Ljava/lang/Object;");

    jclass zz = jenv->GetObjectClass(zzObj);
    jmethodID zz_iterator =
        jenv->GetMethodID(zz, "iterator", "()Ljava/util/Iterator;");

    jobject iiObj = jenv->CallObjectMethod(zzObj, zz_iterator);

    s.clear();
    while (jenv->CallBooleanMethod(iiObj, ii_hasNext)) {
        jstring item = (jstring) jenv->CallObjectMethod(iiObj, ii_next);
	const char* str = jenv->GetStringUTFChars(item, NULL);
        s.insert(str);
        jenv->ReleaseStringUTFChars(item, str);
    }
}

static void
jmap2map(JNIEnv* jenv, jobject& mmObj, map<string,string>& m)
{
    if (mmObj == NULL) return;

    jclass ss = jenv->FindClass("java/util/Set");
    jclass ii = jenv->FindClass("java/util/Iterator");

    jmethodID ss_iterator = jenv->GetMethodID(ss, "iterator", "()Ljava/util/Iterator;");
    jmethodID ii_hasNext = jenv->GetMethodID(ii, "hasNext", "()Z");
    jmethodID ii_next = jenv->GetMethodID(ii, "next", "()Ljava/lang/Object;");

    jclass mm = jenv->GetObjectClass(mmObj);
    jmethodID mm_entrySet =
       jenv->GetMethodID(mm, "entrySet", "()Ljava/util/Set;");
    // FIXME: what if mmObj is not java.util.Map??

    jobject ssObj = jenv->CallObjectMethod(mmObj, mm_entrySet);
    jobject iiObj = jenv->CallObjectMethod(ssObj, ss_iterator);

    // loop through iterator looking at each mapping
    jclass ee;
    jobject eeObj;
    jmethodID ee_getKey, ee_getValue;
    if (jenv->CallBooleanMethod(iiObj, ii_hasNext)) {
        eeObj = jenv->CallObjectMethod(iiObj, ii_next);
        ee = jenv->GetObjectClass(eeObj);
        ee_getKey = jenv->GetMethodID(ee, "getKey", "()Ljava/lang/Object;");
        ee_getValue = jenv->GetMethodID(ee, "getValue", "()Ljava/lang/Object;");

        while (1) {
            jstring key = (jstring) jenv->CallObjectMethod(eeObj, ee_getKey);
            jstring value = (jstring) jenv->CallObjectMethod(eeObj, ee_getValue);
            const char* str1 = jenv->GetStringUTFChars(key,NULL);
            const char* str2 = jenv->GetStringUTFChars(value,NULL);
            m[str1] = str2;
            jenv->ReleaseStringUTFChars(key, str1);
            jenv->ReleaseStringUTFChars(value, str2);

            //TODO: what if the objects arent strings?
            if (jenv->CallBooleanMethod(iiObj, ii_hasNext))
                eeObj = jenv->CallObjectMethod(iiObj, ii_next);
            else
                break;
        }
    }
}

%}
