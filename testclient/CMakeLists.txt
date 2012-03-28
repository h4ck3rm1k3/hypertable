cmake_minimum_required(VERSION 2.8)
#find_package(Boost REQUIRED)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED OFF)
#find_package( Boost 1.48.0 COMPONENTS date_time filesystem system threads )
SET (Boost_ADDITIONAL_VERSIONS "1.39" "1.39.0" "1.40" "1.40.0" "1.41" "1.41.0" "1.42" "1.42.0" "1.43" "1.43.0" "1.44" "1.44.0" "1.45" "1.45.0" "1.46" "1.46.0" "1.47" "1.47.0" "1.48" "1.48.0" "1.49" "1.49.0" "1.50" "1.50.0")
FIND_PACKAGE (Boost 1.48 COMPONENTS "date_time" "filesystem" "graph" "iostreams" "program_options" "regex" "serialization" "signals" "system" "thread" "wserialization" REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

find_package(ZLIB REQUIRED)

## sigar, why do we need this?
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(SIGAR REQUIRED)


# src/cc/Hypertable/Lib/Schema.cc:181: undefined reference to `XML_ParserCreate'
find_package(EXPAT REQUIRED)
find_package(Readline REQUIRED)

find_package(Log4cpp REQUIRED)

include_directories(/home/mdupont/OSM-API-Proxy/lib/todo/db-5.2.36/build_unix/  /opt/hypertable/0.9.5.0/include/ )
link_directories(/opt/hypertable/0.9.5.0/lib/)
add_executable(testclient testclient.cpp)



target_link_libraries(
  testclient 

  Hypertable 
  HyperComm 
  HyperCommon 
  HyperDfsBroker 
  HyperDfsCmds 
  HyperMaster 
  HyperRanger 
  HyperTools 
  Hyperspace 

/opt/hypertable/0.9.5.0/lib/libHyperComm.a    
/opt/hypertable/0.9.5.0/lib/libHyperDfsCmds.a  
/opt/hypertable/0.9.5.0/lib/libHyperTools.a  
/opt/hypertable/0.9.5.0/lib/libHyperCommon.a     
/opt/hypertable/0.9.5.0/lib/libHyperMaster.a  
/opt/hypertable/0.9.5.0/lib/libHyperspace.a  
/opt/hypertable/0.9.5.0/lib/libHyperDfsBroker.a 
/opt/hypertable/0.9.5.0/lib/libHyperRanger.a  
/opt/hypertable/0.9.5.0/lib/libHypertable.a  

  /opt/hypertable/0.9.5.0/lib/libdb_cxx-5.2.a
  /opt/hypertable/0.9.5.0/lib/libdb_stl-5.2.a
  /opt/hypertable/0.9.5.0/lib/libdb-5.2.a
  ${Boost_FILESYSTEM_LIBRARY}  
  ${Boost_SYSTEM_LIBRARY} 
  ${Boost_THREAD_LIBRARY} 
  ${Boost_LIBRARIES}  
  ${SIGAR_LIBRARIES}
  ${EXPAT_LIBRARIES}
  ${Log4cpp_LIBRARIES}
  ${NCURSES_LIBRARY}
  ${READLINE_LIBRARY}
  ${ZLIB_LIBRARY}
  )
