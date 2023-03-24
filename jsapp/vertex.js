export class Vertex {
  constructor(x, y) {
    this.x = x;
    this.y = y;
  }
  equals(other) {
    return this.x === other.x && this.y === other.y;
  }
}
class HalfEdge {
  constructor() {
    /** @type Vertex */
    this.origin = null;
    /** @type HalfEdge */
    this.twin = null;
    /** @type HalfEdge */
    this.next = null;
  }
  destination() {
    return this.twin.origin;
  }
  lies_on_left(v) {
    return ang_leq_180(this.destination(), this.origin, v)
  }
  equals(other) {
    return this.origin == other.origin && this.destination() === other.destination()
  }
}
export class DCEL {
  constructor() {
    /** @type Array<Vertex> */
    this.vertices = [];
    /** @type Array<HalfEdge> */
    this.half_edges = []
  }
  is_notch(v) {
    let a = v.incident_edge.destination();
    let b = v;
    let c = v.incident_edge.twin.next.destination();
    return ang_leq_180(a, b, c)
  }
  get_notches() {
    return this.vertices.filter(v => this.is_notch(v));
  }
  is_inside(v) {
    let e = this.half_edges[0];
    let on_left = e.lies_on_left(v);
    do {
      if (e.lies_on_left(v) !== on_left) {
        return false;
      }
      e = e.next;
    } while (e !== this.half_edges[0]);
    return true;
  }
  next_v(v) {
    let i = 0;
    for (; i < this.vertices.length; i++) {
      if (this.vertices[i].equals(v)) {
        break;
      }
    }
    return this.vertices[(i + 1) % this.vertices.length];
  }
  prev_v(v) {
    let i = 0;
    for (; i < this.vertices.length; i++) {
      if (this.vertices[i].equals(v)) {
        break;
      }
    }
    return this.vertices[(i - 1 + this.vertices.length) % this.vertices.length];
  }
  initialize(verts) {
    this.vertices = verts;
    for (let i = 0; i < verts.length; i++) {
      let e1 = new HalfEdge();
      let e2 = new HalfEdge();
      e1.twin = e2;
      e2.twin = e1;
      e1.origin = verts[i];
      e2.origin = verts[(i + 1) % verts.length];

      verts[i].incident_edge = e1;
      this.half_edges.push(e1, e2)
    }
    for (let i = 0; i < this.half_edges.length; i += 2) {
      let e1 = this.half_edges[i]
      let e2 = this.half_edges[i + 1]
      e1.next = this.half_edges[(i + 2) % this.half_edges.length]
      this.half_edges[(i + 3) % this.half_edges.length].next = e2
    }
  }
}

export class Decomposition {
  constructor() {
    this.vertices = [];
    this.dcel = null;
    /** @type HalfEdge*/
    this.diagonal = null;
  }
}

export function cross_product(a, b, c) {
  return (a.x - b.x) * (c.y - b.y) - (a.y - b.y) * (c.x - b.x);
}

export function ang_leq_180(a, b, c) {
  return cross_product(a, b, c) >= 0;
}
export function v_is_notch(a, b, c) {
  return ang_leq_180(a, b, c)
}