import { ang_leq_180, DCEL, Vertex } from './vertex.js';
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
function decompose(vertices) {
  let dcel_p = new DCEL();
  let dcel_l = new DCEL();
  dcel_p.initialize(vertices)
  let L = [vertices[0]]
  while (dcel_p.vertices.length > 3) {
    let notches_in_p = dcel_p.get_notches()
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
      return dcel_p.vertices;
    }
    if (L.length != dcel_p.vertices.length) {
      // let LPVS = 
    }
  }
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