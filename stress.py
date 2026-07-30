import random
import subprocess
import sys

BIN = "./overlap"

def play(n, seqcolors):
    g = [[0]*n for _ in range(n)]
    for col in seqcolors:
        while True:
            horiz = random.random() < 0.5
            if horiz:
                r = random.randrange(n); c = random.randrange(n-1)
                cells = [(r,c),(r,c+1)]
            else:
                r = random.randrange(n-1); c = random.randrange(n)
                cells = [(r,c),(r+1,c)]
            break
        for (r,c) in cells: g[r][c] = col
    return g

def grid_str(g):
    return "\n".join(" ".join(map(str,row)) for row in g) + "\n"

def run(args, grid):
    p = subprocess.run([BIN]+args, input=grid, capture_output=True, text=True, check=False)
    return p.returncode

def main():
    random.seed(12345)
    T = int(sys.argv[1]) if len(sys.argv)>1 else 500
    fails = 0
    for t in range(T):
        n = random.randint(2,4)
        m = random.randint(1, 6)
        k = random.randint(1, 4)
        seq = [random.randint(1,k) for _ in range(m)]
        g = play(n, seq)
        gs = grid_str(g)
        kk = max(k, max((max(row) for row in g), default=0))
        counts = [0]*(kk+1)
        for x in seq: counts[x]+=1

        rc2 = run(["mode2", str(n), str(m)] + [str(x) for x in seq], gs)
        if rc2 != 0:
            print(f"[t{t}] M2 fail")
            print(gs, seq); fails+=1; continue

        args1 = ["mode1", str(n), str(kk)] + [str(counts[c]) for c in range(1,kk+1)]
        rc1 = run(args1, gs)
        if rc1 != 0:
            print(f"[t{t}] M1 fail")
            print(gs, counts[1:]); fails+=1; continue
    print(f"T:{T} fail:{fails}")
    sys.exit(1 if fails else 0)

if __name__=="__main__":
    main()
