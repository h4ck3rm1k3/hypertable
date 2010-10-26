/** -*- C++ -*-
 * Copyright (C) 2008  Luke Lu (Zvents, Inc.)
 *
 * This file is part of Hypertable.
 *
 * Hypertable is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 *
 * Hypertable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hypertable. If not, see <http://www.gnu.org/licenses/>
 */

#include "Common/Compat.h"
#include "Common/System.h"

#include <iostream>
#include <fstream>
#include <list>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdio>
#include "Client.h"
#include "gen-cpp/HqlService.h"
#include "ThriftHelper.h"

#define TABLENAME "rediffmail_srs"
#define COLUMN  "actions"
#define MAXCOUNT 50


int HASH[MAXCOUNT] = {0};//{0,0,-4,5,-5,5,5,5,5,0,-5,0,-5,5,5,-4,-5,-4,-5,-5,5,5,-5,-5,-5};
double MAP[MAXCOUNT] = {0};

namespace Hypertable { namespace ThriftGen {

  // Implement the If, so we don't miss any interface changes
  struct BasicTest : HqlServiceIf {
    boost::shared_ptr<Thrift::Client> client;

    BasicTest() : client(new Thrift::Client("10.50.50.101", 38080)) { }

    void create_namespace(const std::string& ns) {
      client->create_namespace(ns);
    }

    void create_table(const Namespace ns, const std::string& name, const std::string& schema) {
      client->create_table(ns, name, schema);
    }

    Scanner open_scanner(const Namespace ns, const std::string& name,
                         const ScanSpec& scan_spec,bool retry_table_not_found = true) {
      return client->open_scanner(ns, name, scan_spec, retry_table_not_found);
    }

    void next_cells_serialized(CellsSerialized& _return,
                               const Scanner scanner) {
      client->next_cells_serialized(_return, scanner);
    }

    void close_scanner(const Scanner scanner) {
      client->close_scanner(scanner);
    }

    void next_cells(std::vector<Cell> & _return, const Scanner scanner) {
      client->next_cells(_return, scanner);
    }

    void next_cells_as_arrays(std::vector<CellAsArray> & _return,const Scanner scanner) {
      client->next_cells_as_arrays(_return, scanner);
    }

    void next_row(std::vector<Cell> & _return, const Scanner scanner) {
      client->next_row(_return, scanner);
    }

    void get_row_serialized(CellsSerialized& _return, const Namespace ns,
                            const std::string& name, const std::string& row) {
      client->get_row_serialized(_return, ns, name, row);
    }
    
    void next_row_serialized(CellsSerialized& _return,
                             const Scanner scanner) {
      client->next_row_serialized(_return, scanner);
    }

    void refresh_shared_mutator(const Namespace ns, const std::string &name, const MutateSpec &mutate_spec) {
      client->refresh_shared_mutator(ns, name, mutate_spec);
    }

    void next_row_as_arrays(std::vector<CellAsArray> & _return,const Scanner scanner) {
      client->next_row_as_arrays(_return, scanner);
    }

    void get_row(std::vector<Cell> & _return, const Namespace ns, const std::string& name,const std::string& row) {
      client->get_row(_return, ns, name, row);
    }

    void get_row_as_arrays(std::vector<CellAsArray> & _return, const Namespace ns,
                           const std::string& name,const std::string& row) {
      client->get_row_as_arrays(_return, ns, name, row);
    }

    void
    get_cells_serialized(CellsSerialized& _return, const Namespace ns,
                         const std::string& name, const ScanSpec& scan_spec) {
      client->get_cells_serialized(_return, ns, name, scan_spec);
    }

    void get_cell(Value& _return, const Namespace ns, const std::string& name,
                  const std::string& row,const std::string& column) {
      client->get_cell(_return, ns, name, row, column);
    }

    void get_cells(std::vector<Cell> & _return, const Namespace ns, 
                   const std::string& name,const ScanSpec& scan_spec) {
      client->get_cells(_return, ns, name, scan_spec);
    }

    void get_cells_as_arrays(std::vector<CellAsArray> & _return, const Namespace ns,
                             const std::string& name, const ScanSpec& scan_spec) {
      client->get_cells_as_arrays(_return, ns, name, scan_spec);
    }

    void offer_cell(const Namespace ns, const std::string &name,
                    const MutateSpec &mutate_spec,const Cell &cell) {
      client->offer_cell(ns, name, mutate_spec, cell);
    }

    void offer_cells(const Namespace ns, const std::string &name,
                     const MutateSpec &mutate_spec,const std::vector<Cell> & cells) {
      client->offer_cells(ns, name, mutate_spec, cells);
    }

    void offer_cell_as_array(const Namespace ns, const std::string &name,
                             const MutateSpec &mutate_spec,const CellAsArray &cell) {
      client->offer_cell_as_array(ns, name, mutate_spec, cell);
    }

    void offer_cells_as_arrays(const Namespace ns, const std::string &name,
                               const MutateSpec &mutate_spec,const std::vector<CellAsArray> & cells) {
      client->offer_cells_as_arrays(ns, name, mutate_spec, cells);
    }

    Namespace open_namespace(const String &ns) {
      return client->open_namespace(ns);
    }
    
    void close_namespace(const Namespace ns) {
      client->close_namespace(ns);
    }

    Mutator open_mutator(const Namespace ns, const std::string& name,
                         int32_t flags,int32_t flush_interval = 0) {
      return client->open_mutator(ns, name, flags, flush_interval);
    }

    void close_mutator(const Mutator mutator, const bool flush) {
      client->close_mutator(mutator, flush);
    }

    void flush_mutator(const Mutator mutator) {
      client->flush_mutator(mutator);
    }

    void set_cell(const Mutator mutator, const Cell& cell) {
      client->set_cell(mutator, cell);
    }

    void
    set_cells_serialized(const Mutator mutator,
                         const CellsSerialized &cells,
                         bool flush) {
      client->set_cells_serialized(mutator, cells, flush);
    }

    bool exists_namespace(const std::string& ns) {
      return client->exists_namespace(ns);
    }

    void set_cells(const Mutator mutator, const std::vector<Cell> & cells) {
      client->set_cells(mutator, cells);
    }

    void set_cell_as_array(const Mutator mutator, const CellAsArray& cell) {
      client->set_cell_as_array(mutator, cell);
    }

    void set_cells_as_arrays(const Mutator mutator,     const std::vector<CellAsArray> & cells) {
      client->set_cells_as_arrays(mutator, cells);
    }

    bool exists_table(const Namespace ns, const std::string& name) {
      return client->exists_table(ns, name);
    }

    void get_table_id(std::string& _return, const Namespace ns, const std::string& table) {
      client->get_table_id(_return, ns, table);
    }

    void get_schema_str(std::string& _return, const Namespace ns, const std::string& name) {
      client->get_schema_str(_return, ns, name);
    }
    
    void get_schema(Schema& _return, const Namespace ns, const std::string& name) {
      client->get_schema(_return, ns, name);
    }

    void get_tables(std::vector<std::string> & _return, const Namespace ns) {
      client->get_tables(_return, ns);
    }

    void get_listing(std::vector<NamespaceListing> & _return, const Namespace ns) {
      client->get_listing(_return, ns);
    }

    void get_table_splits(std::vector<TableSplit> & _return, const Namespace ns,
                          const std::string& name) {
      client->get_table_splits(_return, ns, name);
    }

    void drop_namespace(const std::string& ns, const bool if_exists) {
      client->drop_namespace(ns, if_exists);
    }

    void rename_table(const Namespace ns, const std::string& table, const std::string& new_name) {
      client->rename_table(ns, table, new_name);
    }

    void drop_table(const Namespace ns, const std::string& name, const bool if_exists) {
      client->drop_table(ns, name, if_exists);
    }

    void hql_exec(HqlResult& _return, const Namespace ns, const std::string &command,
                  const bool noflush, const bool unbuffered) {
      client->hql_exec(_return, ns, command, noflush, unbuffered);
    }

    void hql_query(HqlResult &ret, const Namespace ns, const std::string &command) {
      client->hql_query(ret, ns, command);
    }

    void hql_exec2(HqlResult2& _return, const Namespace ns, const std::string &command,
                   const bool noflush, const bool unbuffered) {
      client->hql_exec2(_return, ns, command, noflush, unbuffered);
    }

    void hql_query2(HqlResult2 &ret, const Namespace ns, const std::string &command) {
      client->hql_query2(ret, ns, command);
    }

    int run(std::string inputstr, int method, const Namespace ns,
            std::string tablename, std::string &returnStr) {
      try
        {
          std::ostream &out = std::cout;
          switch (method)
            {
            case 1:
              getRowsHQL(out,inputstr,ns,tablename,returnStr);
              break;
            case 2:
              test_get_row(out,inputstr,ns,tablename,returnStr);
              break;
            case 3:
              test_scan(out,inputstr,ns,tablename,returnStr);
              break;
            default:
              test_get_row(out,inputstr,ns,tablename,returnStr);
              break;
            }
        }
      catch (ClientException &e) {
        returnStr.clear();
        returnStr = e.message;
        return e.code;
      }
      return 0;
    }

    void test_hql(std::ostream &out) {
      HqlResult result;
      Namespace ns = open_namespace("rediffmail");
      hql_query(result, ns, "drop table if exists thrift_test");
      hql_query(result, ns, "create table thrift_test ( col )");
      hql_query(result, ns, "insert into thrift_test values"
                "('2008-11-11 11:11:11', 'k1', 'col', 'v1'), "
                "('2008-11-11 11:11:11', 'k2', 'col', 'v2'), "
                "('2008-11-11 11:11:11', 'k3', 'col', 'v3')");
      hql_query(result, ns, "select * from thrift_test");
      out << result << std::endl;

      HqlResult2 result2;
      hql_query2(result2, ns, "select * from thrift_test");

      out << result2 << std::endl;
    }

    int getRowsHQL(std::ostream &out, std::string inputdata, const Namespace ns,
                   std::string tablename, std::string &returnStr)
    {
      HqlResult result;
      //std::string query("select actions from rediffmail_srs where ROW = '");
      std::string query("select actions from ");
      query += tablename;
      query += " where ROW = '";
      query += inputdata;
      query += "'";
      hql_query(result, ns, query);
      std::vector<Cell> lc;
      std::vector<Cell>::iterator itr;
      lc = result.cells;
      for(itr = lc.begin(); itr != lc.end(); itr++){
        returnStr += itr->key.row;
        //returnStr += ":";
        //returnStr += itr->timestamp;
        returnStr += ":";
        returnStr += itr->key.column_qualifier ;
        returnStr += ",";
        //std::cout<< "HQL [" <<itr->row_key <<"] [" <<itr->timestamp << "] [" <<itr->column_qualifier<< "]"<<std::endl;
      }
      return 0;
    }
    void test_get_row(std::ostream &out, std::string inputdata, const Namespace ns,
                      std::string tablename,std::string &returnStr)
    {
      std::vector<Cell> cells;
      get_row(cells,ns,tablename,inputdata);
      //std::cout<<"size ["<<cells.size()<<"]"<<std::endl;
      foreach(const Cell &cell, cells) {
        returnStr += cell.key.row;
        returnStr += ":";
        //returnStr += cell.timestamp;
        //returnStr += ":";
        returnStr += cell.value;
        returnStr += ",";
        //out <<"GET ["<< cell.row_key <<"] ["<<cell.timestamp<<"] [" <<cell.revision<<"]"<< std::endl;
      }

    }
    void test_scan(std::ostream &out, std::string inputdata, const Namespace ns,
                   std::string tablename, std::string &returnStr) {
      ScanSpec ss;
      RowInterval RI;
      CellInterval CI;

      RI.start_row = inputdata;
      RI.__isset.start_row = true;

      RI.end_row = inputdata;
      RI.__isset.end_row = true;

      RI.start_inclusive = true;
      RI.__isset.start_inclusive = true;

      RI.end_inclusive = true;
      RI.__isset.end_inclusive = true;

      ss.row_intervals.push_back(RI);
      ss.__isset.row_intervals = true;

      //ss.row_intervals.push_back(RI);
      //std::string tablename("rediffmail_srs");
      std::vector<Cell> cells;
      Scanner s = open_scanner(ns, tablename, ss);

      //int COUNTER = 0;
      do {
        cells.clear();
        next_cells(cells, s);
        //std::cout<<"COUNTER ["<<COUNTER++<<"] SIZE ["<<cells.size() <<"]"<<std::endl;
        std::vector<Cell>::iterator itr;
        for(itr = cells.begin(); itr != cells.end(); itr++) {
          returnStr += itr->key.row;
          returnStr += ":";
          //returnStr += itr->timestamp;
          //returnStr += ":";
          returnStr += itr->value;
          returnStr += ",";
          //std::cout<< "SCN [" <<itr->row_key <<"] [" <<itr->timestamp << "] [" <<itr->revision<< "]"<<std::endl;
        }
      } while (cells.size());
      close_scanner(s);
    }

    void test_set() {
      std::vector<Cell> cells;

      Namespace ns = open_namespace("rediffmail");

      Mutator m = open_mutator(ns, "thrift_test", 0);
      cells.push_back(make_cell("k4", "col", 0, "v4", "2008-11-11 22:22:22"));
      cells.push_back(make_cell("k5", "col", 0, "v5", "2008-11-11 22:22:22"));
      cells.push_back(make_cell("k2", "col", 0, "v2a", "2008-11-11 22:22:22"));
      cells.push_back(make_cell("k3", "col", 0, "", "2008-11-11 22:22:22", 0,
                                ThriftGen::KeyFlag::DELETE_ROW));
      set_cells(m, cells);
      close_mutator(m, true);
    }


    void test_put() {
      std::vector<Cell> cells;
      MutateSpec mutate_spec;
      mutate_spec.appname = "test-cpp";
      mutate_spec.flush_interval = 1000;

      Namespace ns = open_namespace("rediffmail");

      cells.push_back(make_cell("put1", "col", 0, "v1", "2008-11-11 22:22:22"));
      cells.push_back(make_cell("put2", "col", 0, "this_will_be_deleted", "2008-11-11 22:22:22"));
      offer_cells(ns, "thrift_test", mutate_spec, cells);
      cells.clear();
      cells.push_back(make_cell("put1", "no_such_col", 0, "v1", "2008-11-11 22:22:22"));
      cells.push_back(make_cell("put2", "col", 0, "", "2008-11-11 22:22:23", 0,
                                ThriftGen::KeyFlag::DELETE_ROW));
      offer_cells(ns, "thrift_test", mutate_spec, cells);
      sleep(2);
    }

  };

}} // namespace Hypertable::Thrift

int initHash()
{
  int i=0;
  for(i=0;i<MAXCOUNT;i++)
    {
      HASH[i] = 0;
      MAP[i] = 0;
    }
  HASH[0] = 0;
  HASH[1] = 0;
  HASH[2] = 0;
  HASH[3] = -4;
  HASH[4] = 5;
  HASH[5] = -5;
  HASH[6] = 5;
  HASH[7] = 5;
  HASH[8] = 5;
  HASH[9] = 5;
  HASH[10] = 0;
  HASH[11] = -5;
  HASH[12] = 0;
  HASH[13] = -5;
  HASH[14] = 5;
  HASH[15] = 5;
  HASH[16] = -4;
  HASH[17] = -5;
  HASH[18] = -4;
  HASH[19] = -5;
  HASH[20] = -5;
  HASH[21] = 5;
  HASH[22] = 5;
  HASH[23] = -5;
  HASH[24] = -5;
  HASH[25] = -5;
  HASH[26] = -5;
  return 0;
}

int getPos(char *line)
{
  char *tok = (char *)strtok(line,":");
  int i = 0;
  while(tok)
    {
      i++;
      switch (i)
        {
        case 1:
          break;
        case 2:
          break;
        case 3:
          int place = atoi(tok);
          if(place > MAXCOUNT)
            place = 0;
          return place;
          break;
        }
      tok = (char *)strtok(NULL,":");
    }
  return 0;

}

void AddInMap(char * line)
{
  int pos = getPos(line);
  MAP[pos] += 1;
}

int makeHtKey(char *sender, char *rcpt, std::string &returnStr)
{

  char *tok = NULL;
  char *ptr = NULL;
  char dd[6][256];
  memset(dd,0,sizeof(dd));
  ptr = strstr(sender,"@");
  if(!ptr)
    {
      ptr = strstr(sender,".");
      if(ptr)
        {
          int i = 0, j = 0;
          char *tok = strtok(sender,".");
          while(tok)
            {
              strcpy(dd[i++],tok);
              tok = strtok(NULL,".");
            }
          --i;
          for (j=i;j>=0;j--)
            {
              returnStr += dd[j];
              returnStr += ".";
            }
          if(returnStr.length() > 0) {
            returnStr.replace(returnStr.length()-1,1,"");
          }
        } else {
        returnStr += sender;
      }
      return 0;
    } else {
    strncpy(dd[0],sender,(ptr - sender));
    char *tmplink = ptr;
    tok = strtok(tmplink+1,".");
    int i = 1,j=0;
    while(tok)
      {
        strcpy(dd[i++],tok);
        tok = strtok(NULL,".");
      }
    i--;
    for(j=i;j>0;j--)
      {
        returnStr += dd[j];
        returnStr += ".";
      }
    if(returnStr.length() > 0) {
      returnStr.replace(returnStr.length()-1,1,"");
    }
    returnStr += "@";
    returnStr += dd[0];
  }
  ptr = strstr(rcpt,"@");
  if(!ptr)
    {
      returnStr += ":";
      returnStr += rcpt;
      return 0;
    } else {
    memset(dd,0,sizeof(dd));
    returnStr += ":";
    strncpy(dd[0],rcpt,(ptr - rcpt));
    char *tmplink = ptr;
    tok = strtok(tmplink+1,".");
    int i = 1,j=0;
    while(tok)
      {
        strcpy(dd[i++],tok);
        tok = strtok(NULL,".");
      }
    i--;
    for(j=i;j>0;j--)
      {
        returnStr += dd[j];
        returnStr += ".";
      }
    if(returnStr.length() > 0) {
      returnStr.replace(returnStr.length()-1,1,"");
    }
    returnStr += "@";
    returnStr += dd[0];
  }
  return 0;
}


int main(int argc, char **argv) {
  char * senders = NULL;//(argv[1]);
  char * rcptto = NULL;//(argv[2]);
  std::string tablename("");
  std::string columname("");
  //char * action = NULL;
  senders = strdup(argv[1]);
  rcptto = strdup(argv[2]);

  if(argc < 5)
    tablename = "srs";
  else
    tablename = argv[4];
  //action = strdup(argv[3]);

  std::string htkey("");
  int method  = atoi(argv[3]); // for using HQL
  std::string responseStr("");
  int rspcode = 0;
  Hypertable::ThriftGen::BasicTest test;

  Hypertable::ThriftGen::Namespace ns = test.open_namespace("rediffmail");

  memset(MAP,0,sizeof(MAP));
  initHash();
  //printContent();
  makeHtKey(senders,rcptto,htkey);
  //htkey += ":";
  //htkey += action;
  //std::cout << " HT KEY " << htkey <<std::endl;
  rspcode = test.run(htkey,method,ns,tablename,responseStr);
  /*char *rspStr = NULL;
    rspStr = new char[responseStr.length()];
    memset(rspStr,0,responseStr.length());
    sprintf(rspStr,"%s",responseStr.c_str());*/
  //std::cout<< rspcode << " " << responseStr << std::endl;
  size_t start = 0;
  size_t end   = 0;
  size_t lnstr = 0;
  end = responseStr.find(',',start);
  lnstr = end;
  std::string tmpstr;
  //std::cout<< responseStr << std::endl;

  while(end != std::string::npos)
    {
      tmpstr.clear();
      tmpstr = responseStr.substr(start,lnstr);
      start = end+1;
      end = responseStr.find(",",start);
      lnstr = (end - start);
      //std::cout<< "TMP STR [" << tmpstr <<"] start : "<<start << " end : "<<end <<" lnstr " <<lnstr << std::endl;
      char row[256] = {0};
      strcpy(row,tmpstr.c_str());
      AddInMap(row);
    }
  /*for(int i =0; i < 50; i++)
    {
    std::cout << " values : " << i << " " << MAP[i] << std::endl;
    }
    exit(0);*/
  int icount= 0;
  double agg = 0;
  double plus = 0;
  double minus = 0;
  double p=0,n=0;
  double positivescore = 0;
  double negativescore = 0;
  for(icount=0;icount<MAXCOUNT;icount++)
    {
      if(MAP[icount] > 0) {
        if(HASH[icount] > 0) {
          plus += MAP[icount] * HASH[icount];
          p += MAP[icount];
        }else if(HASH[icount] < 0) {
          minus += MAP[icount] * HASH[icount];
          n += MAP[icount];
        }
      }
    }
  if(plus == 0 && p ==0)
    positivescore = 0;
  else if (plus == 0)
    positivescore = 0;
  else
    positivescore = plus/p;

  if(minus == 0 && n ==0)
    negativescore = 0;
  else if (minus == 0)
    negativescore = 0;
  else
    negativescore = minus/n;

  if(positivescore > 0 && negativescore < 0)
    agg = (positivescore + negativescore)/2;
  else if (negativescore < 0)
    agg = negativescore;
  else if (positivescore > 0)
    agg = positivescore;
  else
    agg = 0;

  //cout<<"positivescore : "<<positivescore<<" negativescore : "<<negativescore<<"plus : "<<plus<<" p : "<<p<<" minus : "<<minus<<" n : "<<n<<" agg : "<<agg<<endl;
  //cout << "Key : "<< inputdata <<"Score:" << agg << ":Instances:" <<total<<endl;
  std::cout<<"<score>"<<agg<<"</score>";
  return 0;
}

