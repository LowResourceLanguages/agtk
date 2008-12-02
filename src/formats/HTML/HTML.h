// HTML.h: HTML loader class definition
// Haejoong Lee, Xiaoyi Ma, Steven Bird
// Copyright (C) 2003 Linguistic Data Consortium, U Penn.
// Web: http://www.ldc.upenn.edu/; Email: ldc@ldc.upenn.edu
// For license information, see the file `LICENSE' included
// with the distribution.


#ifndef _HTML_H_
#define _HTML_H_

#include <ag/agfio_plugin.h>

class DllExport HTML : public agfio_plugin
{
private:
  virtual string
  store(const string& filename,
	const Id& id = "",
	map<string,string>* options = NULL)
    throw (agfio::StoreError);
};

AGFIO_PLUGIN(HTML);

#endif
