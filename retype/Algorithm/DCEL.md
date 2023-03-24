---
order: 99
---

# DCEL

DCEL (Doubly-Connected Edge List) is a data structure used in computational geometry for representing planar graphs. It is mainly used for solving various geometric problems such as:

- computing the intersection of two polygons,
- finding the convex hull of a set of points, and
- computing the Voronoi diagram of a set of points.

The DCEL data structure represents the planar graph using three main components: vertices, edges, and faces.

**Vertices**: A vertex is a point in space with a unique identifier. In the DCEL data structure, a vertex is represented by a vertex object that stores its coordinates and a pointer to one of the half-edges that emanates from it. This pointer is used to navigate around the graph from one vertex to another. Each vertex is incident to one or more half-edges, which are either incoming or outgoing from the vertex.

**Edges**: An edge is a line segment connecting two vertices. In the DCEL data structure, an edge is represented by a pair of half-edges that point in opposite directions. Each half-edge stores a pointer to its origin vertex, a pointer to its twin half-edge, a pointer to the next half-edge in the counterclockwise direction around its incident face, and a pointer to the previous half-edge in the counterclockwise direction around its origin vertex. The twin half-edge points to the other side of the edge, and the next and previous half-edges are used to navigate around the incident face.

**Faces**: A face is a region bounded by a set of edges. In the DCEL data structure, a face is represented by a face object that stores a pointer to one of its incident half-edges. This pointer is used to navigate around the face by following the next half-edge in the counterclockwise direction.

The DCEL data structure also includes pointers that link the vertices, edges, and faces together. Each vertex stores a pointer to one of its incident half-edges, and each half-edge stores pointers to its origin vertex, its twin half-edge, the next and previous half-edges around its origin vertex, and the incident face. The face object stores a pointer to one of its incident half-edges, which points to the next half-edge around the face.

The DCEL data structure allows efficient operations such as adding or deleting vertices, edges, or faces, and navigating around the planar graph. It also provides constant-time access to the neighboring edges of a given edge, the incident faces of a given vertex, and the edges incident to a given face.
