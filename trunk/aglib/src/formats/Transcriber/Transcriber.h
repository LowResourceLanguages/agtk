// Transcriber.h: Transcriber loader/writer class
// Haejoong Lee, Xiaoyi Ma, Steven Bird
// Copyright (C) 2001,2002 Linguistic Data Consortium, U Penn.
// Web: http://www.ldc.upenn.edu/; Email: ldc@ldc.upenn.edu
// For license information, see the file `LICENSE' included
// with the distribution.


#ifndef _TRANSCRIBER_H_
#define _TRANSCRIBER_H_

#include "agfio.h"
#include "AGTypes.h"
#include <string>
#include <map>
using std::string;
using std::map;

class Transcriber: public agfio
{
private:
  virtual bool
  is_id_required()
  { return true; }

  virtual AGIds
  load();

public:
  Transcriber() {}
  ~Transcriber() {}

  virtual void
  store(const string& filename,
	const AGIds& agIds = "",
	map<string,string>* options = NULL);
};

#endif
