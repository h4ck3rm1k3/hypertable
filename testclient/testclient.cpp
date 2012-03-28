#include "Common/Compat.h"

#include <cstdio>
#include <cstring>
#include <iostream>

#include "Common/Error.h"
#include "Common/System.h"

#include "Hypertable/Lib/ApacheLogParser.h"
#include "Hypertable/Lib/Client.h"
#include "Hypertable/Lib/KeySpec.h"

using namespace Hypertable;
using namespace std;
int main(int argc, char **argv) {
  //  ApacheLogParser parser;
  //  ApacheLogEntry entry;
  ClientPtr client_ptr;
  NamespacePtr ns;
  TablePtr table_ptr;
  TableMutatorPtr mutator_ptr;
  KeySpec key;
  const char *inputfile;
  bool time_order = false;
  String row;

  try {
    client_ptr = new Client("/opt/hypertable/0.9.5.0/","/opt/hypertable/0.9.5.0/conf/hypertable.cfg");
    ns = client_ptr->open_namespace("OSM");
    table_ptr = ns->open_table("user");
    mutator_ptr = table_ptr->create_mutator();
  }
  catch (Exception &e) {
    //    report_error(e);
    cerr << "error connnecting"<< e << endl;
    return 1;
  }                                                         

  memset(&key, 0, sizeof(key));                             

  //  parser.load(inputfile);                                   

  //  while (parser.next(entry)) {                              

  //  row = ;
  //    key.row = row.c_str();
  //    key.row_len = row.length();                             

    try {
      key.column_family = "email";
      //      key.row = "test2@test.com";
      //      key.row_len = strlen(key.row);
      mutator_ptr->set(key, "test2@test.com");
      mutator_ptr->flush();                                     
    }
    catch (Exception &e) {
      cerr << "error" << endl;
      mutator_ptr->show_failed(e, cerr);
      //      report_error(e);
      return 1;
    }
                                                         

  return 0;
}
