/**
 * @file dcel.cpp
 * @brief Helper functions for DCEL
 * @version 0.1
 * @date 2023-03-19
 *
 *
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include "dcel.hpp"
using namespace std;

// struct HalfEdge;
// struct Vertex
// {
//   double x, y;
//   HalfEdge *incident_edge;
//   Vertex(double x, double y)
//   {
//     this->x = x;
//     this->y = y;
//   }
//   Vertex() {}

//   bool operator==(const Vertex &other) const
//   {
//     return x == other.x && y == other.y;
//   }
//   void print()
//   {
//     cout << "(" << x << ", " << y << ") ";
//   }
//   std::ostream &operator<<(std::ostream &os)
//   {
//     return os << "(" << x << ", " << y << ")";
//   }
// };

// struct Face;
/**
 * @brief ba x ca
 *
 * Computes cross product ba*ca.
 *       . c
 *      /     This will be positive.
 *     /
 * b ./________.a
 *
 * @param a
 * @param b
 * @param c
 * @return double k component of the product.
 */
double cross_product(Vertex *a, Vertex *b, Vertex *c)
{
  return (a->x - b->x) * (c->y - b->y) - (a->y - b->y) * (c->x - b->x);
}
/**
 * @brief Checks if angle abc is less than or equal to 180 degrees.
 *
 * The angle swept by a
counterclockwise rotation from line segment ba to
line segment bc
 *       . c
 *      /     This will be true.
 *     /
 * b ./________.a
 *
 * @param a
 * @param b
 * @param c
 * @return true if angle is less than or equal to 180 degrees.
 * @return false otherwise
 */
bool ang_leq_180(Vertex *a, Vertex *b, Vertex *c)
{
  return cross_product(a, b, c) >= 0;
}

/**
 * @brief Checks if vertex v is a notch.
 *
 * Notch is when the angle abc is a reflex angle.
 *       . a
 *      /     This will be true.
 *     /
 * b ./________.c
 * @param v
 * @return true if v is a notch.
 * @return false otherwise.
 */
bool v_is_notch(Vertex *a, Vertex *b, Vertex *c)
{
  // return cross_product(a, b, c) < 0;
  return ang_leq_180(a, b, c);
}