#include <ag/agfio.h>
#include <ag/agfio_plugin.h>
#include <ag/AGAPI.h>
#include <ag/AGException.h>
#include "ag_dlfcn.h"

agfio::plugin_map_t agfio::plugin_map;
agfio::destroy_func_map_t agfio::destroy_func_map;
agfio::loader_map_t agfio::loader_map;

agfio::LoadError::LoadError(const string& s)
: agfioError(s)
{}

agfio::StoreError::StoreError(const string& s)
: agfioError(s)
{}

agfio::~agfio()
{
  plugin_map_t::iterator pos;
  for (pos=plugin_map.begin(); pos != plugin_map.end(); ++pos)
    unplug(pos->first);
}

list<AGId>
agfio::load(const string& format,
	    const string& filename,
	    const Id& id,
	    map<string,string>* signalInfo,
	    map<string,string>* options)
  throw (LoadError)
{
  try {
    return plug(format)->load(filename, id, signalInfo, options);
  }
  catch (const string& msg) {
    throw LoadError(msg);
  }
  catch (const LoadError& e) {
    throw e;
  }
  catch (AGException& e) {
    throw LoadError(string("AGException occurred, ") + e.error());
  }
  catch (const exception& e) {
    throw LoadError(e.what());
  }
  catch (...) {
    throw LoadError("Unknown exception");
  }
}

string
agfio::store(const string& format,
	     const string& filename,
	     const Id& id,
	     map<string,string>* options)
  throw (StoreError)
{
  try {
    return plug(format)->store(filename, id, options);
  }
  catch (const string& msg) {
    throw StoreError(msg);
  }
  catch (const StoreError& e) {
    throw e;
  }
  catch (AGException& e) {
    throw StoreError(string("AGException occurred, ") + e.error());
  }
  catch (const exception& e) {
    throw StoreError(e.what());
  }
  catch (...) {
    throw StoreError("Unknown exception");
  }
}

string
agfio::store(const string& format,
	     const string& filename,
	     list<string>* const ids,
	     map<string,string>* options)
  throw (StoreError)
{
  try {
    return plug(format)->store(filename, ids, options);
  }
  catch (const string& msg) {
    throw StoreError(msg);
  }
  catch (const StoreError& e) {
    throw e;
  }
  catch (AGException& e) {
    throw StoreError(string("AGException occurred, ") + e.error());
  }
  catch (const exception& e) {
    throw StoreError(e.what());
  }
  catch (...) {
    throw StoreError("Unknown exception");
  }
}

agfio_plugin*
agfio::plug(const string& format)
  throw (const string&)
{
  loader_map_t::iterator pos = loader_map.find(format);
  if (pos != loader_map.end()) {
    return pos->second;
  }

#ifdef _MSC_VER
  string plugin_name = "agfio_plugin_" + format + ".dll";
#else
  string plugin_name = "agfio_plugin_" + format + ".so";
#endif

  void* plugin = dlopen(plugin_name.c_str(), RTLD_LAZY);
  if (!plugin) {
    throw string("Cannot load plugin: ") + dlerror() + '\n';
  }

  // load the symbols
  create_func_t* create_func = (create_func_t*) dlsym(plugin, "create");
  destroy_func_t* destroy_func = (destroy_func_t*) dlsym(plugin, "destroy");
  if (!create_func || !destroy_func) {
    throw string("Cannot load symbols: ") + dlerror() + '\n';
  }

  // create an instance of the class
  agfio_plugin* loader = create_func();

  plugin_map[format] = plugin;
  destroy_func_map[format] = destroy_func;
  loader_map[format] = loader;

  return loader;
}

void
agfio::unplug(const string& format)
{
  destroy_func_map_t::iterator pos = destroy_func_map.find(format);
  if (pos != destroy_func_map.end()) {
    (pos->second)(loader_map[format]);
    dlclose(plugin_map[format]);
    destroy_func_map.erase(pos);
    loader_map.erase(format);
    plugin_map.erase(format);
  }
}

