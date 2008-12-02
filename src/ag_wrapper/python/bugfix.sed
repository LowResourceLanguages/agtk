s/SWIG_ConvertPtr(\([^,]*\),[^,]*,[ \t]*SWIGTYPE_p_setTstring_t[ \t]*,[^,]*)[ \t]*==[ \t]*-1/!PyList_Check(\1)/
s/SWIG_ConvertPtr(\([^,]*\),[^,]*,[ \t]*SWIGTYPE_p_mapTstring_string_t[ \t]*,[^,]*)[ \t]*==[ \t]*-1/!PyDict_Check(\1)/
s/SWIG_ConvertPtr(\([^,]*\),[^,]*,[ \t]*SWIGTYPE_p_listTstring_t[ \t]*,[^,]*)[ \t]*==[ \t]*-1/!PyList_Check(\1)/
s/SWIG_ConvertPtr(\([^,]*\),[^,]*,[ \t]*SWIGTYPE_p_string[ \t]*,[^,]*)[ \t]*==[ \t]*-1/!PyString_Check(\1)/

