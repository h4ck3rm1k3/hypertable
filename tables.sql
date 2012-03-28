CREATE TABLE way (
  id,
  version,
  nodes,
  geohashs,
  tags,
  'in_relations',
  changeset,
  ACCESS GROUP default (id, version, nodes, geohashs, tags, 'in_relations', changeset)
);

CREATE TABLE changeset (
  id,
  bbox,
  nodes,
  ways,
  relations,
  opentime,
  closetime,
  ACCESS GROUP default (id, bbox, nodes, ways, relations, opentime, closetime)
);

CREATE TABLE user (
  id,
  name,
  email,
  password,
  changesets,
  ACCESS GROUP default (id, name, email, password, changesets)
);
CREATE TABLE node (
  id,
  version,
  lat,
  lon,
  geohash,
  tags,
  'in_ways',
  'in_relations',
  changeset,
  ACCESS GROUP default (id, version, lat, lon, geohash, tags, 'in_ways', 'in_relations', changeset)
);

CREATE TABLE relation (
  id,
  version,
  nodes,
  ways,
  relations,
  geohashs,
  tags,
  'in_relations',
  changeset,
  ACCESS GROUP default (id, version, nodes, ways, relations, geohashs, tags, 'in_relations', changeset)
);