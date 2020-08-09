import cProfile
import pstats
import collections

length = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
prices = [1, 5, 8, 9, 10, 17, 17, 20, 24, 30]
items = list(zip(prices, length))

W = 15
items = [(4, 12), (2, 2), (1, 1), (10, 4), (2, 1)]


# unbounded knapsack problem
def ukp(W, items, mem={}):
    if mem.get(W):
        return mem[W]
    if W == 0:
        return 0
    max_ = 0
    for value, weight in items:
        if weight > W:
            continue
        v = ukp(W - weight, items) + value
        if max_ < v:
            max_ = v
    mem[W] = max_
    return max_


def ukp_bp(W, items):
    if W <= 0:
        return 0

    result = [0] * (W + 1)
    for i in range(1, W+1):
        result[i] = max(
            value + result[i-weight]
            for value, weight in items
            if weight <= i
        )
    return result[-1]


# 0/1 knapsack problem
def kp_01(W, items):
    result = collections.defaultdict(int)
    for i, item in enumerate(items, 1):
        value, weight = item
        for wx in range(W+1):
            if weight > wx:
                result[(i, wx)] = result[(i-1, wx)]
            else:
                result[(i, wx)] = max(
                    result[(i-1, wx)],
                    result[(i-1, wx-weight)] + value
                )
    return result[len(items), W]


def _kp_01_r(W, items, n):
    if W <= 0 or n <= 0:
        return 0

    value, weight = items[n-1]
    if weight > W:
        return _kp_01_r(W, items, n-1)
    else:
        return max(
            value + _kp_01_r(W-weight, items, n-1),
            _kp_01_r(W, items, n-1)
        )


def kp_01_r(W, items):
    return _kp_01_r(W, items, len(items))


for i in range(1, 400):
    assert ukp_bp(i, items) == ukp(i, items), f"ukp_01: {i} {ukp_bp(i, items)} {ukp(i, items)}"
    assert kp_01(i, items) == kp_01_r(i, items), f"ukp: {i} {kp_01(i, items)} {kp_01_r(i, items)}"

print("all pass")

# profiler = cProfile.Profile()
# profiler.runcall(lambda: ukp_um(W, items))

# stats = pstats.Stats(profiler)
# stats.strip_dirs()
# stats.sort_stats('cumulative')
# stats.print_stats()
