s/SWIG_ConvertPtr([^,]*,\([^,]*\),[^,]*,[ \t]*SWIGTYPE_p_setTstring_t[ \t]*,[^,]*)[ \t]*==[ \t]*TCL_ERROR/false/
s/SWIG_ConvertPtr(\([^,]*\),\([^,]*\),[^,]*,[ \t]*SWIGTYPE_p_mapTstring_string_t[ \t]*,[^,]*)[ \t]*==[ \t]*TCL_ERROR/!check_array(\1,\2)/
s/SWIG_ConvertPtr([^,]*,\([^,]*\),[^,]*,[ \t]*SWIGTYPE_p_listTstring_t[ \t]*,[^,]*)[ \t]*==[ \t]*TCL_ERROR/false/
s/SWIG_ConvertPtr([^,]*,\([^,]*\),[^,]*,[ \t]*SWIGTYPE_p_string[ \t]*,[^,]*)[ \t]*==[ \t]*TCL_ERROR/false/

