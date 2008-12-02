// agtree.i: SWIG interface for Treebank API
// Haejoong Lee
// Copyright (C) 2001-2003 Linguistic Data Consortium, U Penn.
// Web: http://www.ldc.upenn.edu/; Email: ldc@ldc.upenn.edu
// For license information, see the file `LICENSE' included
// with the distribution.


%header %{
#include <ag/agtree.h>
%}

string tree_init_tree(const string&,const string&);
string tree_init_tree(const string&);
bool tree_move_down(const string&,const string&,string new_anno);
bool tree_move_down(const string&,const string&);
bool tree_move_down(const string&);
bool tree_move_up(const string&);
bool tree_promote_right(const string&);
bool tree_promote_left(const string&);
bool tree_demote_right(const string&);
bool tree_demote_left(const string&);
string tree_first_tree(const string&);
string tree_last_tree(const string&);
bool tree_move(const string&,const string&);
string tree_insert_node_left(const string&,const string&,string);
string tree_insert_node_left(const string&,const string&);
string tree_insert_node_left(const string&);
string tree_insert_node_right(const string&,const string&,string);
string tree_insert_node_right(const string&,const string&);
string tree_insert_node_right(const string&);
bool tree_delete_node_left(const string&);
bool tree_delete_node_right(const string&);
string tree_root(string,int);
string tree_root(string);
string tree_parent(const string&);
list<string> tree_children(const string&);
string tree_left(const string&);
string tree_right(const string&);
list<string> tree_path(string);
string tree_common_ancestor(const string&,const string&);
string tree_insert_node(const string&,const string&,const string&,const string&);
string tree_insert_node(const string&,const string&,const string&);
string tree_insert_node(const string&,const string&);
bool tree_delete_node(const string&);
