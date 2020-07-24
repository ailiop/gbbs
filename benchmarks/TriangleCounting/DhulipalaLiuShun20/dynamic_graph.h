#pragma once

#include <tuple>
#include "gbbs/gbbs.h"
#include "pbbslib/monoid.h"
// #include "gbbs/pbbslib/sparse_table.h"
#include "sparse_table.h"
// #include "two_level_tables.h"
#include "set.h"
#include "gbbs/macros.h"

using namespace std;

#define EMPTYV numeric_limits<uintE>::max()
#define EMPTYKV make_tuple(EMPTYV, (SetT *)NULL)
#define UPDATET1 1
#define UPDATET2 2
#define UPDATET3 3
#define UPDATET4 4
#define UPDATET5 5


namespace gbbs{
namespace DBTGraph{
    using EdgeT = pair<uintE, uintE>;

    struct WTV{
        size_t c1, c2, c3;
        WTV():c1(0),c2(0),c3(0){}
        WTV(size_t cc1, size_t cc2, size_t cc3):c1(cc1),c2(cc2),c3(cc3){}
        WTV(size_t flag, size_t val):c1(flag),c2(val),c3(0){}

        // inline size_t getFlag(){return c1;}
        inline size_t getUpdateVal(){return c2;}
        inline void update(const  std::tuple<EdgeT, WTV>& kv){
            size_t flag  = std::get<1>(kv).c1;
            if(flag == UPDATET1){
                pbbslib::write_add(&c1, std::get<1>(kv).c2);
            }else if(flag == UPDATET2){
                pbbslib::write_add(&c2, std::get<1>(kv).c2);
            }else if(flag == UPDATET3){
                pbbslib::write_add(&c3, std::get<1>(kv).c2);
            }else if(flag == UPDATET4){
                pbbslib::write_add(&c2, std::get<1>(kv).c2);
            }else if(flag == UPDATET5){
                pbbslib::write_add(&c3, std::get<1>(kv).c2);
            }else{
                cout << "invalid update flag " << flag << endl;
                exit(1);
            }
        }
    };


    // auto hash_vertex = [](const uintE& v) {return pbbs::hash64_2(v);};
    struct vertexHash { //TODO: check
        uint64_t operator ()(const uintE& v) const {return pbbs::hash64_2(v);}
    };

    // auto hash_edge = [](const pair<uintE, uintE>& v) {return pbbs::hash_combine(v.first, v.second);};
    struct edgeHash { //TODO: check
        uint64_t operator ()(const pair<uintE, uintE>& v) const{return pbbs::hash_combine(v.first, v.second);}
    };


    inline pbbslib::sparse_set<uintE, vertexHash> make_vertex_set(size_t m, long space_mult=-1) {
        return pbbslib::sparse_set<uintE, vertexHash>(m, EMPTYV, vertexHash(), space_mult);
    }
    

    template <class Graph> //symmetric_graph
    class DyGraph{
        using vertex = typename Graph::vertex;
        using weight_type = typename Graph::weight_type;
        using edge_type = typename Graph::edge_type;
        using SetT = pbbslib::sparse_set<uintE, vertexHash >;
        using tableE = pbbslib::sparse_table<uintE, SetT*, vertexHash >;
        // using WTV = std::tuple<size_t, size_t, size_t>;
        using tableW = pbbslib::sparse_table<EdgeT, WTV, edgeHash>;
        // const 

        bool is_high(size_t k){ return k > t2;}
        bool is_low(size_t k){return !is_high(k);}

        bool is_high_v(uintE v){ return is_high(D[v]);}
        bool is_low_v(uintE v){return !is_high_v(v);}

        inline void insertTop(tableE *tb, uintE u, size_t size, size_t bottom_load ){
            SetT *tbB = new SetT(size, EMPTYV, vertexHash(), bottom_load);
            tb->insert(make_tuple(u, tbB));
        }

        inline void insertE(tableE *tb, uintE u, uintE v){
            SetT *tbB = tb->find(u, NULL);
            tbB->insert(v);
        }

        struct updateTF { //TODO: check
            void operator () (WTV* v0, const std::tuple<EdgeT, WTV>& kv)const {
                v0->update(kv);
            }
        };
        inline void insertW(uintE u, uintE v, int flag, size_t val = 1){
            if(u > v) swap(u,v);
            T->insert_f(make_tuple(make_pair(u,v), WTV(flag, val)), updateTF());

        }

    public:
        size_t block_size;
        size_t n;
        size_t m;
        size_t M;
        double t1, t2;
        pbbs::sequence<size_t> D;
        pbbs::sequence<size_t> lowD;
        pbbs::sequence<uintE> edges;
        tableE *LL;
        tableE *HH;
        tableE *LH;
        tableE *HL;
        tableW *T;


        DyGraph(int t_block_size, Graph& G):block_size(t_block_size){
            n = G.num_vertices();
            m = G.num_edges();
            M  = 2 * m + 1;
            t1 = sqrt(M) / 2;
            t2 = 3 * t1;

            vertex_data* v_data = G.v_data;
            D = pbbs::sequence<size_t>(n, [&](size_t i) { return G.get_vertex(i).getOutDegree(); });
            edges = pbbs::sequence<uintE>((size_t)(block_size*n));
            parallel_for(0, block_size*n, [&](size_t i) { edges[i] = 0; });
            lowD = pbbs::sequence<size_t>::no_init(n);
            
            //gather vertices with low/high neighbors
            // pbbs::sequence<std::tuple<uintE, uintE, W>> edgesAll = G.edges();
            pbbs::sequence<bool> flag = pbbs::sequence<bool>(m);
            parallel_for(0, m, [&](size_t i) { flag[i] = 0; });
            auto monoid = pbbslib::addm<size_t>();
            // par_for(0, m, [&] (size_t i) {
            //     if(is_low_v(get<2>(edgesAll[i]))) flag[i] = 1;
            // });
            pbbslib::scan_add_inplace(D.slice());
            parallel_for(0, n, [&](size_t i) {
            size_t k = D[i];
                auto map_f = [&](const uintE& u, const uintE& v, const typename Graph::weight_type& wgh) {
                    if(is_low_v(v)) flag[k] = 1;
                    k++;
                };
                G.get_vertex(i).mapOutNgh(i, map_f, false);
            }, 1);
            par_for(0, n, [&] (size_t i) {
                size_t offset = v_data[i].offset;
                size_t offset_next = i == n-1? m : v_data[i+1].offset;
                
                lowD[i] = pbbs::reduce(flag.slice(offset, offset_next), monoid);
            });

            flag.shrink(n);
            par_for(0, n, [&] (size_t i) {
                D[i] = G.get_vertex(i).getOutDegree(); 
                if(is_low_v(i)) flag[i] = 1;
                else flag[i] = 0;
            });
            size_t lowNum = pbbs::reduce(flag, monoid);

            LL = new tableE(lowNum, EMPTYKV, vertexHash(), 1.0);
            LH = new tableE(lowNum, EMPTYKV, vertexHash(), 1.0);
            HL = new tableE(n-lowNum, EMPTYKV, vertexHash(), 1.0);
            HH = new tableE(n-lowNum, EMPTYKV, vertexHash(), 1.0);
            T  = new tableW((n-lowNum)*(n-lowNum), make_tuple(make_pair(EMPTYV, EMPTYV), WTV()), edgeHash(), 1.0);

            // insert top level keys
            double bottom_load = 1.5;
            par_for(0, n, [&] (size_t i) {
                size_t degree = D[i];
                if(degree < block_size){
                    memcpy ( &edges[block_size*i], &G.e0[v_data[i].offset], degree*sizeof(edge_type) );
                }else{
                    // edges[offset*(i+1)-1] = NOEDGE;
                    tableE *tb1 = HL;
                    tableE *tb2 = HH;
                    if(is_high(degree)){
                        tb1 = LL;
                        tb2 = LH;
                    }
                    if(lowD[i] == 0){ //only has high neighbors
                        insertTop(tb2, i, degree, bottom_load);
                    }else if(lowD[i] < degree){
                        insertTop(tb1, i, lowD[i], bottom_load);
                        insertTop(tb2, i, degree - lowD[i], bottom_load);
                    }else if(lowD[i] == degree){//only has low neighbors
                        insertTop(tb1, i, degree, bottom_load);
                    }else{
                        cout << "dynamic_graph.h wrong size " << lowD[i] << endl;
                        exit(1);
                    }
                }
            });
            flag.clear();

            // insert bottom level
            auto insert_f = [&](const uintE& u, const uintE& v, const typename Graph::weight_type& wgh) {
                size_t degree = D[u];
                if(degree < block_size) return; // edges already in array
                if(is_low_v(u)){
                    if(is_low_v(v)){
                        insertE(LL, u,v);
                    }else{
                        insertE(LH, u,v);
                    }
                }else{
                    if(is_low_v(v)){
                        insertE(HL, u,v);
                    }else{
                        insertE(HH, u,v);
                    }
                }
            };
            G.mapEdges(insert_f, true);

            // init T
            par_for(0, HL->size(), [&] (size_t i) {
                uintE u = get<0>(HL->table[i]);
                SetT* L = get<1>(HL->table[i]);
                if(u != HL->empty_key){
                    par_for(0, L->size(), [&] (size_t j) {
                        uintE w = L->table[j];
                        if(w != L->empty_key){
                            insertT(u);
                        }
                    });
                }
            });

        }

        inline void insertT(uintE u){
            par_for(0, LH->size(), [&] (size_t k) {
                uintE v = get<0>(LH->table[k]);
                if(v != LH->empty_key){
                    insertW(u, v, UPDATET1, 1);
                }
            });
        }

        // ~(){
        //     HL->del();
        //     //todo: clear up
        // }
    };

}
}