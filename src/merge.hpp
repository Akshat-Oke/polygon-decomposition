#ifndef MERGE_HPP
#define MERGE_HPP
#include "dcel.hpp"
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

struct Diagonal
{
  Vertex *vs, *vt;
  int vs_index, vt_index;
  Diagonal(Vertex *vs, Vertex *vt)
  {
    this->vs = vs;
    this->vt = vt;
  }
  void print()
  {
    cout << "Diagonal: ";
    vs->print();
    cout << " -> ";
    vt->print();
    cout << endl;
  }
};

inline vector<Vertex *> original_vertices;
inline DCEL orig_dcel;
inline vector<Diagonal *> LLE;
inline vector<DCEL *> polygons;
inline vector<Diagonal *> lastPolygonDiagonals;
inline vector<bool> LDP; // whether polygon is definite polygon
inline vector<int> LUP;  // LUP[i] = j means that polygon i is part of polygon j

/// @brief Find the next vertex in all polygons
/// @param v
/// @return next verties with their associated polygon index
vector<pair<int, Vertex *>> LP(Vertex *v);

vector<Vertex *> subtract(vector<Vertex *> a, vector<Vertex *> b);
int find_polygon_u_vs_for_vt(Vertex *vt, Vertex *vs);
void initMerge(vector<vector<Vertex *>> decomposition, vector<Vertex *> original_v, vector<vector<Vertex *>> diagonals, bool lastPolygonWasConstructed);
vector<DCEL *> merge(vector<vector<Vertex *>> decomposition, vector<Vertex *> original_v, vector<vector<Vertex *>> diagonals, bool lastPolygonWasConstructed);
void testMerge(vector<vector<Vertex *>> decomposition, vector<Vertex *> original_v, vector<vector<Vertex *>> diagonals, bool lastPolygonWasConstructed);
#endif