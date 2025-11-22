class C { bit V = 1; }
class D : C { let V = 0; }

let V=1 in {
    def E : D {}
}


def Z : D;