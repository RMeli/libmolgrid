#ifndef TEST_UTIL_H
#define TEST_UTIL_H
#include <random>
#include "grid.h"
#include "atom_typer.h"
#include "libmolgrid.h"

using namespace libmolgrid;
inline void make_mol(Grid<float, 2, false>& coords, Grid<float, 1, false>& type_indices,
    Grid<float, 1, false>& radii, 
    size_t natoms = 0, size_t min_atoms = 10,
    size_t max_atoms = 1000, float max_x = 25, float max_y = 25,
    float max_z = 25) {

  if (!natoms) {
    //if not provided, randomly generate the number of atoms
    std::uniform_int_distribution<int> natoms_dist(min_atoms, max_atoms + 1);
    natoms = natoms_dist(random_engine);
  }

  //randomly seed reasonable-ish coordinates and types
  std::uniform_real_distribution<float> coords_dists[3];
  coords_dists[0] = std::uniform_real_distribution<float>(-max_x,
      std::nextafter(max_x, FLT_MAX));
  coords_dists[1] = std::uniform_real_distribution<float>(-max_y,
      std::nextafter(max_y, FLT_MAX));
  coords_dists[2] = std::uniform_real_distribution<float>(-max_z,
      std::nextafter(max_z, FLT_MAX));
  std::uniform_int_distribution<int> type_dist(0, GninaIndexTyper::NumTypes);
  GninaIndexTyper gtyper;

  //set up vector of atoms as well as types
  for (size_t i = 0; i < natoms; ++i) {
    for (size_t j=0; j < 3; ++j) 
      coords[i][j] = coords_dists[j](random_engine);
    int atype = type_dist(random_engine);
    type_indices[i] = (float)atype;
    auto ainfo = gtyper.get_int_type(atype);
    radii[i] = ainfo.second;
  }
}

template <typename Dtype, std::size_t NumDims>
inline bool grid_empty(Grid<Dtype, NumDims, false>& grid) {
  size_t n = grid.size();
  Dtype* data = grid.data();
  for (size_t i=0; i<n; ++i) {
    if (data[i] != 0.0) 
      return false;
  }
  return true;
}

#endif /* TEST_UTIL_H */
