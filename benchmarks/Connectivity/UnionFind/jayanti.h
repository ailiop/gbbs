#pragma once

#include <algorithm>
#include "ligra/ligra.h"

namespace jayanti_rank {
  static constexpr uintE RANK_MASK = (uintE)INT_E_MAX;
  static constexpr uintE TOP_BIT_SHIFT = sizeof(uintE)*8 - 1;
  static constexpr uintE TOP_BIT = ((uintE)1) << TOP_BIT_SHIFT;

  struct vdata {
    volatile uintE rank; // top bit is used to indicate root or not
    volatile uintE parent; // parent id

    vdata() { }

    vdata(uintE _parent, uintE _rank, bool is_root) {
      rank = combine_root_rank(is_root, _rank);
      parent = _parent;
    }

    __attribute__((always_inline)) inline uintE combine_root_rank(bool is_root, uintE _rank) const {
      return (((uintE)is_root) << TOP_BIT_SHIFT) + _rank;
    }

    __attribute__((always_inline)) inline bool is_root() const {
      return rank & TOP_BIT;
    }

    __attribute__((always_inline)) inline uintE get_rank() const {
      return rank & RANK_MASK;
    }

     __attribute__((always_inline)) inline uintE get_parent() const {
      return parent;
    }

    void print(uintE vtx_id) const {
      cout << "vtx: " << vtx_id << " parent = " << get_parent() << " rank = " << get_rank() << " is_root = " << is_root() << endl;
    }
  };

  template <class S>
  void link(uintE u, uintE v, S& vdatas, pbbs::random r) {
    auto ud = vdatas[u];
    auto vd = vdatas[v];
    // spend two reads to abort early with no CASs if either of the
    // vertices has already been updated.
    if (ud.is_root() == false || vd.is_root() == false) {
      return;
    }
    // o.w. continue
    if (ud.get_rank() < vd.get_rank()) { // u.r < v.r
      auto expected_u = vdata(ud.get_parent(), ud.get_rank(), /* is_root= */ true);
      auto new_u = vdata(v, ud.get_rank(), /* is_root= */ false);
      pbbs::atomic_compare_and_swap(&(vdatas[u]), expected_u, new_u);
    } else if (ud.get_rank() > vd.get_rank()) { // v.r < u.r
      auto expected_v = vdata(vd.get_parent(), vd.get_rank(), /* is_root= */ true);
      auto new_v = vdata(u, vd.get_rank(), /* is_root= */ false);
      pbbs::atomic_compare_and_swap(&(vdatas[v]), expected_v, new_v);
    } else { // u.r == v.r
      auto random_bit = r.rand() & 1;
      if (u < v) {
        auto expected_u = vdata(ud.get_parent(), ud.get_rank(), /* is_root= */ true);
        auto new_u = vdata(v, ud.get_rank() + 1, /* is_root= */ random_bit);
        pbbs::atomic_compare_and_swap(&(vdatas[u]), expected_u, new_u);
      } else {
        auto expected_v = vdata(vd.get_parent(), vd.get_rank(), /* is_root= */ true);
        auto new_v = vdata(u, vd.get_rank() + 1, /* is_root= */ random_bit);
        pbbs::atomic_compare_and_swap(&(vdatas[v]), expected_v, new_v);
      }
    }
  }

  inline uintE find(uintE x, pbbs::sequence<vdata>& vdatas) {
    uintE u = x;
    while (!vdatas[u].is_root()) { // * on u.is_root()
      u = vdatas[u].get_parent();
    }
    return u; // u is a root
  }

  inline uintE find_twotry_splitting(uintE x, pbbs::sequence<vdata>& vdatas) {
    uintE u = x;
    while (!vdatas[u].is_root()) { // * on u.is_root()
      auto ud = vdatas[u]; uintE v = ud.get_parent();
      auto vd = vdatas[v];
      if (vd.is_root()) return v;

      // CAS 1
      uintE w = vd.get_parent();
      auto expected_u = vdata(v, ud.get_rank(), false);
      auto new_u = vdata(w, ud.get_rank(), false);
      pbbs::atomic_compare_and_swap<vdata>(&(vdatas[u]), expected_u, new_u);

      // read and check
      ud = vdatas[u]; v = ud.get_parent();
      vd = vdatas[v]; w = vd.get_parent();
      if (vd.is_root()) return v;

      // CAS 2
      expected_u = vdata(v, ud.get_rank(), false);
      new_u = vdata(w, ud.get_rank(), false);
      pbbs::atomic_compare_and_swap<vdata>(&(vdatas[u]), expected_u, new_u);

      u = v;
    }
    return u; // u is a root
  }

  template <class S, class Find>
  void unite(uintE x, uintE y, S& vdatas, pbbs::random r, Find& find) {
    uintE u = find(x, vdatas);
    uintE v = find(y, vdatas);
    while (u != v) {
      link(u, v, vdatas, r);
      u = find(u, vdatas);
      v = find(v, vdatas);
      r = r.next();
    }
  }

  // implementation of randomized linking-by-rank.
  // implementation of two-try splitting
  /* Implementation of randomized linking-by-rank as proposed in
   * Randomized Concurrent Set Union and Generalized Wake-Up
   * by Jayanti, Tarjan, and Boix-Adserà */
  template <class G, class Find>
  struct JayantiTBUnite {
    G& GA;
    Find& find;
    JayantiTBUnite(G& GA, Find& find) : GA(GA), find(find) {}

    template <bool provides_frequent_comp>
    pbbs::sequence<uintE> compute_components(pbbs::sequence<uintE>& parents, uintE frequent_comp = UINT_E_MAX) {
      using W = typename G::weight_type;
      size_t n = GA.n;

      auto vdatas = pbbs::sequence<vdata>(n);
      parallel_for(0, n, [&] (uintE i) {
        vdatas[i] = vdata(/* parent */ parents[i], /* rank */ 1, /* is_root */ true);
      });

      timer ut; ut.start();
      auto r = pbbs::random();

      parallel_for(0, n, [&] (size_t i) {
        auto map_f = [&] (uintE u, uintE v, const W& wgh) {
          auto r_u = r.fork(u);
          auto r_uv = r_u.fork(v);
          if (u < v) {
            unite(u, v, vdatas, r_uv, find);
          }
        };
        if constexpr (provides_frequent_comp) {
          if (parents[i] != frequent_comp) {
            GA.get_vertex(i).mapOutNgh(i, map_f); // in parallel
          }
        } else {
          GA.get_vertex(i).mapOutNgh(i, map_f);
        }
      }, 1);
      ut.stop(); ut.reportTotal("union time");

      timer ft; ft.start();
      parallel_for(0, n, [&] (size_t i) {
        parents[i] = find(i,vdatas);
      });
      ft.stop(); ft.reportTotal("find time");

      return parents;
    }
  };
} // namespace jayanti_rank