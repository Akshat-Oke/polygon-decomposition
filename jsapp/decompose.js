import { ang_leq_180, DCEL, Vertex } from './vertex.js';


/**
 * Returns the decomposition of the polygon
 * given as `vertices`
 * @param {Vertex[]} vertices 
 */
export function decompose_polygon(vertices) {
  let decomposition = [];
  let lastPolygonWasConstructed = false;
  let rv = decompose(vertices);
  let new_p_vertices = rv.remaining_p_vertices;
  decomposition.push(rv.decomposition)
  decomposition.push(decompose.decomposition);
  while (new_p_vertices.length > 3) {
    let rv = decompose(new_p_vertices);
    new_p_vertices = rv.remaining_p_vertices;
    decomposition.push(rv.decomposition)
  }
  if (new_p_vertices.length == 3) {
    lastPolygonWasConstructed = true;
    decomposition.push(new_p_vertices);
  }
  return {
    decomposition,
    lastPolygonWasConstructed
  }
}

/**
 * 
 * @param {Vertex[]} vertices 
 * @param {Vertex} v 
 * @returns 
 */
function next_v(vertices, v) {
  let found = false; let i = 0;
  for (; i < vertices.length; i++) {
    if (vertices[i].equals(v)) {
      found = true;
      break
    }
  }
  if (!found) {
    return null;
  }
  return vertices[(i + 1) % vertices.length]
}
/**
 * 
 * @param {Vertex[]} vertices 
 */
export function decompose(vertices) {
  let returnObj = {
    /** @type {Vertex[][]} */
    decomposition: [],
    /** @type {Vertex[][]} */
    diagonals: [],
    /** @type {Vertex[]} */
    remaining_p_vertices: [],
  };
  let dcel_p = new DCEL();
  // let dcel_l = new DCEL();
  dcel_p.initialize(vertices)
  returnObj.remaining_p_vertices = dcel_p.vertices;
  let L = [vertices[0]]
  while (dcel_p.vertices.length > 3) {
    let notches_in_p = dcel_p.get_notches()
    /** @type Vertex[] */
    let v = [];
    v.push(L.at(-1));
    let next_vertex = next_v(dcel_p.vertices, v[0])
    v.push(next_vertex);
    L = [v[0], v[1]];
    let i = 1;
    v.push(next_v(dcel_p.vertices, v[i]));
    while (
      ang_leq_180(v[i - 1], v[i], v[i + 1]) &&
      ang_leq_180(v[i], v[i + 1], v[0]) &&
      ang_leq_180(v[i + 1], v[0], v[1]) &&
      L.length < dcel_p.vertices.length
    ) {
      L.push(v[i + 1]);
      i++;
      v.push(next_v(dcel_p.vertices, v[i]));
    }
    if (L.length = 2) {
      return returnObj;
    }
    if (L.length != dcel_p.vertices.length) {
      let LPVS = subtract(dcel_p.vertices, L);
      let no_notches = subtract(LPVS, notches_in_p);
      LPVS = subtract(LPVS, no_notches);
      while (LPVS.size() > 0) {
        let min_x = LPVS.reduce((min, p) => p.x < min ? p.x : min, LPVS[0].x);
        let min_y = LPVS.reduce((min, p) => p.y < min ? p.y : min, LPVS[0].y);
        let max_x = LPVS.reduce((max, p) => p.x > max ? p.x : max, LPVS[0].x);
        let max_y = LPVS.reduce((max, p) => p.y > max ? p.y : max, LPVS[0].y);
        let r = new Rectangle(min_x, max_x, min_y, max_y);
        let backward = false;
        while (!backward && LPVS.length > 0) {
          while (true) {
            if (LPVS.length == 0) break;
            let v = LPVS[0];
            if (!r.contains(v)) {
              let index = LPVS.findIndex((p) => p.equals(v));
              LPVS.splice(index, 1);
            } else break;
          }
          if (LPVS.length > 0) {
            let dcel_l = new DCEL();
            dcel_l.initialize(L);
            if (dcel_l.is_inside(LPVS[0])) {
              let VTR = [];
              let side = ang_leq_180(LPVS[0], v[0], L.at(-1));
              for (const l_v of dcel_l.vertices) {
                if (ang_leq_180(LPVS[0], v[0], l_v) == side) {
                  VTR.push(l_v);
                }
              }
              for (const vtr_v of VTR) {
                let index = L.findIndex((p) => p.equals(vtr_v));
                L.splice(index, 1);
              }
              backward = true;
            }
            LPVS.splice(0, 1);
          }
        }
      } //while lpvs > 0
    }
    if (!L.at(-1).equals(v[1])) {
      // if L has atleast 3 vertices
      let p_new = subtract(dcel_p.vertices, L);
      let first_last_l = [L[0], L.at(-1)];
      let p_nn = subtract(dcel_p.vertices, p_new);
      p_nn = subtract(p_nn, first_last_l);

      dcel_p.vertices = subtract(dcel_p.vertices, p_nn);
      ///found decomposition
      returnObj.decomposition.push(L);
      returnObj.diagonals.push([L.at(-1), L[0]]);
      dcel_p.init()
    } else {
      //L has two vertices
      return returnObj;
    }
  }
  return returnObj;
}
/**
 * Subtracts two arrays of vertices
 * @param {Vertex[]} a 
 * @param {Vertex[]} b 
 */
function subtract(a, b) {
  let result = [];
  for (const v of a) {
    let found = false;
    for (const w of b) {
      if (v.equals(w)) {
        found = true;
        break;
      }
    }
    if (!found) {
      result.push(v);
    }
  }
  return result;
}

class Rectangle {
  constructor(x1, x2, y1, y2) {
    this.x1 = x1;
    this.x2 = x2;
    this.y1 = y1;
    this.y2 = y2;
  }
  /**
   * 
   * @param {Vertex} v Vertex to check
   * @returns 
   */
  contains(v) {
    return v.x >= this.x1 && v.x <= this.x2 && v.y >= this.y1 && v.y <= this.y2;
  }
}