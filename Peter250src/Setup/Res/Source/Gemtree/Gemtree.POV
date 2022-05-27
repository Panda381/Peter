
#include "Shapes.inc"
#include "Shapes2.inc"
#include "Colors.inc"

camera {
   location <0.0, 0.0, -200.0>
   direction <0.0, 0.0, 1.0>
   up <0.0, 1, 0.0>
   right <1, 0.0, 0.0>
   orthographic
}

union {
    object {
        Tetrahedron
        texture {
            pigment{ color BrightGold }
            finish {
                ambient 0.2
                diffuse 0.3
                reflection 0.1
                phong 0.75
                phong_size 4
            }
        }
        rotate -90*z
        rotate -60*x
    }

    object {
        Tetrahedron
        texture {
            pigment{ color Silver }
            finish {
                ambient 0.2
                diffuse 0.3
                reflection 0.1
                phong 0.75
                phong_size 4
            }
        }
        rotate 90*z
    }

    rotate -clock*360*y
    scale 33
}

background { color rgb <192/255, 192/255, 192/255> }

light_source { <30.0, 60.0, -200.0> colour White shadowless }
light_source { <15.0, 50.0, -200.0> colour White shadowless }
light_source { <0.0, 60.0, -200.0> colour White shadowless }
light_source { <-15.0, 50.0, -200.0> colour White shadowless }
light_source { <-30.0, 60.0, -200.0> colour White shadowless }
