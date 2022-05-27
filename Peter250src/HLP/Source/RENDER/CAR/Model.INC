
// ----------- Surfaces
#declare DarkSurface = finish {
    phong 0.3
    phong_size 2
    ambient 0.2
    diffuse 0.8
}

#declare LightSurface = finish {
    phong 0.4
    phong_size 2
    ambient 1
    diffuse 0
}

// ----------- Wheel declaration
#declare Wheel = union {
    torus { 1, 0.4
        texture {
            pigment { rgb <0.1 0.1, 0.1> }
        }
    }
    cylinder { <0, -0.1, 0>, <0, 0.1, 0>, 1.3
        texture {
            pigment { rgb <0.75, 0.75, 0.75> }
        }
    }

    finish { DarkSurface }
}

// *********** Model declaration
#declare Model = union {

// =========== Body
    difference {
        union {

// ----------- Top
            intersection {
                sphere { <-1, 0, 2>, 5 scale <2, 1.5, 1> }
                plane { <0.6, 0, -1> 2.5 }
                plane { <-1.2, 0, -1> 5.6 }
                plane { <0, 4, -1> 4 }
                plane { <0, -4, -1> 4 }
                plane { <-0.03, 0, 1> 0 }

                plane { <0, 1, 0> 4 }
                plane { <0, -1, 0> 4 }
                plane { <0.05, 1, 0> 4.2 }
                plane { <0.05, -1, 0> 4.2 }
                plane { <-1, 0, 0> 6.9 }
            }

// ----------- Bottom
            intersection {
                plane { <0.03, 0, -1> 0 }
                plane { <0, -1, 0.2> 4 }
                plane { <0, 1, 0.2> 4 }
                plane { <-1, 0, 0.3> 7 }
                plane { <1, 0, 0.1> 9 }
                plane { <0, 0, 1> 3 }
                plane { <1, 0, -4> 1.1 }

                plane { <0, 1, 0> 4 }
                plane { <0, -1, 0> 4 }
                plane { <0.05, 1, 0> 4.2 }
                plane { <0.05, -1, 0> 4.2 }
                plane { <-1, 0, 0> 6.9 }

                plane { <1, 0, -0.2> 8.2 }
            }
        }

// ---------- inside off
        intersection {
            sphere { <-1, 0, 6.6>, 9 scale <2, 1.5, 1> }
            box { <-10, -3.3, -3>, <10, 3.3, 0.2> }
        }

        difference {
            intersection {
                sphere { <-1, 0, 6.8>, 9 scale <2, 1.5, 1> }
                box { <-4.7, -10, -3>, <4, 10, 0.2> }
                plane { <1, 0, -1.8> 2.1 }
            }
            box { <-1.3, -10, -3.1>, <-1, 10, 0.3> }
        }

// ------- wheels off
        cylinder { <-3.8, 2, 2.8>, <-3.8, 10, 2.8> 1.5 }
        cylinder { <6.5, 2, 2.8>, <6.5, 10, 2.8> 1.5 }
        cylinder { <-3.8, -2, 2.8>, <-3.8, -10, 2.8> 1.5 }
        cylinder { <6.5, -2, 2.8>, <6.5, -10, 2.8> 1.5 }

// ----------- maincolor
        texture {
            pigment { MainColor }
            finish { DarkSurface }
       }
    }

// =========== Window
    intersection {
        sphere { <-1, 0, 6.7>, 9 scale <2, 1.5, 1> }
        plane { <0.6, 0, -1> 2.4 }
        plane { <-1.2, 0, -1> 5.5 }
        plane { <0, 4, -1> 3.9 }
        plane { <0, -4, -1> 3.9 }
        plane { <-0.03, 0, 1> 0.3 }

        plane { <0, 1, 0> 3.9 }
        plane { <0, -1, 0> 3.9 }
        plane { <0.05, 1, 0> 4.1 }
        plane { <0.05, -1, 0> 4.1 }
        plane { <-1, 0, 0> 6.8 }

        texture {
            pigment { rgb <0, 0, 0.6> }
            finish { DarkSurface }
        }
    }

// ======== Wipers
    union {
        cylinder { <5, 2.2, 0.1> <4.8, -0.8, -0.3> 0.1 }
        cylinder { <5, 0.2, 0.1> <4.8, -2.8, -0.3> 0.1 }
        texture {
            pigment { rgb <0.1 0.1, 0.1> }
            finish { DarkSurface }
        }
    }

// ========== Wheels
    object { Wheel translate <-3.8, -3, 2.8> }
    object { Wheel translate <6.5, -3, 2.8> }
    object { Wheel translate <-3.8, 3, 2.8> }
    object { Wheel translate <6.5, 3, 2.8> }

// ========== Lights
    union {
        box { <8.74, 3, 1.2> <8, 1.5, 2.2> }
        box { <8.74, -3, 1.2> <8, -1.5, 2.2> }
        texture {
            pigment { White }
            finish { LightSurface }
        }
    }

    union {
        box { <-7, 2.7, 1.15> <-6, 1.7, 1.65> }
        box { <-7, -2.7, 1.15> <-6, -1.7, 1.65> }
        texture {
            pigment { Red }
            finish { LightSurface }
        }
    }

// ========== Blinker
    union {
        box { <8.74, 3.8, 1.2> <8.4, 3, 1.9 > }
        cylinder { <5, 3, 1.3>, <5, 3.88, 1.3>, 0.25 }
        box { <-7, 3.7, 1.15> <-6, 2.7, 1.65 > }

        texture {
            pigment { rgb <0.7, 0.7, 0> }
            finish { DarkSurface }
        }
    }

    union {
        box { <8.74, -3.8, 1.2> <8.4, -3, 1.9 > }
        cylinder { <5, -3, 1.3>, <5, -3.88, 1.3>, 0.25 }
        box { <-7, -3.7, 1.15> <-6, -2.7, 1.65 > }

        texture {
            pigment { rgb <0.7, 0.7, 0> }
            finish { DarkSurface }
        }
    }

// ========== Bumpers
    union {
        box { <9.4, 3.2, 2.3> <8, -3.2, 2.9> }
        cylinder { <8.7, 3.2, 2.3>, <8.7, 3.2, 2.9>, 0.7 }
        cylinder { <8.7, -3.2, 2.3>, <8.7, -3.2, 2.9>, 0.7 }

        box { <-7.3, 3.3, 1.8> <-5.9, -3.3, 2.4> }
        cylinder { <-6.6, 3.3, 1.8>, <-6.6, 3.3, 2.4>, 0.7 }
        cylinder { <-6.6, -3.3, 1.8>, <-6.6, -3.3, 2.4>, 0.7 }

        texture {
            pigment { rgb <0, 0, 0> }
            finish { DarkSurface }
        }
    }

// ====== Signs
    union {
        box { <9.5, 1, 2.4> <9.4, -1, 2.8> }
        box { <-7, 1, 1.2> <-6.9, -1, 1.6> }

        texture {
            pigment { rgb <0.6, 0.6, 0.6> }
            finish { DarkSurface }
        }
    }

// ======== Firma
    cylinder { <8.7, 0, 1.6> <8.4, 0, 1.8> 0.2

        texture {
            pigment { rgb < 0, 0.9, 0.4> }
            finish { DarkSurface }
        }
    }

// ======= Mirrors
    union {
        cone { <4, 4, 0.5>, 0.5, <4.8, 4, 0.5>, 0.1 }
        cone { <4, -4, 0.5>, 0.5, <4.8, -4, 0.5>, 0.1 }

        texture {
            pigment { rgb <0, 0, 0> }
            finish { DarkSurface }
        }
    }

    translate -1*x
    translate -1*z
}
