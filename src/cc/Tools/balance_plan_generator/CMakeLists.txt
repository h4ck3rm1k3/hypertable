#
# Copyright (C) 2009 Sanjit Jhala (Zvents, Inc.)
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.
#

# hypertable - command interpreter
add_executable(ht_balance_plan_generator ht_balance_plan_generator.cc)
target_link_libraries(ht_balance_plan_generator Hypertable HyperMaster)

if (NOT HT_COMPONENT_INSTALL)
  install (TARGETS ht_balance_plan_generator RUNTIME DESTINATION bin)
endif ()
