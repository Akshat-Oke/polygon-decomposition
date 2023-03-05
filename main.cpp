#include <iostream>
#include <bits/stdc++.h>
#include "dcel.cpp"

using namespace std;

void decompose(vector<Vertex *> vertices);
vector<Vertex *> read_vertices(char *filename);

int main(int argc, char *argv[])
{
  vector<Vertex *> vertices;
  vertices = read_vertices(argv[1]);
  decompose(vertices);
  // for (auto v : vertices)
  // {
  //   cout << v->x << " " << v->y << endl;
  // }
  // Vertex *v = vertices[0];
  // cout << "Removed one" << endl;
  // vertices.erase(remove(vertices.begin(), vertices.end(), v), vertices.end());
  // for (auto v : vertices)
  // {
  //   cout << v->x << " " << v->y << endl;
  // }
}

Vertex *next_v(vector<Vertex *> vertices, Vertex *v)
{
  auto it = find(vertices.begin(), vertices.end(), v);
  if (it != vertices.end())
  {
    return *(it + 1);
  }
  else
  {
    return vertices[0];
  }
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

void decompose(vector<Vertex *> vertices)
{
  DCEL dcel_p, dcel_l;
  dcel_p.initialize(vertices);
  vector<Vertex *> L = {vertices[0]};
  cout << "init" << dcel_p.vertices.size() << endl;

  while (dcel_p.vertices.size() > 3)
  {
    cout << "dcel_p.vertices.size() = " << dcel_p.vertices.size() << endl;
    cout << "Notches are\n";
    auto notches_in_p = dcel_p.get_notches();
    print(notches_in_p);
    vector<Vertex *> v;
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
      LPVS = subtract(LPVS, notches_in_p);
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
          /// v = First(LPVS)
          while (true)
          {
            Vertex *v = LPVS[0];
            if (!r.contains(v))
            {
              cout << "asd" << endl;
              /// L = L + {v}
              L.push_back(v);
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
              backward = true;
              vector<Vertex *> VTR;
              bool on_left = ang_leq_180(LPVS[0], v[0], L.back());
              for (auto l_v : dcel_l.vertices)
              {
                if (ang_leq_180(LPVS[0], v[0], l_v) == on_left)
                {
                  VTR.push_back(l_v);
                }
              }
              for (auto vtr_v : VTR)
              {
                L.erase(remove(L.begin(), L.end(), vtr_v), L.end());
              }
            }
            LPVS.erase(remove(LPVS.begin(), LPVS.end(), LPVS[0]), LPVS.end());
          }
        }
      }
    }
    if (L.back() != v[1])
    {
      cout << "Found convex polygon" << endl;
      dcel_p.vertices = subtract(dcel_p.vertices, L);
      // insert L.back() as first of dcel_p.vertices
      dcel_p.vertices.insert(dcel_p.vertices.begin(), L.back());
      dcel_p.vertices.push_back(L.front());
      dcel_p.init();
    }
  }
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