#include <iostream>
#include <bits/stdc++.h>
#include "dcel.cpp"
 
using namespace std;
 
vector<Vertex *> decompose(vector<Vertex *> vertices);
Vertex *next_v(vector<Vertex *> vertices, Vertex *v);
vector<Vertex *> read_vertices(char *filename);
bool colinear(vector<Vertex *> polygon);
 
vector <vector <Vertex *>> decomposition;
 
void testDCEL(vector<Vertex *> LPVS)
{
  // DCEL dcel;
  // dcel.initialize(vertices);
  // Vertex v{300, 250};
//   auto min_y = min_element(LPVS.begin(), LPVS.end(), [](Vertex *a, Vertex *b)
//                            { return a->x < b->x; });
//   (*min_y)->print();
  std::cout << colinear(LPVS);
}
vector<Vertex *> rotate_forward(vector<Vertex *> v)
{
  vector<Vertex *> result;
  for (int i = 1; i < v.size(); i++)
  {
    result.push_back(v[i]);
  }
  result.push_back(v[0]);
  return result;
}
int main(int argc, char *argv[])
{
    decomposition.clear();
  vector<Vertex *> vertices;
  vertices = read_vertices(argv[1]);
  auto new_p_vertices = decompose(vertices);
  while (new_p_vertices.size() > 3)
  {
    new_p_vertices = rotate_forward(new_p_vertices);
    new_p_vertices = decompose(new_p_vertices);
  }
//   testDCEL(vertices);
}
 
bool colinear(vector<Vertex *> polygon)
{
    int x_neq = 0, y_neq = 0;
 
    for (int i = 1; i < polygon.size(); i++)
    {
        if (polygon[i]->x != polygon[i-1]->x)
            x_neq = 1;
 
        if (polygon[i]->y != polygon[i-1]->y)
            y_neq = 1;
 
        if (x_neq && y_neq)
          break;
    }
 
    if (!(x_neq && y_neq))
        return true;
 
    float curr_m, prev_m;
 
    for (int i = 1; i < polygon.size(); i++)
    {
        if (polygon[i]->x == polygon[i-1]->x || polygon[i]->y == polygon[i-1]->y)
            return false;
 
        curr_m = (polygon[i]->y - polygon[i-1]->y)/(polygon[i]->x - polygon[i-1]->x);
 
        if (i > 1)
            if (curr_m != prev_m)
                return false;
 
        prev_m = curr_m;
    }
 
    return true;
} 
 
Vertex *next_v(vector<Vertex *> vertices, Vertex *v)
{
  int i = 0;
  for (; i < vertices.size(); i++)
  {
    if (vertices[i] == v)
    {
      break;
    }
  }
  return vertices[(i + 1) % vertices.size()];
}
 
vector<Vertex *> subtract(vector<Vertex *> a, vector<Vertex *> b)
{
  vector<Vertex *> result;
  for (auto v : a)
  {
    if (find(b.begin(), b.end(), v) == b.end())
    {
      result.push_back(v);
    }
  }
  return result;
}
 
struct Rectangle
{
  double x1, y1, x2, y2;
  Rectangle(double x1, double y1, double x2, double y2)
  {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
  }
  bool contains(Vertex *v)
  {
    return v->x >= x1 && v->x <= x2 && v->y >= y1 && v->y <= y2;
  }
};
void print(vector<Vertex *> v)
{
  for (auto i : v)
  {
    cout << i->x << " " << i->y << endl;
  }
}
 
vector<Vertex *> decompose(vector<Vertex *> vertices)
{
  DCEL dcel_p, dcel_l;
  dcel_p.initialize(vertices);
  vector<Vertex *> L = {vertices[0]};
  cout << "init" << dcel_p.vertices.size() << endl;
 
  while (dcel_p.vertices.size() > 3)
  {
    cout << "dcel_p.vertices.size() = " << dcel_p.vertices.size() << endl;
    // cout << "Notches are\n";
    auto notches_in_p = dcel_p.get_notches();
    // print(notches_in_p);
    vector<Vertex *> v; // = dcel_p.vertices;
    /// v(1) = Last[L]
    v.push_back(L.back());
    // get next vertex to L.back() in dcel_p.vertices
    // add it to v
    auto next_vertex = next_v(dcel_p.vertices, v[0]);
    /// v(2) = Next[P, v(1)]
    v.push_back(next_vertex);
    /// L = {v(1), v(2)}
    L.clear();
    L.push_back(v[0]);
    L.push_back(v[1]);
    /// i = 2
    int i = 1;
    /// v(i+1) = Next[P, v(i)]
    v.push_back(next_v(dcel_p.vertices, v[i]));
    while (
        ang_leq_180(v[i - 1], v[i], v[i + 1]) &&
        ang_leq_180(v[i], v[i + 1], v[0]) &&
        ang_leq_180(v[i + 1], v[0], v[1]) &&
        L.size() < dcel_p.vertices.size())
    {
      /// L = L + {v(i+1)}
      L.push_back(v[i + 1]);
      /// i = i + 1
      i++;
      /// v(i+1) = Next[P, v(i)]
      v.push_back(next_v(dcel_p.vertices, v[i]));
    } // while
    if (L.size() != dcel_p.vertices.size())
    {
      /// P = P - L
      auto LPVS = subtract(dcel_p.vertices, L);
      auto no_notches = subtract(LPVS, notches_in_p);
      LPVS = subtract(LPVS, no_notches);
      print(LPVS);
      while (LPVS.size() > 0)
      {
        auto min_x = min_element(LPVS.begin(), LPVS.end(), [](Vertex *a, Vertex *b)
                                 { return a->x < b->x; });
        auto min_y = min_element(LPVS.begin(), LPVS.end(), [](Vertex *a, Vertex *b)
                                 { return a->y < b->y; });
        auto max_x = max_element(LPVS.begin(), LPVS.end(), [](Vertex *a, Vertex *b)
                                 { return a->x < b->x; });
        auto max_y = max_element(LPVS.begin(), LPVS.end(), [](Vertex *a, Vertex *b)
                                 { return a->y < b->y; });
        Rectangle r((*min_x)->x, (*min_y)->y, (*max_x)->x, (*max_y)->y);
        bool backward = false;
        while (!backward and LPVS.size() > 0)
        {
          while (true)
          {
            if (LPVS.size() == 0)
              break;
            /// v = First(LPVS)
            Vertex *v = LPVS[0];
            cout << "v is ";
            v->print();
            cout << endl;
            if (!r.contains(v))
            {
              /// LPVS = LPVS - {v}
              LPVS.erase(remove(LPVS.begin(), LPVS.end(), v), LPVS.end());
              cout << "Erased, now size: " << LPVS.size() << endl;
            }
            else
              break;
          }
          if (LPVS.size() > 0)
          {
            DCEL dcel_l;
            dcel_l.initialize(L);
            if (dcel_l.is_inside(LPVS[0]))
            {
              vector<Vertex *> VTR;
              bool side = ang_leq_180(LPVS[0], v[0], L.back());
              for (auto l_v : dcel_l.vertices)
              {
                if (ang_leq_180(LPVS[0], v[0], l_v) == side)
                {
                  VTR.push_back(l_v);
                }
              }
              for (auto vtr_v : VTR)
              {
                L.erase(remove(L.begin(), L.end(), vtr_v), L.end());
              }
              backward = true;
            }
            LPVS.erase(remove(LPVS.begin(), LPVS.end(), LPVS[0]), LPVS.end());
          } // if lpvs > 0
        }   // while !backward and lpvs > 0
      }     // while lpvs > 0
    }
    if (L.back() != v[1]) // if L has atleast 3 vertices
    {
    //   cout << "Found convex polygon" << endl;
      auto p_new = subtract(dcel_p.vertices, L);
      vector<Vertex *> first_last_l = {L.front(), L.back()};
      auto p_nn = subtract(dcel_p.vertices, p_new);
      p_nn = subtract(p_nn, first_last_l);
 
      dcel_p.vertices = subtract(dcel_p.vertices, p_nn);
 
      // dcel_p.vertices = subtract(dcel_p.vertices, L);
    //   print(L);
 
      if (!colinear(L))
      {
        decomposition.push_back(L);
        std::cout << "Found convex polygon" << std::endl;
        print(L);
        
      }
      // insert L.back() as first of dcel_p.vertices
      // dcel_p.vertices.insert(dcel_p.vertices.begin(), L.back());
      // dcel_p.vertices.insert(dcel_p.vertices.begin(), L.front());
      cout << "----New P----\n";
      print(dcel_p.vertices);
      dcel_p.init();
    }
    else
    {
      // L has 2 vertices
      cout << "L failed to form a convex polygon" << endl;
      return dcel_p.vertices;
    }
  }
  return dcel_p.vertices;
}
 
vector<Vertex *> read_vertices(char *filename)
{
  vector<Vertex *> vertices;
  ifstream file;
  file.open(filename);
  int n;
  file >> n;
  for (int i = 0; i < n; i++)
  {
    double x, y;
    file >> x >> y;
    Vertex *v = new Vertex;
    v->x = x;
    v->y = y;
    vertices.push_back(v);
  }
  return vertices;
}
