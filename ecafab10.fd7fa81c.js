(window.webpackJsonp=window.webpackJsonp||[]).push([[39],{141:function(e,a,t){"use strict";t.r(a),t.d(a,"frontMatter",(function(){return c})),t.d(a,"metadata",(function(){return b})),t.d(a,"rightToc",(function(){return p})),t.d(a,"default",(function(){return O}));var n=t(1),m=t(6),s=(t(0),t(147)),c={id:"low_diameter_decomposition",title:"Low Diameter Decomposition"},b={id:"benchmarks/connectivity/low_diameter_decomposition",title:"Low Diameter Decomposition",description:"## Problem Specification",source:"@site/docs/benchmarks/connectivity/low_diameter_decomposition.md",permalink:"/gbbs/docs/benchmarks/connectivity/low_diameter_decomposition",sidebar:"docs",previous:{title:"Connectivity",permalink:"/gbbs/docs/benchmarks/connectivity/connectivity"},next:{title:"Minimum Spanning Forest",permalink:"/gbbs/docs/benchmarks/connectivity/minimum_spanning_forest"}},p=[{value:"Problem Specification",id:"problem-specification",children:[]},{value:"Algorithm Implementations",id:"algorithm-implementations",children:[]},{value:"Cost Bounds",id:"cost-bounds",children:[]}],r={rightToc:p};function O(e){var a=e.components,t=Object(m.a)(e,["components"]);return Object(s.b)("wrapper",Object(n.a)({},r,t,{components:a,mdxType:"MDXLayout"}),Object(s.b)("h2",{id:"problem-specification"},"Problem Specification"),Object(s.b)("h4",{id:"input"},"Input"),Object(s.b)("p",null,Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"G"),Object(s.b)("mo",{parentName:"mrow"},"="),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"("),Object(s.b)("mi",{parentName:"mrow"},"V"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{separator:"true"}),","),Object(s.b)("mi",{parentName:"mrow"},"E"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),")")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"G=(V, E)")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.68333em",verticalAlign:"0em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"G"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mrel"}),"="),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}}))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"("),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.22222em"}}),"V"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mpunct"}),","),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.16666666666666666em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.05764em"}}),"E"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),")"))))),", an undirected graph on ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"n")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"n")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.43056em",verticalAlign:"0em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"n")))))," vertices. The input graph can\neither be weighted or unweighted."),Object(s.b)("h4",{id:"output"},"Output"),Object(s.b)("p",null,Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"C")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"C")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.68333em",verticalAlign:"0em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.07153em"}}),"C"))))),", a ",Object(s.b)("a",Object(n.a)({parentName:"p"},{href:"/benchmarks/definitions/"}),"mapping")," where ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"C"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"["),Object(s.b)("mi",{parentName:"mrow"},"v"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"]")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"C[v]")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.07153em"}}),"C"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"["),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.03588em"}}),"v"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),"]")))))," is a unique id\nbetween ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"["),Object(s.b)("mn",{parentName:"mrow"},"0"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{separator:"true"}),","),Object(s.b)("mi",{parentName:"mrow"},"n"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),")")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"[0, n)")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"["),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord"}),"0"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mpunct"}),","),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.16666666666666666em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"n"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),")")))))," representing the component of ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"v")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"v")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.43056em",verticalAlign:"0em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.03588em"}}),"v")))))," s.t. ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"C"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"["),Object(s.b)("mi",{parentName:"mrow"},"u"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"]"),Object(s.b)("mo",{parentName:"mrow"},"="),Object(s.b)("mi",{parentName:"mrow"},"C"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"["),Object(s.b)("mi",{parentName:"mrow"},"v"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"]")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"C[u] = C[v]")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.07153em"}}),"C"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"["),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"u"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),"]"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mrel"}),"="),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}}))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.07153em"}}),"C"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"["),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.03588em"}}),"v"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),"]"))))),"\nif and only if ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"u")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"u")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.43056em",verticalAlign:"0em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"u")))))," and ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"v")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"v")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.43056em",verticalAlign:"0em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.03588em"}}),"v")))))," are in the same connected component in ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"G")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"G")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.68333em",verticalAlign:"0em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"G"))))),"."),Object(s.b)("h2",{id:"algorithm-implementations"},"Algorithm Implementations"),Object(s.b)("p",null,"We provide multiple implementations of connectivity in GBBS. The\nprimary implementation is based on the low-diameter decomposition\nbased algorithm from ",Object(s.b)("a",Object(n.a)({parentName:"p"},{href:"https://dl.acm.org/doi/10.1145/2612669.2612692"}),"Shun et\nal."),"."),Object(s.b)("p",null,"The code for the primary implementation is available\n",Object(s.b)("a",Object(n.a)({parentName:"p"},{href:"https://github.com/ldhulipala/gbbs/tree/master/benchmarks/Connectivity/WorkEfficientSDB"}),"here"),"."),Object(s.b)("h2",{id:"cost-bounds"},"Cost Bounds"),Object(s.b)("p",null,"The algorithm runs in ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"O"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"("),Object(s.b)("mi",{parentName:"mrow"},"n"),Object(s.b)("mo",{parentName:"mrow"},"+"),Object(s.b)("mi",{parentName:"mrow"},"m"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),")")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"O(n + m)")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.02778em"}}),"O"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"("),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"n"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2222222222222222em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mbin"}),"+"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2222222222222222em"}}))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"m"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),")")))))," expected work and ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"O"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"("),Object(s.b)("msup",{parentName:"mrow"},Object(s.b)("mo",{parentName:"msup"},Object(s.b)("mi",{parentName:"mo"},"log"),Object(s.b)("mo",{parentName:"mo"},"\u2061")),Object(s.b)("mn",{parentName:"msup"},"3")),Object(s.b)("mi",{parentName:"mrow"},"n"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),")")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"O(\\log^{3} n)")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1.148448em",verticalAlign:"-0.25em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.02778em"}}),"O"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"("),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mop"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mop"}),"lo",Object(s.b)("span",Object(n.a)({parentName:"span"},{style:{marginRight:"0.01389em"}}),"g")),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"msupsub"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"vlist-t"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"vlist-r"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"vlist",style:{height:"0.8984479999999999em"}}),Object(s.b)("span",Object(n.a)({parentName:"span"},{style:{top:"-3.1473400000000002em",marginRight:"0.05em"}}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"pstrut",style:{height:"2.7em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"sizing reset-size6 size3 mtight"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mtight"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mtight"}),"3"))))))))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.16666666666666666em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"n"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),")"))))),"\ndepth w.h.p., and the proof can be found in the Shun et al. paper."))}O.isMDXComponent=!0},147:function(e,a,t){"use strict";t.d(a,"a",(function(){return i})),t.d(a,"b",(function(){return o}));var n=t(0),m=t.n(n);function s(e,a,t){return a in e?Object.defineProperty(e,a,{value:t,enumerable:!0,configurable:!0,writable:!0}):e[a]=t,e}function c(e,a){var t=Object.keys(e);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);a&&(n=n.filter((function(a){return Object.getOwnPropertyDescriptor(e,a).enumerable}))),t.push.apply(t,n)}return t}function b(e){for(var a=1;a<arguments.length;a++){var t=null!=arguments[a]?arguments[a]:{};a%2?c(Object(t),!0).forEach((function(a){s(e,a,t[a])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(t)):c(Object(t)).forEach((function(a){Object.defineProperty(e,a,Object.getOwnPropertyDescriptor(t,a))}))}return e}function p(e,a){if(null==e)return{};var t,n,m=function(e,a){if(null==e)return{};var t,n,m={},s=Object.keys(e);for(n=0;n<s.length;n++)t=s[n],a.indexOf(t)>=0||(m[t]=e[t]);return m}(e,a);if(Object.getOwnPropertySymbols){var s=Object.getOwnPropertySymbols(e);for(n=0;n<s.length;n++)t=s[n],a.indexOf(t)>=0||Object.prototype.propertyIsEnumerable.call(e,t)&&(m[t]=e[t])}return m}var r=m.a.createContext({}),O=function(e){var a=m.a.useContext(r),t=a;return e&&(t="function"==typeof e?e(a):b({},a,{},e)),t},i=function(e){var a=O(e.components);return m.a.createElement(r.Provider,{value:a},e.children)},j={inlineCode:"code",wrapper:function(e){var a=e.children;return m.a.createElement(m.a.Fragment,{},a)}},l=Object(n.forwardRef)((function(e,a){var t=e.components,n=e.mdxType,s=e.originalType,c=e.parentName,r=p(e,["components","mdxType","originalType","parentName"]),i=O(t),l=n,o=i["".concat(c,".").concat(l)]||i[l]||j[l]||s;return t?m.a.createElement(o,b({ref:a},r,{components:t})):m.a.createElement(o,b({ref:a},r))}));function o(e,a){var t=arguments,n=a&&a.mdxType;if("string"==typeof e||n){var s=t.length,c=new Array(s);c[0]=l;var b={};for(var p in a)hasOwnProperty.call(a,p)&&(b[p]=a[p]);b.originalType=e,b.mdxType="string"==typeof e?e:n,c[1]=b;for(var r=2;r<s;r++)c[r]=t[r];return m.a.createElement.apply(null,c)}return m.a.createElement.apply(null,t)}l.displayName="MDXCreateElement"}}]);