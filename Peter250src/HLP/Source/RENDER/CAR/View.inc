
#include "colors.inc"

#include "Model.inc"

object { Model
    rotate -30*z
    rotate 75*x
    translate <-0.2, 0, 0>
}

camera {
   location  <0.0, 0, -100.0>
   up        <0.0, 0.185,   0.0>
   right     <0.185, 0,   0.0>
   look_at   <0.0, 0.0, 0>
}

background { color rgb <255/255, 255/255, 230/255> }

light_source { <50, 100, -60> colour rgb<0.6, 0.6, 0.6> }
light_source { <-200, 50, -100> colour rgb<0.6, 0.6, 0.6> }
light_source { <0, -20, -100> colour rgb<0.2, 0.2, 0.2> }
light_source { <0, 50, -100> colour rgb<0.2, 0.2, 0.2> }
light_source { <-30, 20, -100> colour rgb<0.2, 0.2, 0.2> }
light_source { <20, 20, -100> colour rgb<0.2, 0.2, 0.2> }
light_source { <-30, -30, 0> colour rgb<0.2, 0.2, 0.2> }
light_source { <20, -40, 0> colour rgb<0.2, 0.2, 0.2> }
