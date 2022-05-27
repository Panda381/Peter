
// ----------- Surfaces
#declare DarkSurface = finish {
    phong 0.3
    phong_size 2
    ambient 0.2
    diffuse 0.8
}
//------------ Wing
#declare Wing = intersection {
    box { <-8, 5, -0.2> <-2, 2, 0.2>}
    plane { <1, 2, 0> 3 }
    plane { <-1, 2, 0> 7 }
    plane { <-1, -4, 0>, -0.4 }
    texture {
        pigment { rgb <0.8, 0, 0> }
        finish { DarkSurface }
    }
}

// ********** Model declaration
#declare Model = union {

    union {
        cone { <2, 0, 0> 3 <7, 0, 0> 0 }
        intersection {
            sphere { <0,0,0> 4 scale <1.6,1,1>}
            plane {-x, 4}
            translate -2*x
        }
        texture {
            pigment { MainColor }
            finish { DarkSurface }
        }
    }

    cylinder { <6.6, 0, 0> <8.6, 0, 0> 0.2
        texture {
            pigment { rgb <0, 0, 0> }
            finish { DarkSurface }
        }
    }

    torus { 1.9 0.4
        rotate 90*z
        translate 3.5*x
        texture {
            pigment { rgb <0.6, 0.7, 0> }
            finish { DarkSurface }
        }
    }
    
    object { Wing }
    object { Wing rotate 60*x }
    object { Wing rotate 120*x }
    object { Wing rotate 180*x }
    object { Wing rotate 240*x }
    object { Wing rotate 300*x }

    intersection {
        sphere { <0,0,0> 4.1 scale <1.6,1,1>}

        union {
            cylinder { <0, 10, 0> <0, -10, 0> 1 rotate 30*x }
            cylinder { <0, 10, 0> <0, -10, 0> 1 rotate 90*x }
            cylinder { <0, 10, 0> <0, -10, 0> 1 rotate 150*x }
            translate 2*x
        }

        texture {
            pigment { rgb<0, 0, 1> }
            finish { DarkSurface }
        }
        translate -2*x
    }
}
