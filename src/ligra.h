// This code is part of the project "Ligra: A Lightweight Graph Processing
// Framework for Shared Memory", presented at Principles and Practice of
// Parallel Programming, 2013.
// Copyright (c) 2013 Julian Shun and Guy Blelloch
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights (to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#pragma once

#include <limits.h>
#include <stdlib.h>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "IO.h"
#include "compressed_vertex.h"
#include "edge_map_utils.h"
#include "graph.h"
#include "parse_command_line.h"
#include "vertex.h"
#include "vertex_subset.h"

#include "oldlib/utils.h"

#include "../lib/binary_search.h"
#include "../lib/get_time.h"
#include "../lib/index_map.h"

typedef uint32_t flags;
const flags no_output = 1;
const flags pack_edges = 2;
const flags sparse_blocked = 4;
const flags dense_forward = 8;
const flags dense_parallel = 16;
const flags remove_duplicates = 32;
const flags no_dense = 64;
const flags in_edges = 128;  // map over in edges instead of out edges
inline bool should_output(const flags& fl) { return !(fl & no_output); }

template <class W, class F>
struct Wrap_F {
  F f;
  Wrap_F(F _f) : f(_f) {}
  inline bool update(const uintE& s, const uintE& d, const W& e) {
    return f.update(s, d);
  }
  inline bool updateAtomic(const uintE& s, const uintE& d, const W& e) {
    return f.updateAtomic(s, d);
  }
  inline bool cond(const uintE& d) { return f.cond(d); }
};

template <class W, class D, class F>
struct Wrap_Default_F {
  F f;
  D def;
  Wrap_Default_F(F _f, D _def) : f(_f), def(_def) {}
  inline auto update(const uintE& s, const uintE& d, const W& e)
      -> decltype(f.update) {
    return f.update(s, d, def);
  }
  inline auto updateAtomic(const uintE& s, const uintE& d, const W& e)
      -> decltype(f.updateAtomic) {
    return f.updateAtomic(s, d, def);
  }
  inline bool cond(const uintE& d) { return f.cond(d); }
};

template <class W, class F>
inline auto wrap_em_f(F f) -> Wrap_F<W, F> {
  return Wrap_F<W, F>(f);
}

template <class W, class D, class F,
          typename std::enable_if<!std::is_same<W, D>::value, int>::type = 0>
inline auto wrap_with_default(F f, D def) -> Wrap_Default_F<W, D, F> {
  return Wrap_Default_F<W, D, F>(f, def);
}

template <class W, class D, class F,
          typename std::enable_if<std::is_same<W, D>::value, int>::type = 0>
inline auto wrap_with_default(F f, D def) -> decltype(f) {
  return f;
}

template <class data, class vertex, class VS, class F>
inline vertexSubsetData<data> edgeMapDense(graph<vertex> GA, VS& vertexSubset,
                                           F& f, const flags fl) {
  std::cout << "dense"
            << "\n";
  using D = std::tuple<bool, data>;
  long n = GA.n;
  vertex* G = GA.V;
  if (should_output(fl)) {
    D* next = newA(D, n);
    auto g = get_emdense_gen<data>(next);
    parallel_for_bc(v, 0, n, true, {
      std::get<0>(next[v]) = 0;
      if (f.cond(v)) {
        (fl & in_edges) ? G[v].decodeOutNghBreakEarly(v, vertexSubset, f, g,
                                                      fl & dense_parallel)
                        : G[v].decodeInNghBreakEarly(v, vertexSubset, f, g,
                                                     fl & dense_parallel);
      }
    });
    return vertexSubsetData<data>(n, next);
  } else {
    auto g = get_emdense_nooutput_gen<data>();
    parallel_for_bc(v, 0, n, true, {
      if (f.cond(v)) {
        (fl & in_edges) ? G[v].decodeOutNghBreakEarly(v, vertexSubset, f, g,
                                                      fl & dense_parallel)
                        : G[v].decodeInNghBreakEarly(v, vertexSubset, f, g,
                                                     fl & dense_parallel);
      }
    });
    return vertexSubsetData<data>(n);
  }
}

template <class data, class vertex, class VS, class F>
inline vertexSubsetData<data> edgeMapDenseForward(graph<vertex> GA,
                                                  VS& vertexSubset, F& f,
                                                  const flags fl) {
  std::cout << "Dense Forward"
            << "\n";
  using D = std::tuple<bool, data>;
  long n = GA.n;
  vertex* G = GA.V;
  if (should_output(fl)) {
    D* next = newA(D, n);
    auto g = get_emdense_forward_gen<data>(next);
    parallel_for_bc(i, 0, n, (n > pbbs::kSequentialForThreshold),
                    { std::get<0>(next[i]) = 0; });
    parallel_for_bc(i, 0, n, true, {
      if (vertexSubset.isIn(i)) {
        (fl & in_edges) ? G[i].decodeInNgh(i, f, g)
                        : G[i].decodeOutNgh(i, f, g);
      }
    });
    return vertexSubsetData<data>(n, next);
  } else {
    auto g = get_emdense_forward_nooutput_gen<data>();
    parallel_for_bc(i, 0, n, true, {
      if (vertexSubset.isIn(i)) {
        (fl & in_edges) ? G[i].decodeInNgh(i, f, g)
                        : G[i].decodeOutNgh(i, f, g);
      }
    });
    return vertexSubsetData<data>(n);
  }
}

template <class data, class vertex, class VS, class F>
inline vertexSubsetData<data> edgeMapSparse(graph<vertex>& GA,
                                            vertex* frontier_vertices,
                                            VS& indices, uintT m, F& f,
                                            const flags fl) {
  using S = std::tuple<uintE, data>;
  long n = indices.n;
  S* outEdges;

  if (should_output(fl)) {
    auto offsets = array_imap<uintT>(indices.size(), [&](size_t i) {
      return (fl & in_edges) ? frontier_vertices[i].getInDegree()
                             : frontier_vertices[i].getOutDegree();
    });
    size_t outEdgeCount = pbbs::scan_add(offsets, offsets);
    outEdges = newA(S, outEdgeCount);
    auto g = get_emsparse_gen_full<data>(outEdges);
    auto h = get_emsparse_gen_empty<data>(outEdges);
    parallel_for_bc(i, 0, m, true, {
      uintT v = indices.vtx(i);
      uintT o = offsets[i];
      vertex vert = frontier_vertices[i];
      (fl & in_edges) ? vert.decodeInNghSparse(v, o, f, g, h)
                      : vert.decodeOutNghSparse(v, o, f, g, h);
    });
    offsets.del();

    S* nextIndices = newA(S, outEdgeCount);
    if (fl & remove_duplicates) {
      if (GA.flags == NULL) {
        GA.flags = newA(uintE, n);
        parallel_for_bc(i, 0, n, (n > pbbs::kSequentialForThreshold),
                        { GA.flags[i] = UINT_E_MAX; });
      }
      auto get_key = [&](size_t i) -> uintE& {
        return std::get<0>(outEdges[i]);
      };
      ligra_utils::remDuplicates(get_key, GA.flags, outEdgeCount, n);
    }
    auto p = [](std::tuple<uintE, data>& v) {
      return std::get<0>(v) != UINT_E_MAX;
    };
    size_t nextM = pbbs::filterf(outEdges, nextIndices, outEdgeCount, p);
    free(outEdges);
    return vertexSubsetData<data>(n, nextM, nextIndices);
  }

  auto g = get_emsparse_nooutput_gen<data>();
  auto h = get_emsparse_nooutput_gen_empty<data>();
  parallel_for_bc(i, 0, m, true, {
    uintT v = indices.vtx(i);
    vertex vert = frontier_vertices[i];
    (fl & in_edges) ? vert.decodeInNghSparse(v, 0, f, g, h)
                    : vert.decodeOutNghSparse(v, 0, f, g, h);
  });
  return vertexSubsetData<data>(n);
}

template <class data, class vertex, class VS, class F>
inline vertexSubsetData<data> edgeMapSparseNoOutput(graph<vertex>& GA,
                                                    vertex* frontier_vertices,
                                                    VS& indices, uintT m, F& f,
                                                    const flags fl) {
  auto n = GA.n;
  auto g = get_emsparse_nooutput_gen<data>();
  auto h = get_emsparse_nooutput_gen_empty<data>();
  parallel_for_bc(i, 0, m, (m > 1), {
    uintT v = indices.vtx(i);
    vertex vert = frontier_vertices[i];
    (fl & in_edges) ? vert.decodeInNghSparse(v, 0, f, g, h)
                    : vert.decodeOutNghSparse(v, 0, f, g, h);
  });
  return vertexSubsetData<data>(n);
}

struct block {
  uintE id;
  uintE block_num;
  block(uintE _id, uintE _b) : id(_id), block_num(_b) {}
  block() {}
  void print() { std::cout << id << " " << block_num << "\n"; }
};

#ifdef AMORTIZEDPD
template <class data, class vertex, class VS, class F>
inline vertexSubsetData<data> edgeMapBlocked(graph<vertex>& GA,
                                             vertex* frontier_vertices,
                                             VS& indices, uintT m, F& f,
                                             const flags fl) {
  if (fl & no_output) {
    return edgeMapSparseNoOutput<data, vertex, VS, F>(GA, frontier_vertices,
                                                      indices, m, f, fl);
  }
  using S = std::tuple<uintE, data>;
  size_t n = indices.n;
  auto degree_imap = make_in_imap<uintE>(indices.size(), [&](size_t i) {
    return (fl & in_edges) ? frontier_vertices[i].getInVirtualDegree()
                           : frontier_vertices[i].getOutVirtualDegree();
  });

  // 1. Compute the number of blocks each vertex gets subdivided into.
  auto vertex_offs = array_imap<uintE>(indices.size() + 1);
  parallel_for_bc(
      i, 0, indices.size(), (indices.size() > pbbs::kSequentialForThreshold),
      { vertex_offs[i] = (degree_imap[i] + kEMBlockSize - 1) / kEMBlockSize; });
  vertex_offs[indices.size()] = 0;
  size_t num_blocks = pbbs::scan_add(vertex_offs, vertex_offs);
  auto blocks = array_imap<block>(num_blocks);
  auto degrees = array_imap<uintT>(num_blocks);

  // 2. Write each block to blocks and scan.
  parallel_for_bc(i, 0, indices.size(), true, {
    size_t vtx_off = vertex_offs[i];
    size_t num_blocks = vertex_offs[i + 1] - vtx_off;
    size_t degree = degree_imap[i];
    parallel_for_bc(j, 0, num_blocks, (num_blocks > 1000), {
      size_t block_deg =
          std::min((j + 1) * kEMBlockSize, degree) - j * kEMBlockSize;
      blocks[vtx_off + j] = block(i, j);
      degrees[vtx_off + j] = block_deg;
    });
  });
  pbbs::scan_add(degrees, degrees, pbbs::fl_scan_inclusive);
  size_t outEdgeCount = degrees[num_blocks - 1];

  // 3. Compute the number of threads, binary search for offsets.
  size_t n_threads = nblocks(outEdgeCount, kEMBlockSize);
  size_t* thread_offs = newA(size_t, n_threads + 1);
  auto lt = [](const uintT& l, const uintT& r) { return l < r; };
  parallel_for_bc(i, 0, n_threads, (n_threads > 1), {
    size_t start_off = i * kEMBlockSize;
    thread_offs[i] = pbbs::binary_search(degrees, start_off, lt);
  });
  thread_offs[n_threads] = num_blocks;

  // 4. Run each thread in parallel
  auto cts = array_imap<uintE>(n_threads + 1);
  S* outEdges = newA(S, outEdgeCount);
  auto g = get_emsparse_blocked_gen<data>(outEdges);
  parallel_for_bc(i, 0, n_threads, (n_threads > 1), {
    size_t start = thread_offs[i];
    size_t end = thread_offs[i + 1];
    // <= kEMBlockSize edges in this range, sequentially process
    if (start != end && start != num_blocks) {
      size_t start_offset = (start == 0) ? 0 : degrees[start - 1];
      size_t k = start_offset;
      for (size_t j = start; j < end; j++) {
        auto& block = blocks[j];
        uintT block_degree = degrees[j];
        uintE id = block.id;
        uintE v = indices.vtx(id);
        uintE b_size =
            (j == 0) ? block_degree : (block_degree - degrees[j - 1]);
        uintE block_num = block.block_num;
        size_t num_in = (fl & in_edges)
                            ? frontier_vertices[id].decodeInNghSparseBlock(
                                  v, k, b_size, block_num, f, g)
                            : frontier_vertices[id].decodeOutNghSparseBlock(
                                  v, k, b_size, block_num, f, g);
        k += num_in;
      }
      cts[i] = k - start_offset;
    } else {
      cts[i] = 0;
    }
  });
  cts[n_threads] = 0;
  long out_size = pbbs::scan_add(cts, cts);

  // 5. Use cts to get
  S* out = newA(S, out_size);
  parallel_for_bc(i, 0, n_threads, (n_threads > 1), {
    size_t start = thread_offs[i];
    size_t end = thread_offs[i + 1];
    if (start != end) {
      size_t start_offset = (start == 0) ? 0 : degrees[start - 1];
      size_t out_offset = cts[i];
      size_t num_live = cts[i + 1] - out_offset;
      for (size_t j = 0; j < num_live; j++) {
        out[out_offset + j] = outEdges[start_offset + j];
      }
    }
  });
  free(outEdges);
  free(thread_offs);
  cts.del();
  vertex_offs.del();
  blocks.del();
  degrees.del();

  if (fl & remove_duplicates) {
    if (GA.flags == NULL) {
      GA.flags = newA(uintE, n);
      parallel_for_bc(i, 0, n, (n > pbbs::kSequentialForThreshold),
                      { GA.flags[i] = UINT_E_MAX; });
    }
    auto get_key = [&](size_t i) -> uintE& { return std::get<0>(out[i]); };
    ligra_utils::remDuplicates(get_key, GA.flags, out_size, n);
    S* nextIndices = newA(S, out_size);
    auto p = [](std::tuple<uintE, data>& v) {
      return std::get<0>(v) != UINT_E_MAX;
    };
    size_t nextM = pbbs::filterf(out, nextIndices, out_size, p);
    free(out);
    return vertexSubsetData<data>(n, nextM, nextIndices);
  }
  return vertexSubsetData<data>(n, out_size, out);
}
#else
template <class data, class vertex, class VS, class F>
inline vertexSubsetData<data> edgeMapBlocked(graph<vertex>& GA,
                                             vertex* frontier_vertices,
                                             VS& indices, uintT m, F& f,
                                             const flags fl) {
  if (fl & no_output) {
    return edgeMapSparseNoOutput<data, vertex, VS, F>(GA, frontier_vertices,
                                                      indices, m, f, fl);
  }
  using S = std::tuple<uintE, data>;
  size_t n = indices.n;
  auto degree_imap = make_in_imap<uintE>(indices.size(), [&](size_t i) {
    return (fl & in_edges) ? frontier_vertices[i].getInVirtualDegree()
                           : frontier_vertices[i].getOutVirtualDegree();
  });

  // 1. Compute the number of blocks each vertex gets subdivided into.
  size_t num_blocks = indices.size();
  auto degrees = array_imap<uintT>(num_blocks);

  // 2. Write each block to blocks and scan.
  parallel_for_bc(i, 0, indices.size(), true, { degrees[i] = degree_imap[i]; });
  pbbs::scan_add(degrees, degrees, pbbs::fl_scan_inclusive);
  size_t outEdgeCount = degrees[num_blocks - 1];

  // 3. Compute the number of threads, binary search for offsets.
  size_t n_threads =
      nblocks(outEdgeCount, kEMBlockSize);  // TODO(laxmand): 4*nworkers()?
  size_t* thread_offs = newA(size_t, n_threads + 1);
  auto lt = [](const uintT& l, const uintT& r) { return l < r; };
  parallel_for_bc(i, 0, n_threads, (n_threads > 1), {
    size_t start_off = i * kEMBlockSize;
    thread_offs[i] = pbbs::binary_search(degrees, start_off, lt);
  });
  thread_offs[n_threads] = num_blocks;

  // 4. Run each thread in parallel
  auto cts = array_imap<uintE>(n_threads + 1);
  S* outEdges = newA(S, outEdgeCount);
  auto g = get_emsparse_blocked_gen<data>(outEdges);
  parallel_for_bc(i, 0, n_threads, (n_threads > 1), {
    size_t start = thread_offs[i];
    size_t end = thread_offs[i + 1];
    // <= kEMBlockSize edges in this range, sequentially process
    if (start != end && start != num_blocks) {
      size_t start_offset = (start == 0) ? 0 : degrees[start - 1];
      size_t k = start_offset;
      for (size_t j = start; j < end; j++) {
        uintE v = indices.vtx(j);
        size_t num_in =
            (fl & in_edges)
                ? frontier_vertices[j].decodeInNghSparseSeq(v, k, f, g)
                : frontier_vertices[j].decodeOutNghSparseSeq(v, k, f, g);
        k += num_in;
      }
      cts[i] = k - start_offset;
    } else {
      cts[i] = 0;
    }
  });
  cts[n_threads] = 0;
  long out_size = pbbs::scan_add(cts, cts);

  // 5. Use cts to get
  S* out = newA(S, out_size);
  parallel_for_bc(i, 0, n_threads, (n_threads > 1), {
    size_t start = thread_offs[i];
    size_t end = thread_offs[i + 1];
    if (start != end) {
      size_t start_offset = (start == 0) ? 0 : degrees[start - 1];
      size_t out_offset = cts[i];
      size_t num_live = cts[i + 1] - out_offset;
      for (size_t j = 0; j < num_live; j++) {
        out[out_offset + j] = outEdges[start_offset + j];
      }
    }
  });
  free(outEdges);
  free(thread_offs);
  cts.del();
  degrees.del();

  if (fl & remove_duplicates) {
    if (GA.flags == NULL) {
      GA.flags = newA(uintE, n);
      parallel_for_bc(i, 0, n, (n > pbbs::kSequentialForThreshold),
                      { GA.flags[i] = UINT_E_MAX; });
    }
    auto get_key = [&](size_t i) -> uintE& { return std::get<0>(out[i]); };
    ligra_utils::remDuplicates(get_key, GA.flags, out_size, n);
    S* nextIndices = newA(S, out_size);
    auto p = [](std::tuple<uintE, data>& v) {
      return std::get<0>(v) != UINT_E_MAX;
    };
    size_t nextM = pbbs::filterf(out, nextIndices, out_size, p);
    free(out);
    return vertexSubsetData<data>(n, nextM, nextIndices);
  }
  return vertexSubsetData<data>(n, out_size, out);
}
#endif

// Decides on sparse or dense base on number of nonzeros in the active vertices.
template <class data, class vertex, class VS, class F>
inline vertexSubsetData<data> edgeMapData(graph<vertex>& GA, VS& vs, F f,
                                          intT threshold = -1,
                                          const flags& fl = 0) {
  long numVertices = GA.n, numEdges = GA.m, m = vs.numNonzeros();
  if (threshold == -1) threshold = numEdges / 20;
  if (vs.size() == 0) return vertexSubsetData<data>(numVertices);

  if (vs.isDense && vs.size() > numVertices / 10) {
    return (fl & dense_forward)
               ? edgeMapDenseForward<data, vertex, VS, F>(GA, vs, f, fl)
               : edgeMapDense<data, vertex, VS, F>(GA, vs, f, fl);
  }

  vs.toSparse();
  vertex* frontier_vertices = newA(vertex, m);
  parallel_for_bc(i, 0, vs.size(), (vs.size() > pbbs::kSequentialForThreshold),
                  { frontier_vertices[i] = GA.V[vs.vtx(i)]; });
  auto degree_im = make_in_imap<size_t>(vs.size(), [&](size_t i) {
    return (fl & in_edges) ? frontier_vertices[i].getInDegree()
                           : frontier_vertices[i].getOutDegree();
  });
  size_t out_degrees = pbbs::reduce_add(degree_im);

  if (out_degrees == 0) return vertexSubsetData<data>(numVertices);
  if (m + out_degrees > threshold && !(fl & no_dense)) {
    vs.toDense();
    free(frontier_vertices);
    return (fl & dense_forward)
               ? edgeMapDenseForward<data, vertex, VS, F>(GA, vs, f, fl)
               : edgeMapDense<data, vertex, VS, F>(GA, vs, f, fl);
  } else {
    auto vs_out = edgeMapBlocked<data, vertex, VS, F>(GA, frontier_vertices, vs,
                                                      vs.numNonzeros(), f, fl);
    free(frontier_vertices);
    return vs_out;
  }
}

// Regular edgeMap, where no extra data is stored per vertex.
template <class vertex, class VS, class F>
inline vertexSubset edgeMap(graph<vertex> GA, VS& vs, F f, intT threshold = -1,
                            const flags& fl = 0) {
  return edgeMapData<pbbs::empty>(GA, vs, f, threshold, fl);
}

// Packs out the adjacency lists of all vertex in vs. A neighbor, ngh, is kept
// in the new adjacency list if p(ngh) is true.
template <template <class W> class wvertex, class W, class P>
inline vertexSubsetData<uintE> packEdges(graph<wvertex<W>>& GA,
                                         vertexSubset& vs, P& p,
                                         const flags& fl = 0) {
  using S = std::tuple<uintE, uintE>;
  using vertex = wvertex<W>;
  vs.toSparse();
  vertex* G = GA.V;
  size_t m = vs.numNonzeros();
  size_t n = vs.numRows();
  if (vs.size() == 0) {
    return vertexSubsetData<uintE>(n);
  }
  auto space = array_imap<uintT>(m);
  parallel_for_bc(i, 0, m, (m > pbbs::kSequentialForThreshold), {
    uintE v = vs.vtx(i);
    space[i] = G[v].calculateOutTemporarySpace();
  });
  long total_space = pbbs::scan_add(space, space);
  std::cout << "packNghs: total space allocated = " << total_space << "\n";
  auto tmp = array_imap<std::tuple<uintE, W>>(
      total_space);  // careful when total_space == 0
  S* outV;
  if (should_output(fl)) {
    outV = newA(S, vs.size());
    {
      auto for_inner = [&](size_t i) {
        uintE v = vs.vtx(i);
        std::tuple<uintE, W>* tmp_v = tmp.start() + space[i];
        size_t ct = G[v].packOutNgh(v, p, tmp_v);
        outV[i] = std::make_tuple(v, ct);
      };
      parallel_for_bc(i, 0, m, true, { for_inner(i); });
    }
    return vertexSubsetData<uintE>(n, m, outV);
  } else {
    {
      auto for_inner = [&](size_t i) {
        uintE v = vs.vtx(i);
        std::tuple<uintE, W>* tmp_v = tmp.start() + space[i];
        G[v].packOutNgh(v, p, tmp_v);
      };
      parallel_for_bc(i, 0, m, true, { for_inner(i); });
    }
    return vertexSubsetData<uintE>(n);
  }
}

template <template <class W> class wvertex, class W, class P>
inline vertexSubsetData<uintE> edgeMapFilter(graph<wvertex<W>>& GA,
                                             vertexSubset& vs, P& p,
                                             const flags& fl = 0) {
  using vertex = wvertex<W>;
  vs.toSparse();
  if (fl & pack_edges) {
    return packEdges<wvertex, W, P>(GA, vs, p, fl);
  }
  vertex* G = GA.V;
  long m = vs.numNonzeros();
  long n = vs.numRows();
  using S = std::tuple<uintE, uintE>;
  if (vs.size() == 0) {
    return vertexSubsetData<uintE>(n);
  }
  S* outV;
  if (should_output(fl)) {
    outV = newA(S, vs.size());
  }
  if (should_output(fl)) {
    parallel_for_bc(i, 0, m, true, {
      uintE v = vs.vtx(i);
      size_t ct = G[v].countOutNgh(v, p);
      outV[i] = std::make_tuple(v, ct);
    });
  } else {
    parallel_for_bc(i, 0, m, true, {
      uintE v = vs.vtx(i);
      G[v].countOutNgh(v, p);
    });
  }
  if (should_output(fl)) {
    return vertexSubsetData<uintE>(n, m, outV);
  } else {
    return vertexSubsetData<uintE>(n);
  }
}

//*****VERTEX FUNCTIONS*****

template <class F, class VS,
          typename std::enable_if<!std::is_same<VS, vertexSubset>::value,
                                  int>::type = 0>
inline void vertexMap(VS& V, F f) {
  size_t n = V.numRows(), m = V.numNonzeros();
  if (V.dense()) {
    parallel_for_bc(i, 0, n, (n > pbbs::kSequentialForThreshold), {
      if (V.isIn(i)) {
        f(i, V.ithData(i));
      }
    });
  } else {
    parallel_for_bc(i, 0, m, (m > pbbs::kSequentialForThreshold),
                    { f(V.vtx(i), V.vtxData(i)); });
  }
}

template <class VS, class F,
          typename std::enable_if<std::is_same<VS, vertexSubset>::value,
                                  int>::type = 0>
inline void vertexMap(VS& V, F f) {
  size_t n = V.numRows(), m = V.numNonzeros();
  if (V.dense()) {
    parallel_for_bc(i, 0, n, (n > pbbs::kSequentialForThreshold), {
      if (V.isIn(i)) {
        f(i);
      }
    });
  } else {
    parallel_for_bc(i, 0, m, (m > pbbs::kSequentialForThreshold),
                    { f(V.vtx(i)); });
  }
}

// Note: this is the version of vertexMap in which only a subset of the
// input vertexSubset is returned
template <class F>
inline vertexSubset vertexFilter(vertexSubset V, F filter) {
  long n = V.numRows();
  V.toDense();
  bool* d_out = newA(bool, n);
  parallel_for_bc(i, 0, n, (n > pbbs::kSequentialForThreshold),
                  { d_out[i] = 0; });
  parallel_for_bc(i, 0, n, (n > pbbs::kSequentialForThreshold), {
    if (V.d[i]) d_out[i] = filter(i);
  });
  return vertexSubset(n, d_out);
}

template <class F>
inline vertexSubset vertexFilter2(vertexSubset V, F filter) {
  long n = V.numRows(), m = V.numNonzeros();
  if (m == 0) {
    return vertexSubset(n);
  }
  bool* bits = newA(bool, m);
  V.toSparse();
  parallel_for_bc(i, 0, m, (m > pbbs::kSequentialForThreshold), {
    uintE v = V.vtx(i);
    bits[i] = filter(v);
  });
  auto v_imap = make_in_imap<uintE>(m, [&](size_t i) { return V.vtx(i); });
  auto bits_m = make_in_imap<bool>(m, [&](size_t i) { return bits[i]; });
  auto out = pbbs::pack(v_imap, bits_m);
  out.allocated = false;
  free(bits);
  return vertexSubset(n, out.size(), out.s);
}

template <class data, class F>
inline vertexSubset vertexFilter2(vertexSubsetData<data> V, F filter) {
  long n = V.numRows(), m = V.numNonzeros();
  if (m == 0) {
    return vertexSubset(n);
  }
  bool* bits = newA(bool, m);
  V.toSparse();
  parallel_for_bc(i, 0, m, (m > pbbs::kSequentialForThreshold), {
    auto t = V.vtxAndData(i);
    bits[i] = filter(std::get<0>(t), std::get<1>(t));
  });
  auto v_imap = make_in_imap<uintE>(m, [&](size_t i) { return V.vtx(i); });
  auto bits_m = make_in_imap<bool>(m, [&](size_t i) { return bits[i]; });
  auto out = pbbs::pack(v_imap, bits_m);
  out.allocated = false;
  free(bits);
  return vertexSubset(n, out.size(), out.s);
}

// Adds vertices to a vertexSubset vs.
// Caller must ensure that every v in new_verts is not already in vs
// Note: Mutates the given vertexSubset.
inline void add_to_vsubset(vertexSubset& vs, uintE* new_verts,
                           uintE num_new_verts) {
  if (vs.isDense) {
    parallel_for_bc(i, 0, num_new_verts,
                    (num_new_verts > pbbs::kSequentialForThreshold),
                    { vs.d[new_verts[i]] = true; });
    vs.m += num_new_verts;
  } else {
    const long vs_size = vs.numNonzeros();
    const long new_size = num_new_verts + vs_size;
    uintE* all_verts = newA(uintE, new_size);
    parallel_for_bc(i, 0, new_size, (new_size > pbbs::kSequentialForThreshold),
                    {
                      if (i < vs_size) {
                        all_verts[i] = vs.s[i];
                      } else {
                        all_verts[i] = new_verts[i - vs_size];
                      }
                    });
    uintE* old_s = vs.s;
    vs.s = all_verts;
    vs.m = new_size;
    if (old_s) {
      free(old_s);
    }
  }
}

// cond function that always returns true
inline bool cond_true(intT d) { return 1; }

#ifdef USE_PCM_LIB
inline void print_pcm_stats(SystemCounterState& before_sstate,
                            SystemCounterState& after_sstate, size_t rounds,
                            double elapsed) {
  std::cout << "Instructions per clock:        "
            << (getIPC(before_sstate, after_sstate) / rounds) << "\n";
  std::cout << "Total Cycles:                  "
            << (getCycles(before_sstate, after_sstate) / rounds) << "\n";
  std::cout << "========= Cache misses/hits ========="
            << "\n";
  std::cout << "L2 Hit ratio:                  "
            << (getL2CacheHitRatio(before_sstate, after_sstate) / rounds)
            << "\n";
  std::cout << "L3 Hit ratio:                  "
            << (getL3CacheHitRatio(before_sstate, after_sstate) / rounds)
            << "\n";
  std::cout << "L2 Misses:                     "
            << (getL2CacheMisses(before_sstate, after_sstate) / rounds) << "\n";
  std::cout << "L2 Hits:                       "
            << (getL2CacheHits(before_sstate, after_sstate) / rounds) << "\n";
  std::cout << "L3 Misses:                     "
            << (getL3CacheMisses(before_sstate, after_sstate) / rounds) << "\n";
  std::cout << "L3 Hits:                       "
            << (getL3CacheHits(before_sstate, after_sstate) / rounds) << "\n";
  std::cout << "========= Bytes read/written ========="
            << "\n";
  auto bytes_read = getBytesReadFromMC(before_sstate, after_sstate) / rounds;
  auto bytes_written =
      getBytesWrittenToMC(before_sstate, after_sstate) / rounds;
  size_t GB = 1024 * 1024 * 1024;
  auto throughput = ((bytes_read + bytes_written) / elapsed) / GB;
  std::cout << "Bytes read:                    " << bytes_read << "\n";
  std::cout << "Bytes written:                 " << bytes_written << "\n";
  std::cout << "Throughput: " << throughput << " GB/s"
            << "\n";
  std::cout << "========= Other statistics ========="
            << "\n";
  std::cout << "Average relative frequency:    "
            << (getActiveRelativeFrequency(before_sstate, after_sstate) /
                rounds)
            << "\n";
}
inline void pcm_init() {
  auto* m = PCM::getInstance();
  if (m->program() != PCM::Success) {
    std::cout << "Could not enable program counters"
              << "\n";
    exit(0);
  }
}
inline size_t get_pcm_state() { return getSystemCounterState(); }
#else
inline void print_pcm_stats(size_t before, size_t after, size_t rounds,
                            double elapsed) {}
inline void pcm_init() {}
inline size_t get_pcm_state() { return (size_t)1; }
#endif

#define run_app(G, APP, rounds)                                      \
  auto before_state = get_pcm_state();                               \
  timer st;                                                          \
  st.start();                                                        \
  for (int r = 0; r < rounds; r++) {                                 \
    timer at; at.start();                                            \
    APP(G, P);                                                       \
    at.stop(); at.reportTotal("Running time");                       \
  }                                                                  \
  auto time_per_iter = st.stop() / rounds;                           \
  std::cout << "time per iter: " << time_per_iter << "\n";           \
  auto after_state = get_pcm_state();                                \
  print_pcm_stats(before_state, after_state, rounds, time_per_iter); \
  G.del();

#define generate_main(APP, mutates)                                            \
  int main(int argc, char* argv[]) {                                           \
    commandLine P(argc, argv, " [-s] <inFile>");                               \
    char* iFile = P.getArgument(0);                                            \
    bool symmetric = P.getOptionValue("-s");                                   \
    bool compressed = P.getOptionValue("-c");                                  \
    bool binary = P.getOptionValue("-b");                                      \
    bool weighted = P.getOptionValue("-w");                                    \
    bool mmap = P.getOptionValue("-m");                                        \
    bool mmapcopy = mutates;                                                   \
    std::cout << "mmapcopy = " << mmapcopy << "\n";                            \
    long rounds = P.getOptionLongValue("-rounds", 3);                          \
    pcm_init();                                                                \
    if (compressed) {                                                          \
      if (symmetric) {                                                         \
        if (weighted) {                                                        \
          auto G = readCompressedGraph<csv_bytepd_amortized, intE>(            \
              iFile, symmetric, mmap, mmapcopy);                               \
          run_app(G, APP, rounds)                                              \
        } else {                                                               \
          auto G = readCompressedGraph<csv_bytepd_amortized, pbbs::empty>(     \
              iFile, symmetric, mmap, mmapcopy);                               \
          run_app(G, APP, rounds)                                              \
        }                                                                      \
      } else {                                                                 \
        if (weighted) {                                                        \
          auto G = readCompressedGraph<cav_bytepd_amortized, intE>(            \
              iFile, symmetric, mmap, mmapcopy);                               \
          run_app(G, APP, rounds)                                              \
        } else {                                                               \
          auto G = readCompressedGraph<cav_bytepd_amortized, pbbs::empty>(     \
              iFile, symmetric, mmap, mmapcopy);                               \
          run_app(G, APP, rounds)                                              \
        }                                                                      \
      }                                                                        \
    } else {                                                                   \
      if (symmetric) {                                                         \
        if (weighted) {                                                        \
          auto G = readWeightedGraph<symmetricVertex>(iFile, symmetric, mmap); \
          run_app(G, APP, rounds)                                              \
        } else {                                                               \
          auto G =                                                             \
              readUnweightedGraph<symmetricVertex>(iFile, symmetric, mmap);    \
          run_app(G, APP, rounds)                                              \
        }                                                                      \
      } else {                                                                 \
        if (weighted) {                                                        \
          auto G =                                                             \
              readWeightedGraph<asymmetricVertex>(iFile, symmetric, mmap);     \
          run_app(G, APP, rounds)                                              \
        } else {                                                               \
          auto G =                                                             \
              readUnweightedGraph<asymmetricVertex>(iFile, symmetric, mmap);   \
          run_app(G, APP, rounds)                                              \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }