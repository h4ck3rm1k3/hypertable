/** -*- c++ -*-
 * Copyright (C) 2011 Hypertable, Inc.
 *
 * This file is part of Hypertable.
 *
 * Hypertable is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 of the
 * License, or any later version.
 *
 * Hypertable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */
#include "Common/Compat.h"

#include <boost/algorithm/string.hpp>

#include "LoadBalancerBasic.h"
#include "LoadBalancerBasicDistributeLoad.h"
#include "LoadBalancerBasicDistributeTableRanges.h"

#include "OperationProcessor.h"
#include "OperationBalance.h"

using namespace boost::posix_time;
using namespace Hypertable;
using namespace std;


LoadBalancerBasic::LoadBalancerBasic(ContextPtr context) : LoadBalancer(context), m_waiting_for_servers(false) {
  m_enabled = context->props->get_bool("Hypertable.LoadBalancer.Enable");
}


void LoadBalancerBasic::balance(const String &algorithm) {
  BalancePlanPtr plan = new BalancePlan;
  if (!m_enabled && algorithm == "")
    return;
  try {
    calculate_balance_plan(algorithm, plan);
    if (plan->moves.size()>0) {
      OperationPtr operation = new OperationBalance(m_context, plan);
      m_context->op->add_operation(operation);
    }
    else
      HT_DEBUG_OUT << "No balance plan created, nothing to do" << HT_END;
  }
  catch (Exception &e) {
    if (e.code()==Error::MASTER_OPERATION_IN_PROGRESS) {
      HT_INFO_OUT << e << HT_END;
      return;
    }
    HT_THROW(e.code(), e.what());
  }
}

void LoadBalancerBasic::transfer_monitoring_data(vector<RangeServerStatistics> &stats) {
  ScopedLock lock(m_data_mutex);
  m_range_server_stats.swap(stats);
}

void LoadBalancerBasic::calculate_balance_plan(String algorithm, BalancePlanPtr &balance_plan) {

  vector <RangeServerStatistics> range_server_stats;
  uint32_t mode = BALANCE_MODE_DISTRIBUTE_LOAD;
  ptime now = second_clock::local_time();

  boost::to_upper(algorithm);
  if (algorithm.size() == 0) {
    // determine which balancer to use
    mode = BALANCE_MODE_DISTRIBUTE_LOAD;
    {
      ScopedLock lock(m_data_mutex);
      range_server_stats.swap(m_range_server_stats);
    }

    // when we see a new server wait for next maintenance interval to balance
    if (!m_waiting_for_servers) {
      htforeach(const RangeServerStatistics &server_stats, range_server_stats) {
        if (server_stats.stats->live && server_stats.stats->range_count == 0) {
          mode = BALANCE_MODE_DISTRIBUTE_TABLE_RANGES;
          range_server_stats.swap(m_range_server_stats);
          m_wait_time_start = now;
          m_waiting_for_servers = true;
          break;
        }
      }
    }
    else
      mode = BALANCE_MODE_DISTRIBUTE_TABLE_RANGES;
  }
  else {
    if (algorithm == "TABLE_RANGES") {
      mode = BALANCE_MODE_DISTRIBUTE_TABLE_RANGES;
      m_waiting_for_servers = false;
    }
    else if (algorithm == "LOAD")
      mode = BALANCE_MODE_DISTRIBUTE_LOAD;
    else
      HT_THROW(Error::NOT_IMPLEMENTED, (String)"Unknown LoadBalancer algorithm '" + algorithm
          + "' supported algorithms are 'TABLE_RANGES', 'LOAD'");
  }

  // TODO: write a factory class to create the sub balancer objects

  if (mode == BALANCE_MODE_DISTRIBUTE_LOAD && !m_waiting_for_servers) {
    distribute_load(now, balance_plan);
  }
  else {
    time_duration td = now - m_wait_time_start;
    if (m_waiting_for_servers && td.total_milliseconds() > m_balance_wait)
      m_waiting_for_servers = false;
    if (!m_waiting_for_servers)
      distribute_table_ranges(range_server_stats, balance_plan);
  }

  if (balance_plan->moves.size()) {
    if (mode == BALANCE_MODE_DISTRIBUTE_LOAD) {
      HT_INFO_OUT << "LoadBalancerBasic mode=BALANCE_MODE_DISTRIBUTE_LOAD" << HT_END;
      m_last_balance_time = now;
    }
    else
      HT_INFO_OUT << "LoadBalancerBasic mode=BALANCE_MODE_DISTRIBUTE_TABLE_RANGES" << HT_END;
    HT_INFO_OUT << "BalancePlan = " << *balance_plan << HT_END;
  }
}

void LoadBalancerBasic::distribute_load(const ptime &now, BalancePlanPtr &balance_plan) {
  // check to see if m_balance_interval time has passed since the last balance
  time_duration td = now - m_last_balance_time;
  if (td.total_milliseconds() < m_balance_interval)
    return;
  // check to see if it is past time time of day when the balance should occur
  td = now.time_of_day();
  if (td < m_balance_window_start || td > m_balance_window_end)
    return;

  HT_INFO_OUT << "Current time = " << td << ", m_balance_window_start="
      << m_balance_window_start << ", m_balance_window_end="
      << m_balance_window_end << HT_END;

  LoadBalancerBasicDistributeLoad planner(m_balance_loadavg_threshold,
                                          m_context->rs_metrics_table);
  planner.compute_plan(balance_plan);
  return;
}

void LoadBalancerBasic::distribute_table_ranges(vector<RangeServerStatistics> &range_server_stats, BalancePlanPtr &balance_plan) {
  // no need to check if its time to do balance, we have empty servers, so do balance
  LoadBalancerBasicDistributeTableRanges  planner(m_context->metadata_table);
  planner.compute_plan(range_server_stats, balance_plan);
  return;
}
