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

#ifndef HYPERTABLE_CELLSTOREINFO_H
#define HYPERTABLE_CELLSTOREINFO_H

#include "CellCache.h"
#include "CellStoreV5.h"

namespace Hypertable {

  class CellStoreInfo {
  public:
    CellStoreInfo(CellStore *csp) :
      cs(csp), shadow_cache_ecr(TIMESTAMP_MAX), shadow_cache_hits(0), bloom_filter_accesses(0),
      bloom_filter_maybes(0), bloom_filter_fps(0) {
      init_from_trailer();
    }
    CellStoreInfo(CellStorePtr &csp) :
      cs(csp), shadow_cache_ecr(TIMESTAMP_MAX), shadow_cache_hits(0), bloom_filter_accesses(0),
      bloom_filter_maybes(0), bloom_filter_fps(0) {
      init_from_trailer();
    }
    CellStoreInfo(CellStorePtr &csp, CellCachePtr &scp, int64_t ecr) :
      cs(csp), shadow_cache(scp), shadow_cache_ecr(ecr), shadow_cache_hits(0),
      bloom_filter_accesses(0), bloom_filter_maybes(0), bloom_filter_fps(0)  {
      init_from_trailer();
    }
    CellStoreInfo() : cell_count(0), shadow_cache_ecr(TIMESTAMP_MAX), shadow_cache_hits(0),
                      bloom_filter_accesses(0), bloom_filter_maybes(0), bloom_filter_fps(0) { }
    void init_from_trailer() {
      int divisor = 0;
      try {
        divisor = (boost::any_cast<uint32_t>(cs->get_trailer()->get("flags")) & CellStoreTrailerV5::SPLIT) ? 2 : 1;
        cell_count = boost::any_cast<int64_t>(cs->get_trailer()->get("total_entries")) / divisor;
        timestamp_min = boost::any_cast<int64_t>(cs->get_trailer()->get("timestamp_min"));
        timestamp_max = boost::any_cast<int64_t>(cs->get_trailer()->get("timestamp_max"));
        expirable_data = boost::any_cast<int64_t>(cs->get_trailer()->get("expirable_data"))
          / divisor ;
      }
      catch (std::exception &e) {
        divisor = 0;
        cell_count = 0;
        timestamp_min = TIMESTAMP_MAX;
        timestamp_max = TIMESTAMP_MIN;
        expirable_data = 0;
      }
      try {
        if (divisor) {
          key_bytes = boost::any_cast<int64_t>(cs->get_trailer()->get("key_bytes")) / divisor;
          value_bytes = boost::any_cast<int64_t>(cs->get_trailer()->get("value_bytes")) /divisor;
        }
        else
          key_bytes = value_bytes = 0;
      }
      catch (std::exception &e) {
        key_bytes = value_bytes = 0;
      }
    }
    CellStorePtr cs;
    CellCachePtr shadow_cache;
    uint64_t cell_count;
    int64_t key_bytes;
    int64_t value_bytes;
    int64_t shadow_cache_ecr;
    uint32_t shadow_cache_hits;
    uint32_t bloom_filter_accesses;
    uint32_t bloom_filter_maybes;
    uint32_t bloom_filter_fps;
    int64_t timestamp_min;
    int64_t timestamp_max;
    int64_t expirable_data;
    int64_t total_data;
  };

} // namespace Hypertable

#endif // HYPERTABLE_CELLSTOREINFO_H
