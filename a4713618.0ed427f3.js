(window.webpackJsonp=window.webpackJsonp||[]).push([[25],{126:function(e,t,n){"use strict";n.r(t),n.d(t,"frontMatter",(function(){return i})),n.d(t,"metadata",(function(){return c})),n.d(t,"rightToc",(function(){return l})),n.d(t,"default",(function(){return b}));var r=n(1),a=n(6),o=(n(0),n(145)),i={id:"sage",title:"Sage (Semi-Asymmetric Graph Engine)"},c={id:"sage/sage",title:"Sage (Semi-Asymmetric Graph Engine)",description:"This page contains information about how to build and run the",source:"@site/docs/sage/sage.md",permalink:"/gbbs/docs/sage/sage"},l=[{value:"Download",id:"download",children:[]},{value:"Graph Formats",id:"graph-formats",children:[]},{value:"Compilation",id:"compilation",children:[]},{value:"Running Benchmarks",id:"running-benchmarks",children:[]},{value:"Contact",id:"contact",children:[]}],s={rightToc:l};function b(e){var t=e.components,n=Object(a.a)(e,["components"]);return Object(o.b)("wrapper",Object(r.a)({},s,n,{components:t,mdxType:"MDXLayout"}),Object(o.b)("p",null,"This page contains information about how to build and run the\nNVRAM-based code used in our paper ",Object(o.b)("a",Object(r.a)({parentName:"p"},{href:"https://arxiv.org/abs/1910.12310"}),"Sage: Parallel Semi-Asymmetric\nGraph Algorithms for NVRAMs"),"."),Object(o.b)("h2",{id:"download"},"Download"),Object(o.b)("p",null,"Our code is currently included as part of the GBBS benchmarks, under the\ntop-level ",Object(o.b)("inlineCode",{parentName:"p"},"sage")," directory. You can download the code by cloning the\ngbbs repository through ssh (see our\n",Object(o.b)("a",Object(r.a)({parentName:"p"},{href:"https://github.com/ParAlg/gbbs"}),"github")," for other download options)."),Object(o.b)("pre",null,Object(o.b)("code",Object(r.a)({parentName:"pre"},{}),"git clone git@github.com:ParAlg/gbbs.git\n")),Object(o.b)("h2",{id:"graph-formats"},"Graph Formats"),Object(o.b)("p",null,"The code currently requires users to use either the binary\ncompressed-sparse row format (binary CSR), or the parallel byte-encoded\ncompressed-sparse row format (parallel-byte CSR). Both formats are supported by GBBS, and\nwe provide more information about the formats\n",Object(o.b)("a",Object(r.a)({parentName:"p"},{href:"https://paralg.github.io/gbbs/docs/formats"}),"here"),"."),Object(o.b)("p",null,"The default format is the binary CSR. Specifying that the input graph\nis compressed is done through the command-line argument ",Object(o.b)("inlineCode",{parentName:"p"},"-c"),"."),Object(o.b)("h2",{id:"compilation"},"Compilation"),Object(o.b)("p",null,"Compilation requires the ",Object(o.b)("inlineCode",{parentName:"p"},"bazel")," build tool, which you can find\ninstallation instructions for\n",Object(o.b)("a",Object(r.a)({parentName:"p"},{href:"https://docs.bazel.build/versions/master/install.html"}),"here"),"."),Object(o.b)("p",null,"After installing bazel, all of the Sage implementations can be built\nby running:"),Object(o.b)("pre",null,Object(o.b)("code",Object(r.a)({parentName:"pre"},{}),"bazel build //sage/...\n")),Object(o.b)("p",null,"This command will compile binaries and store them in the top-level\n",Object(o.b)("inlineCode",{parentName:"p"},"bazel-bin")," directory. For example, the BFS benchmark is located at:"),Object(o.b)("pre",null,Object(o.b)("code",Object(r.a)({parentName:"pre"},{}),"bazel-bin/sage/benchmarks/BFS/BFS_main\n")),Object(o.b)("h2",{id:"running-benchmarks"},"Running Benchmarks"),Object(o.b)("p",null,"After compilation, benchmarks can be run as follow, e.g., for BFS:"),Object(o.b)("pre",null,Object(o.b)("code",Object(r.a)({parentName:"pre"},{}),"./bazel-bin/sage/benchmarks/Connectivity/Connectivity_main -s -m -f1 /mnt/pmem0/soc-LiveJournal1_sym.binary -f2 /mnt/pmem1/soc-LiveJournal1_sym.binary\n")),Object(o.b)("p",null,"where ",Object(o.b)("inlineCode",{parentName:"p"},"-f1")," and ",Object(o.b)("inlineCode",{parentName:"p"},"-f2")," specify paths to graphs stored on persistent\nmemory attached to the 1st and 2nd sockets, respectively."),Object(o.b)("p",null,"If the system is run on a single socket system, or graph is served\nfrom DRAM (in which case the NUMA effect is less severe), both\narguments should supply the same graph."),Object(o.b)("h2",{id:"contact"},"Contact"),Object(o.b)("p",null,"Please contact ",Object(o.b)("a",Object(r.a)({parentName:"p"},{href:"mailto:ldhulipa@cs.cmu.edu"}),"ldhulipa@cs.cmu.edu")," with any questions about using\nSage, or assistance with running experiments."))}b.isMDXComponent=!0},145:function(e,t,n){"use strict";n.d(t,"a",(function(){return p})),n.d(t,"b",(function(){return d}));var r=n(0),a=n.n(r);function o(e,t,n){return t in e?Object.defineProperty(e,t,{value:n,enumerable:!0,configurable:!0,writable:!0}):e[t]=n,e}function i(e,t){var n=Object.keys(e);if(Object.getOwnPropertySymbols){var r=Object.getOwnPropertySymbols(e);t&&(r=r.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),n.push.apply(n,r)}return n}function c(e){for(var t=1;t<arguments.length;t++){var n=null!=arguments[t]?arguments[t]:{};t%2?i(Object(n),!0).forEach((function(t){o(e,t,n[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(n)):i(Object(n)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(n,t))}))}return e}function l(e,t){if(null==e)return{};var n,r,a=function(e,t){if(null==e)return{};var n,r,a={},o=Object.keys(e);for(r=0;r<o.length;r++)n=o[r],t.indexOf(n)>=0||(a[n]=e[n]);return a}(e,t);if(Object.getOwnPropertySymbols){var o=Object.getOwnPropertySymbols(e);for(r=0;r<o.length;r++)n=o[r],t.indexOf(n)>=0||Object.prototype.propertyIsEnumerable.call(e,n)&&(a[n]=e[n])}return a}var s=a.a.createContext({}),b=function(e){var t=a.a.useContext(s),n=t;return e&&(n="function"==typeof e?e(t):c({},t,{},e)),n},p=function(e){var t=b(e.components);return a.a.createElement(s.Provider,{value:t},e.children)},u={inlineCode:"code",wrapper:function(e){var t=e.children;return a.a.createElement(a.a.Fragment,{},t)}},m=Object(r.forwardRef)((function(e,t){var n=e.components,r=e.mdxType,o=e.originalType,i=e.parentName,s=l(e,["components","mdxType","originalType","parentName"]),p=b(n),m=r,d=p["".concat(i,".").concat(m)]||p[m]||u[m]||o;return n?a.a.createElement(d,c({ref:t},s,{components:n})):a.a.createElement(d,c({ref:t},s))}));function d(e,t){var n=arguments,r=t&&t.mdxType;if("string"==typeof e||r){var o=n.length,i=new Array(o);i[0]=m;var c={};for(var l in t)hasOwnProperty.call(t,l)&&(c[l]=t[l]);c.originalType=e,c.mdxType="string"==typeof e?e:r,i[1]=c;for(var s=2;s<o;s++)i[s]=n[s];return a.a.createElement.apply(null,i)}return a.a.createElement.apply(null,n)}m.displayName="MDXCreateElement"}}]);