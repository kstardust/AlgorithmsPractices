class AABBBox:
    def __init__(self, x, y, w, h):
        self.x = x
        self.y = y
        self.w = w
        self.h = h

    def contains(self, x, y):
        """
        @params p: type Point(a namedtuple)
        """
        return x >= self.x and x <= self.x + self.w \
            and y >= self.y and y <= self.y + self.h

    def intersectsAABB(self, aabb):
        return self.x < aabb.x + aabb.w and \
            self.x + self.w > aabb.x and \
            self.y < aabb.y + aabb.h and \
            self.y + self.h > aabb.y

    def __str__(self):
        return f'x: {self.x} y: {self.y} w: {self.w} h: {self.h}'


class QuadTree:
    def __init__(self, w, h, capacity, x=0, y=0):
        self.cap = capacity
        self.x = x
        self.y = y
        self.w = w
        self.h = h
        self.aabb = AABBBox(self.x, self.y, self.w, self.h)

        # CAST http://mathonweb.com/help_ebook/html/cast.htm
        self.C = None
        self.A = None
        self.S = None
        self.T = None
        self.units = []

    def insert(self, x, y):
        if len(self.units) >= self.cap:
            self._subdivide()

        # has been subdivided
        if self.C is not None:
            return self.C.insert(x, y) or \
                self.A.insert(x, y) or \
                self.S.insert(x, y) or \
                self.T.insert(x, y)

        if self.aabb.contains(x, y):
            self.units += [(x, y)]
            return True
        return False

    def _subdivide(self):
        self.C = QuadTree(
            w=self.w/2, h=self.h/2, x=self.x+self.w/2, y=self.y,
            capacity=self.cap
        )
        self.A = QuadTree(
            w=self.w/2, h=self.h/2, x=self.x+self.w/2, y=self.y+self.h/2,
            capacity=self.cap
        )
        self.S = QuadTree(
            w=self.w/2, h=self.h/2, x=self.x, y=self.y+self.h/2,
            capacity=self.cap
        )
        self.T = QuadTree(
            w=self.w/2, h=self.h/2, x=self.x, y=self.y,
            capacity=self.cap
        )

        for unit in self.units:
            if self.C.insert(*unit) or \
               self.A.insert(*unit) or \
               self.S.insert(*unit) or \
               self.T.insert(*unit):
                continue
            assert False, f"cannot subdivide {unit}"
        self.units = []

    def query_range(self, aabb: AABBBox):
        if not self.aabb.intersectsAABB(aabb):
            return []

        res = [u for u in self.units if aabb.contains(*u)]
        if self.C is None:
            return res

        res += self.C.query_range(aabb)
        res += self.A.query_range(aabb)
        res += self.S.query_range(aabb)
        res += self.T.query_range(aabb)
        return res

    def __str__(self):
        return f'AABB: {self.aabb}\nunits: {self.units} \nC: {self.C} \nA: {self.A}\nS: {self.S}\nT: {self.T}'


t = QuadTree(10, 10, 2)
print(t.insert(2, 4))
print(t.insert(6, 8))
print(t.insert(1, 1))
print(t.insert(9, 9))
print(t.insert(7, 7))
print(t.query_range(AABBBox(5, 5, 10, 10)))
