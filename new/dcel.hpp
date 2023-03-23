#ifndef _DCEL_H_
#define _DCEL_H_
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct HalfEdge;
struct Vertex;
double cross_product(Vertex *a, Vertex *b, Vertex *c);
bool ang_leq_180(Vertex *a, Vertex *b, Vertex *c);
bool v_is_notch(Vertex *a, Vertex *b, Vertex *c);
Vertex *next_v(vector<Vertex *> vertices, Vertex *v);

/**
 * @brief Vertex of the polygon.
 */
struct Vertex
{
  double x, y;
  /*! The half edge that has this vertex as the destination*/
  HalfEdge *incident_edge;
  Vertex(double x, double y)
  {
    this->x = x;
    this->y = y;
  }
  Vertex() {}

  bool operator==(const Vertex &other) const
  {
    return x == other.x && y == other.y;
  }
  void print()
  {
    cout << "(" << x << ", " << y << ") ";
  }
  std::ostream &operator<<(std::ostream &os)
  {
    return os << "(" << x << ", " << y << ")";
  }
};
struct Face
{
  HalfEdge *incident_edge;
};
/**
 * @brief A directed edge.
 * 
 * Around a 
 * 
 */
struct HalfEdge
{
  Vertex *origin;
  HalfEdge *twin;
  HalfEdge *next;
  Face *incident_face;
  Vertex *destination() const { return twin->origin; }
  // tested
  bool lies_on_left(Vertex *v)
  {
    return ang_leq_180(destination(), origin, v);
  }
  void print()
  {
    origin->print();
    cout << " -> ";
    destination()->print();
  }
  bool equals(const HalfEdge *other) const
  {
    return origin == other->origin && destination() == other->destination();
  }
};

class DCEL
{
public:
  vector<Vertex *> vertices;
  vector<HalfEdge *> half_edges;
  vector<Face *> faces;

  void add_vertex(Vertex *v)
  {
    vertices.push_back(v);
  }

  void remove_vertex(Vertex *v)
  {
    vertices.erase(remove(vertices.begin(), vertices.end(), v), vertices.end());
  }
  // tested
  bool is_notch(Vertex *v)
  {
    Vertex *a = v->incident_edge->destination();
    auto b = v;
    auto c = v->incident_edge->twin->next->destination();
    // cout << "a: ";
    // a->print();
    // b->print();
    // c->print();
    // cout << endl;
    return ang_leq_180(a, b, c);
  }

  void printFromVertex(Vertex *v)
  {
    cout << "Vertices from v are\n";
    auto e = v->incident_edge;
    do
    {
      cout << "(" << e->origin->x << ", " << e->origin->y << ")" << endl;
      e = e->next;
    } while (e != v->incident_edge);
  }

  vector<Vertex *> get_notches()
  {
    vector<Vertex *> notches;
    for (auto v : vertices)
    {
      if (is_notch(v))
      {
        notches.push_back(v);
      }
    }
    return notches;
  }

  void add_half_edge(HalfEdge *e)
  {
    half_edges.push_back(e);
  }

  void add_face(Face *f)
  {
    faces.push_back(f);
  }

  void init()
  {
    initialize(vertices);
  }

  bool is_inside(Vertex *v)
  {
    auto e = this->half_edges[0];
    bool on_left = e->lies_on_left(v);
    // cout << on_left << endl;
    do
    {
      if (e->lies_on_left(v) != on_left)
      {
        return false;
      }
      e = e->next;
    } while (e != this->half_edges[0]);
    return true;
  }

  Vertex *next_v(Vertex *v)
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
    // return v->incident_edge->next->destination();
  }
  Vertex *prev_v(Vertex *v)
  {
    int i = 0;
    for (; i < vertices.size(); i++)
    {
      if (vertices[i] == v)
      {
        break;
      }
    }
    return vertices[(i - 1 + vertices.size()) % vertices.size()];
  }
  void print()
  {
    for (auto v : vertices)
    {
      v->print();
      cout << endl;
    }
  }

  void initialize(vector<Vertex *> verts)
  {
    this->vertices = verts;
    // Create a half-edge for each pair of vertices
    for (int i = 0; i < verts.size(); ++i)
    {
      HalfEdge *e1 = new HalfEdge;
      HalfEdge *e2 = new HalfEdge;

      e1->twin = e2;
      e2->twin = e1;

      e1->origin = verts[i];
      e2->origin = verts[(i + 1) % verts.size()];

      verts[i]->incident_edge = e1;

      add_half_edge(e1);
      add_half_edge(e2);
    }

    // Connect the half-edges to form a cycle
    for (int i = 0; i < half_edges.size(); i += 2)
    {
      HalfEdge *e1 = half_edges[i];
      HalfEdge *e2 = half_edges[i + 1];

      e1->next = half_edges[(i + 2) % half_edges.size()];
      // e2->next = e1s;
      half_edges[(i + 3) % half_edges.size()]->next = e2;
    }
    // cout << "half_edges.size()" << half_edges.size() << endl;
    // Create a face for the cycle of half-edges
    Face *f = new Face;
    HalfEdge *e = half_edges[1];

    do
    {
      e->incident_face = f;
      e = e->next;
    } while (e != half_edges[1]);

    f->incident_edge = e;

    add_face(f);
  }
};

struct Decomposition
{
  vector<Vertex *> vertices;
  DCEL dcel;
  HalfEdge *diagonal;
};

#endif