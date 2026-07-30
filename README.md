# FAMCS-2026
## https://github.com/Wafflelover404/FAMCS-2026

Solutions to the three problems for the FAMCS 2026 entrance problem set.

| Problem | Topic | Key Files |
|---------|-------|-----------|
| Z1 (п.1-2) | Rational functions & roots of unity (elementary parts only) | `verify.cpp`, `Afanasyev_Z1.tex` |
| Z2 | Overlapping dominoes (rectangle peeling) | `overlap.cpp`, `stress.py`, `Afanasyev_Z2.tex` |
| Z3 | Permutations and n-cycles | `p3_brute.cpp`, `p3_construct.cpp`, `Afanasyev_Z3.tex` |

## Build & Run

```bash
# Z1 — numerical verification
g++ -O2 -std=c++17 -o verify verify.cpp && ./verify

# Z2 — overlapping dominoes solver
g++ -O2 -std=c++17 -o overlap overlap.cpp
./run_examples.sh

# Z3 — permutation brute-force & construction
g++ -O2 -std=c++17 -o p3_brute p3_brute.cpp && ./p3_brute 12
g++ -O2 -std=c++17 -o p3_construct p3_construct.cpp && ./p3_construct 2 40
```

## AI Disclosure

Solutions prepared with assistance from **GLM-5.2** and **OpenCode-CI-Agent**