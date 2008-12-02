#include "MyFormat.h"

list<AGId>
MyFormat::load(const string& filename,
	       const Id& id,
	       map<string,string>* signalInfo,
	       map<string,string>* options)
  throw (agfio::LoadError)
{
  cout << "MyFormat::load" << endl;
  cout << "filename=" << filename << endl;
  list<AGId> a;
  return a;
}

string
MyFormat::store(const string& filename,
		const Id& id,
		map<string,string>* options)
  throw (agfio::StoreError)
{
  cout << "MyFormat::store" << endl;
  cout << "filename=" << filename << endl;
  cout << "id=" << id << endl;
  return "";
}
