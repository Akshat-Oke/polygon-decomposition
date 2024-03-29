/** \file
 * Merge functions.
 * This contains the implementation of the merging algorithm.
 */
#include "dcel.hpp"
#include "merge.hpp"
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

void testMerge(vector<vector<Vertex *>> decomposition, vector<Vertex *> original_v, vector<vector<Vertex *>> diagonals, bool lastPolygonWasConstructed)
{
  initMerge(decomposition, original_v, diagonals, lastPolygonWasConstructed);
  // is_notch for every vertex
  for (int i = 0; i < original_vertices.size(); i++)
  {
    auto v = original_vertices[i];
    cout << "is_notch(" << v->x << ", " << v->y << "): " << orig_dcel.is_notch(v) << endl;
  }
  // after initMerge
  for (int i = 0; i < LLE.size(); i++)
  {
    auto p = polygons[i];
    cout << "Polygon " << i << endl;
    LLE[i]->print();
    p->print();
  }
  cout << "LP(v)" << endl;
  for (int i = 0; i < original_v.size(); i++)
  {
    auto v = original_v[i];
    auto LP_v = LP(v);
    cout << "v: ";
    v->print();
    for (int j = 0; j < LP_v.size(); j++)
    {
      cout << "polygon " << LP_v[j].first << ": ";
      LP_v[j].second->print();
    }
    cout << endl;
  }
  cout << "u for vs, vt" << endl;
  for (int i = 0; i < LLE.size(); i++)
  {
    auto vs = LLE[i]->vs;
    auto vt = LLE[i]->vt;
    cout << "vs: ";
    vs->print();
    cout << "vt: ";
    vt->print();
    cout << "u: " << find_polygon_u_vs_for_vt(vt, vs) << endl;
  }
  cout << "Last Polygon added ";
  cout << lastPolygonWasConstructed << endl;
  for (auto d : lastPolygonDiagonals)
  {
    d->print();
  }
}
void print_vector(vector<int> v)
{
  for (int i = 0; i < v.size(); i++)
  {
    cout << v[i] << " ";
  }
  cout << endl;
}
/**
 * @brief Tries to merge polygons in the decomposition
 *
 * Polygons are merged such that the resulting polygon
 * stays convex.
 *
 * @param decomposition
 * @param original_v Original polygon's vertices
 * @param diagonals The diagonals that form the decomposition. These must not be the sides of the original polygon.
 * @param lastPolygonWasConstructed If the last polygon of the decomposition was constructed from the diagonals.
 * @return vector<DCEL *>
 */
vector<DCEL *> merge(vector<vector<Vertex *>> decomposition, vector<Vertex *> original_v, vector<vector<Vertex *>> diagonals, bool lastPolygonWasConstructed)
{
  initMerge(decomposition, original_v, diagonals, lastPolygonWasConstructed);
  int m = LLE.size();
  int NP = m + 1;
  NP += lastPolygonWasConstructed;
  for (int j = 0; j < m; j++)
  {
    // cout << "\n---New iteration---NP is " << NP << " j=" << j << endl;
    auto vs = LLE[j]->vs;
    auto vt = LLE[j]->vt;
    auto LP_vs = LP(vs);
    auto LP_vt = LP(vt);
    // cout << "LUP\n";
    // print_vector(LUP);
    if (
        true ||
        (LP_vs.size() >= 2 && LP_vs.size() >= 2) ||
        (LP_vs.size() >= 2 && !orig_dcel.is_notch(vt)) ||
        (LP_vt.size() >= 2 && !orig_dcel.is_notch(vs)) ||
        (!orig_dcel.is_notch(vs) && !orig_dcel.is_notch(vt)))
    {
      auto j2 = vt, i2 = vs;
      // cout << "Inspecting diagonal ";
      // LLE[j]->print();

      auto j3 = polygons[LUP[j]]->next_v(vt);
      auto i1 = polygons[LUP[j]]->prev_v(vs);
      int u = find_polygon_u_vs_for_vt(vt, vs);
      if (u == -1)
      {
        // cout << "No diagonal found for " << j << endl;
        // exit(1);
        continue;
      }
      // cout << "Found u as " << u << endl;
      // cout << "LUP.size() = " << LUP.size() << endl;
      auto j1 = polygons[LUP[u]]->prev_v(vt);
      auto i3 = polygons[LUP[u]]->next_v(vs);
      // cout << "Checking angle for ";
      // i1->print();
      // i2->print();
      // i3->print();
      // cout << " and ";
      // j1->print();
      // j2->print();
      // j3->print();
      if (ang_leq_180(i1, i2, i3) && ang_leq_180(j1, j2, j3))
      {
        // cout << "Found polygon merging " << j << " and " << u << endl;
        auto new_polygon_vertices = subtract(original_vertices, polygons[LUP[j]]->vertices);
        new_polygon_vertices = subtract(new_polygon_vertices, polygons[LUP[u]]->vertices);
        new_polygon_vertices = subtract(original_vertices, new_polygon_vertices);
        if (*(new_polygon_vertices.front()) == *(new_polygon_vertices.back()))
        {
          new_polygon_vertices.pop_back();
        }

        LDP[j] = false;
        LDP[u] = false;
        // LDP[NP] = true;
        if (NP - 1 < LDP.size())
        {
          LDP[NP - 1] = true;
        }
        else
        {
          LDP.push_back(true);
        }
        int saved_LUP_j = LUP[j];
        int saved_LUP_u = LUP[u];
        LUP[j] = NP - 1;
        LUP[u] = NP - 1;
        if (NP - 1 < LUP.size())
        {
          LUP[NP - 1] = NP - 1;
        }
        else
        {
          LUP.push_back(NP - 1);
        }
        for (int h = 0; h < LUP.size(); h++)
        {
          // if (LUP[h] == j || LUP[h] == u)
          // {
          //   LUP[h] = NP - 1;
          // }//?
          if (LUP[h] == saved_LUP_j || LUP[h] == saved_LUP_u)
            LUP[h] = NP - 1;
        }

        NP += 1;

        // cout << "\n\nNew poly vertices are:\n";
        // for (auto v : new_polygon_vertices)
        // {
        //   v->print();
        // }
        DCEL *new_polygon = new DCEL;
        new_polygon->initialize(new_polygon_vertices);
        // cout << "Setting " << NP - 2 << "th polygon as new polygon\n";
        // cout << "\n\n";
        if (NP - 2 < polygons.size())
        {
          polygons[NP - 2] = new_polygon;
        }
        else
        {
          polygons.push_back(new_polygon);
        }
        // cout << "\nDone processing new polygon\n";
      }
    }
  }
  unordered_map<int, bool> visited;
  vector<DCEL *> final_polygons;
  for (int i = 0; i < LUP.size(); i++)
  {
    if (!visited[LUP[i]])
    {
      visited[LUP[i]] = true;
      final_polygons.push_back(polygons[LUP[i]]);
    }
  }
  // for (int i = 0; i < LUP.size(); i++)
  // {
  //   if (LDP[i])
  //   {
  //     final_polygons.push_back(polygons[i]);
  //   }
  // }

  return final_polygons;
}
/**
 * @brief Find the polygon u which contains the diagonal (vt, vs)
 *
 * Find in LP(vt) the only pair (u, vs) containing vs.
 * vs is the next vertex of vt in the polygon u.
 *
 * @param vt Start vertex of diagonal
 * @param vs End vertex of diagonal
 * @return int The index of the polygon
 */
int find_polygon_u_vs_for_vt(Vertex *vt, Vertex *vs)
{
  auto LP_vt = LP(vt);
  for (auto p : LP_vt)
  {
    if (p.second == vs)
    {
      return p.first;
    }
  }
  for (auto d : lastPolygonDiagonals)
  {
    if (d->vs == vs && d->vt == vt)
    {
      return LUP[polygons.size() - 1];
    }
  }
  return -1;
}
/**
 * @brief Sets up the global variables for the merge algorithm
 *
 * @param decomposition (Unmerged) decomposition
 * @param original_v Original vertices in the non-convex polygon
 * @param diagonals Diagonals of the decomposition
 * @param lastPolygonWasConstructed Whether the last polygon was constructed only from the diagonals
 */
void initMerge(vector<vector<Vertex *>> decomposition, vector<Vertex *> original_v, vector<vector<Vertex *>> diagonals, bool lastPolygonWasConstructed)
{
  static bool initialized = false;
  if (initialized)
    return;
  cout << "Last polygon was constructed" << lastPolygonWasConstructed << endl;
  original_vertices = original_v;
  for (auto polygon : decomposition)
  {
    DCEL *dcel = new DCEL();
    dcel->initialize(polygon);
    polygons.push_back(dcel);
  }
  for (auto diagonal : diagonals)
  {
    LLE.push_back(new Diagonal(diagonal[0], diagonal[1]));
  }
  LDP.resize(polygons.size());
  LUP.resize(polygons.size());
  for (int i = 0; i < LDP.size(); i++)
  {
    LDP[i] = true;
    LUP[i] = i;
  }
  // initialize original dcel
  orig_dcel.initialize(original_vertices);

  if (lastPolygonWasConstructed)
  {
    auto lastPolygonV = polygons.back()->vertices;
    for (int i = 0; i < lastPolygonV.size(); i++)
    {
      Diagonal *d = new Diagonal(lastPolygonV[i], lastPolygonV[(i + 1) % lastPolygonV.size()]);
      lastPolygonDiagonals.push_back(d);
    }
  }
  initialized = true;
}
/**
 * @brief Get list of (polygon, vv) pairs for which vv is the other vertex of a diagonal in polygon
 *
 * LP(vj) is a list containing pairs (k, vr)
 * where k is the index of a polygon containing vj as
 * one of its vertices and vr is the next vertex to vj in
 * that polygon k, but these pairs are in LPvj only if vr
 * is not the consecutive vertex of vj in the initial
 * polygon, i.e., if vjvr is a diagonal,
 *
 * @param v vj
 * @return vector<pair<int, Vertex *>>
 */
vector<pair<int, Vertex *>> LP(Vertex *v)
{
  vector<pair<int, Vertex *>> result;

  for (int ip = 0; ip < polygons.size(); ip++)
  {
    auto polygon_dcel = polygons[LUP[ip]];
    auto vertices = polygon_dcel->vertices;
    for (int i = 0; i < vertices.size(); i++)
    {
      auto vertex = vertices[i];
      auto next_vertex = vertices[(i + 1) % vertices.size()];
      if (vertex == v && next_v(original_vertices, vertex) != next_vertex)
      {
        result.push_back(make_pair(LUP[ip], next_vertex));
      }
    }
  }
  for (auto d : lastPolygonDiagonals)
  {
    if (d->vs == v && next_v(original_vertices, v) != d->vt)
    {
      result.push_back(make_pair(LUP[polygons.size() - 1], d->vt));
    }
  }
  return result;
}
