def max_common_substring(x, y):
    xl = len(x) + 1
    yl = len(y) + 1
    max = 0
    begin = 0
    brochure = [[0 for i in range(yl)] for j in range(xl)]
    for xi in range(xl):
        for yi in range(yl):
            if xi == 0 or yi == 0:
                brochure[xi][yi] = 0
            elif x[xi-1] == y[yi-1]:
                brochure[xi][yi] = brochure[xi-1][yi-1] + 1
                if brochure[xi][yi] > max:
                    max = brochure[xi][yi]
                    begin = xi - max
            else:
                brochure[xi][yi] = 0

    return x[begin: begin+max]

x = "hellokev"
y = "abcdefghki"
print(max_common_substring(x, y))
