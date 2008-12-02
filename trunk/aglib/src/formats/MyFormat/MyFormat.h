#ifndef _MYFORMAT_H_
#define _MYFORMAT_H_

#include <ag/agfio_plugin.h>

class MyFormat: public agfio_plugin
{
public:
  virtual list<AGId>
  load(const string& filename,
       const Id& id = "",
       map<string,string>* signalInfo = NULL,
       map<string,string>* options = NULL)
    throw (agfio::LoadError);

  virtual string
  store(const string& filename,
	const Id& id = "",
	map<string,string>* options = NULL)
    throw (agfio::StoreError);
};

AGFIO_PLUGIN(MyFormat);

#endif
