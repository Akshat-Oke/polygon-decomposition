/**
 * @file main.cpp
 * @brief Decomposes a polygon into convex polygons
 * @version 0.1
 * @date 2023-03-19
 * @authors Akshat, Arya, Sanidhya, Kushal
 *
 * IDs: 2020A7PS0284H, 2020A7TS2068H, 2020A7PS2056H, 2020A7PS2083H
 *
 * This has the main decomposition algorithm.
 *
 */
#include <chrono>
#include <iostream>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstdlib>
#include "dcel.hpp"
#include "merge.hpp"

using namespace std;

vector<Vertex *> decompose(vector<Vertex *> vertices);
Vertex *next_v(vector<Vertex *> vertices, Vertex *v);
vector<Vertex *> read_vertices(char *filename);
bool colinear(vector<Vertex *> polygon);

/**
 * @brief Final decomposition
 */
vector<vector<Vertex *>> decomposition;
//! Diagonals of the decomposition
vector<vector<Vertex *>> diagonals;
//! Polygon constructed from only diagonals
/*!
 * The decompisition may include an _"inside"_ polygon,
 * which has none of the original polygon sides; but is
 * constructed from diagonals.
 * This polygon will always be the last in #decomposition
 */
bool lastPolygonWasConstructed = false;

int number_of_lines(char *filename)
{
  int number_of_lines = 0;
  std::string line;
  std::ifstream myfile(filename);

  while (std::getline(myfile, line))
    ++number_of_lines;
  // std::cout << "Number of lines in text file: " << number_of_lines;
  return number_of_lines;
}

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
/**
 * @brief Rotates a vector
 *
 * [1,2,3] will be [2,3,1]
 *
 * @param v
 * @return vector<Vertex *>
 */
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

void writePolygons(vector<DCEL *> polygons, char *filename)
{
  // cout << "Open output.txt to see the output\nFormat:\n[total polygons]\n[vertices in polygon 1]\n[x1 y1]\n[x2 y2]\n[vertices in polygon 2]\n[x1 y1]\n[x2 y2]\n...\n";
  ofstream myfile;
  string filename_str = filename;
  myfile.open(filename_str + "_merged.txt");
  myfile << polygons.size() << endl;
  for (int i = 0; i < polygons.size(); i++)
  {
    auto p = polygons[i];
    // myfile << i << endl;
    myfile << p->vertices.size() << endl;
    for (int j = 0; j < p->vertices.size(); j++)
    {
      myfile << p->vertices[j]->x << " " << p->vertices[j]->y << endl;
    }
  }
  myfile.close();
}
/**
 * @brief Writes the decomposition to a file
 *
 * @param decomposition Vector of polygons as a vector of vertices
 * @param filename The filename to write to
 */
void writeDecomposition(vector<vector<Vertex *>> decomposition, char *filename)
{
  // cout << "Open decomposition.txt to see the output\nFormat:\n[total polygons]\n[vertices in polygon 1]\n[x1 y1]\n[x2 y2]\n[vertices in polygon 2]\n[x1 y1]\n[x2 y2]\n...\n";
  ofstream myfile;
  string filename_str = filename;
  myfile.open(filename_str + "_decomp.txt");
  myfile << decomposition.size() << endl;
  for (int i = 0; i < decomposition.size(); i++)
  {
    auto p = decomposition[i];
    // myfile << i << endl;
    myfile << p.size() << endl;
    for (int j = 0; j < p.size(); j++)
    {
      myfile << p[j]->x << " " << p[j]->y << endl;
    }
  }
  myfile.close();
}
struct RunningTime
{
  /// Time taken to decompose in microseconds
  int decompose;
  int merge;
  int total;
  int notches;
};

/**
 * @brief Decompose a polygon in the file
 *
 * Decomposes and merges the decomposition for
 * the testcase in the file.
 *
 * @param filename The polygon testcase file
 */
RunningTime run_for_testcase(char *filename)
{
  RunningTime rt;
  decomposition.clear();
  vector<Vertex *> vertices;
  vertices = read_vertices(filename);
  auto vertices_copy = vertices;
  DCEL dcel;
  dcel.initialize(vertices);
  rt.notches = dcel.get_notches().size();
  auto start = chrono::high_resolution_clock::now();
  auto new_p_vertices = decompose(vertices);
  bool addPolygon = false;
  int same_size_count = 0;
  while (new_p_vertices.size() > 3)
  {
    cout << "Hello " << new_p_vertices.size() << endl;
    new_p_vertices = rotate_forward(new_p_vertices);
    int old_size = new_p_vertices.size();
    new_p_vertices = decompose(new_p_vertices);
    if (old_size == new_p_vertices.size())
    {
      same_size_count++;
    }
    else
    {
      same_size_count = 0;
    }
    if (same_size_count > old_size)
    {
      addPolygon = true;
      break;
    }
  }
  // add new_p_vertices as a decomposition if it has 3 vertices
  if (addPolygon || (new_p_vertices.size() == 3) && !colinear(new_p_vertices))
  {
    lastPolygonWasConstructed = true;
    decomposition.push_back(new_p_vertices);
  }
  auto end = chrono::high_resolution_clock::now();
  rt.decompose = chrono::duration_cast<chrono::microseconds>(end - start).count();
  writeDecomposition(decomposition, filename);
  // testMerge(decomposition, vertices_copy, diagonals, lastPolygonWasConstructed);
  start = chrono::high_resolution_clock::now();
  auto final_polygons = merge(decomposition, vertices_copy, diagonals, lastPolygonWasConstructed);
  end = chrono::high_resolution_clock::now();
  rt.merge = chrono::duration_cast<chrono::microseconds>(end - start).count();
  rt.total = rt.decompose + rt.merge;
  // auto final_polygons = decomposition;
  // cout << "----Final merged polyons----\n";
  // for (auto p : final_polygons)
  // {
  //   // cout << "\n---Final polygon---\n";
  //   // p->print();
  // }
  writePolygons(final_polygons, filename);
  return rt;
}
/**
 * @brief Run for all files ending with .txt in this directory
 *
 * Writes the [numPoints, time] for each testcase to timelog.txt
 * Times are written in microseconds.
 *
 * @param directory Directory with txt files of polygon testcases
 */
void run_and_time_for_directory(char *directory)
{
  DIR *dir = opendir(directory);
  ofstream timelogfile;
  timelogfile.open("timelog.txt", ios::app);
  timelogfile << "n notches decompose merge total" << endl;
  struct dirent *ent;
  while ((ent = readdir(dir)) != NULL)
  {
    if (ent->d_name[0] == '.')
      continue;
    char *filename = (char *)malloc(100);
    strcpy(filename, directory);
    strcat(filename, "/");
    strcat(filename, ent->d_name);
    // check if extension is .txt
    if (filename[strlen(filename) - 1] != 't' || filename[strlen(filename) - 2] != 'x' || filename[strlen(filename) - 3] != 't' || filename[strlen(filename) - 4] != '.')
      continue;
    cout << "Running for " << filename << endl;
    auto rt = run_for_testcase(filename);
    ifstream file;
    file.open(filename);
    int n;
    file >> n;
    file.close();
    timelogfile << n << " " << rt.notches << " " << rt.decompose << " " << rt.merge << " " << rt.total << endl;
  }
  closedir(dir);
  timelogfile.close();
}
/**
 * @brief Run for one file
 *
 * Writes the time taken for each testcase to timelog.txt
 *
 * @param filename
 */
void run_and_time_one_file(char *filename)
{
  ofstream timelogfile;
  timelogfile.open("timelog.txt", ios::app);
  auto rt = run_for_testcase(filename);
  timelogfile << "decompose merge total" << endl;
  timelogfile << rt.decompose << " " << rt.merge << " " << rt.total << endl;
  timelogfile.close();
}
int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    cout << "Please provide the input directory name as an argument\n";
    return 1;
  }
  char *directory = argv[1];
  DIR *dir;
  if ((dir = opendir(directory)) != NULL)
  {
    cout << "Running for directory " << directory << endl;
    run_and_time_for_directory(directory);
  }
  else
  {
    cout << "Running for file " << directory << endl;
    run_and_time_one_file(directory);
  }
  cout << "Finished. See decomposition.txt for decomposition and output.txt for merged polygons.\n";
  return 0;
}
/**
 * @brief Checks if vertices are colinear
 *
 * @param polygon
 * @return true If vertices are colinear
 */
bool colinear(vector<Vertex *> polygon)
{
  int x_neq = 0, y_neq = 0;

  for (int i = 1; i < polygon.size(); i++)
  {
    if (polygon[i]->x != polygon[i - 1]->x)
      x_neq = 1;

    if (polygon[i]->y != polygon[i - 1]->y)
      y_neq = 1;

    if (x_neq && y_neq)
      break;
  }

  if (!(x_neq && y_neq))
    return true;

  float curr_m, prev_m;

  for (int i = 1; i < polygon.size(); i++)
  {
    if (polygon[i]->x == polygon[i - 1]->x || polygon[i]->y == polygon[i - 1]->y)
      return false;

    curr_m = (polygon[i]->y - polygon[i - 1]->y) / (polygon[i]->x - polygon[i - 1]->x);

    if (i > 1)
      if (curr_m != prev_m)
        return false;

    prev_m = curr_m;
  }

  return true;
}
/**
 * @brief Get next vertex in order from the vector
 *
 * @param vertices Vertices in clockwise order
 * @param v
 * @return Vertex* Next in order to v
 */
Vertex *next_v(vector<Vertex *> vertices, Vertex *v)
{
  bool found = false;
  int i = 0;
  for (; i < vertices.size(); i++)
  {
    if (vertices[i] == v)
    {
      found = true;
      break;
    }
  }
  if (!found)
  {
    return NULL;
  }
  return vertices[(i + 1) % vertices.size()];
}
/**
 * @brief Subtract two vectors. Preserves the order of the first vector.
 *
 * Finds the elements of the first vector that are not in the second vector.
 *
 * @param a
 * @param b
 * @return vector<Vertex *>
 */
vector<Vertex *> subtract(vector<Vertex *> a, vector<Vertex *> b)
{
  vector<Vertex *> result;
  for (auto v : a)
  {
    // if (find(b.begin(), b.end(), v) == b.end())
    // {
    //   result.push_back(v);
    // }
    bool found = false;
    for (auto w : b)
    {
      if (v->x == w->x && v->y == w->y)
      {
        found = true;
        break;
      }
    }
    if (!found)
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
  /**
   * @brief Check if a vertex is inside the rectangle
   *
   * @param v Vertex to be checked for.
   * @return true If the vertex is on or inside the rectangle
   * @return false Otherwise
   */
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

/**
 * @brief Chops off a convex polygon from the given one
 *
 * Uses a clockwise sweep to find a convex polygon
 * If no convex polygon starting from the first vertex in
 * `vertices` is found, this returns two points (the first
 *  and second)
 *
 *
 * @param vertices
 * @return vector<Vertex *>
 */
vector<Vertex *> decompose(vector<Vertex *> vertices)
{
  DCEL dcel_p, dcel_l;
  dcel_p.initialize(vertices);
  vector<Vertex *> L = {vertices[0]};
  // cout << "init" << dcel_p.vertices.size() << endl;

  while (dcel_p.vertices.size() > 3)
  {
    // cout << "dcel_p.vertices.size() = " << dcel_p.vertices.size() << endl;
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
    if (L.size() == 2)
      return dcel_p.vertices;
    if (L.size() != dcel_p.vertices.size())
    {
      /// P = P - L
      auto LPVS = subtract(dcel_p.vertices, L);
      auto no_notches = subtract(LPVS, notches_in_p);
      LPVS = subtract(LPVS, no_notches);
      // print(LPVS);
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
            // cout << "v is ";
            // v->print();
            // cout << endl;
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
        diagonals.push_back({L.back(), L.front()});
        // std::cout
        // << "Found convex polygon" << std::endl;
        // print(L);
      }
      // insert L.back() as first of dcel_p.vertices
      // dcel_p.vertices.insert(dcel_p.vertices.begin(), L.back());
      // dcel_p.vertices.insert(dcel_p.vertices.begin(), L.front());
      // cout << "----New P----\n";
      // print(dcel_p.vertices);
      dcel_p.init();
    }
    else
    {
      // L has 2 vertices
      // cout << "L failed to form a convex polygon" << endl;
      return dcel_p.vertices;
    }
  }
  return dcel_p.vertices;
}
/**
 * @brief Reads vertices from a file
 *
 * The first line of the file should contain the number of vertices
 * Following lines should contain the x and y coordinates of the vertices,
 * separated by a space.
 *
 * @param filename Filename to read from
 * @return vector<Vertex *>
 */
vector<Vertex *> read_vertices(char *filename)
{
  vector<Vertex *> vertices;
  ifstream file;
  file.open(filename);
  // int n = number_of_lines(filename);
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
  // reverse and return
  // reverse(vertices.begin(), vertices.end());
  return vertices;
}