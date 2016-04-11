raspi_width=61;
raspi_length=91;
whole_length=139;
raspi_hole_positions=[[6, 5], [6+49, 5], [6, 5+58], [6+49, 5+58]];
depth=37;
wall=3;
camera_holes=[[-10.5, 0], [10.5, 0], [-10.5, -12.5], [10.5, -12.5]];
switch_hole_diameter=6.5;
button_hole_diameter=7;
extra_cable=true;
T3_SSD=true;

module bottom_base() {
  difference() {
    translate([-wall, -wall, 0]) {
      cube([raspi_width+2*wall, whole_length+2*wall, depth+wall]);
    }
    translate([0, 0, wall]) {
      cube([raspi_width, raspi_length, depth+1]);
    }
    translate([0, raspi_length-1, 7]) {
      cube([raspi_width, whole_length-raspi_length+1, depth+1]);
    }
  }
}

module top_base() {
  difference() {
    translate([-wall, -wall, wall]) {
      cube([raspi_width+2*wall, whole_length+2*wall, depth+wall]);
    }
    translate([0, 0, wall-1]) {
      cube([raspi_width, whole_length, depth+1]);
    }
  }
}

module ssd() {
  translate([5, 0, 5.15]) {
    rotate([-90, 0, 0]) {
      cylinder(h=75, d=10.3, $fn=32);
    }
  }
  translate([58-5, 0, 5.15]) {
    rotate([-90, 0, 0]) {
      cylinder(h=75, d=10.3, $fn=32);
    }
  }
  translate([5, 0, 0]) {
    cube([48, 75, 10.3]);
  }
}

module bottom() {
  difference() {
    union() {
      bottom_base();
      for(pos=raspi_hole_positions) {
        translate([pos[0], pos[1], 0]) {
          cylinder(h=5, d=6, $fn=16);
        }
      }
      for(i=[3, raspi_width-3]) {
        translate([i, whole_length-3, 0]) {
          cylinder(h=30, d=3+2*wall, $fn=16);
        }
      }
      for(i=[3, raspi_width-3]) {
        difference() {
          translate([i, 3, 10]) {
            cylinder(h=20, d=3+2*wall, $fn=16);
          }
          translate([i, 3+2*wall, 10]) {
            rotate([45, 0, 0]) {
              cube([26, 26, 26], center=true);
            }
          }
        }
      }
      if(T3_SSD) {
        translate([-63-wall-10, whole_length-30, 0]) {
          cube([63+10, 10, 10]);
        }
        translate([-63-wall-10, whole_length-30-75+15, 0]) {
          cube([63+10, 10, 10]);
        }
      }
    }
    for(pos=raspi_hole_positions) {
      translate([pos[0], pos[1], -1]) {
        cylinder(h=7, d=3, $fn=16);
        cylinder(h=3.5, d=6.5, $fn=6);
      }
    }
    for(i=[raspi_width-5, 5]) {
      translate([i, raspi_length+7, 0]) {
        cylinder(h=depth, d=4, $fn=16);
      }
      translate([i, raspi_length+7, wall]) {
        cylinder(h=4.1, d1=4, d2=10, $fn=32);
      }
    }
    for(i=[3, raspi_width-3]) {
      translate([i, whole_length-3, -1]) {
        cylinder(h=depth+2*wall+2, d=3, $fn=16);
        cylinder(h=20+1, d=6.5, $fn=6);
      }
    }
    for(i=[3, raspi_width-3]) {
      translate([i, 3, 10-1]) {
        cylinder(h=depth+2*wall+2, d=3, $fn=16);
        rotate([0, 0, 30]) {
          cylinder(h=18, d=6.5, $fn=6);
        }
      }
    }
    translate([27.5, whole_length-1, 10]) {
      rotate([-90, 0, 0]) {
        cylinder(h=5, d=4, $fn=16);
      }
    }
    translate([46, whole_length-1, 13]) {
      rotate([-90, 0, 0]) {
        cylinder(h=5, d=7, $fn=16);
      }
    }
    if(T3_SSD) {
      translate([9, whole_length-1, 10]) {
        rotate([-90, 0, 0]) {
          cylinder(h=5, d=4, $fn=16);
        }
      }
      translate([-63.5, whole_length-30-75+8, 5]) {
        ssd();
      }
      translate([(-63.5)/2-wall, whole_length-22.5, 10]) {
        cube([30, 10, 10], center=true);
      }
      for(i=[whole_length-30+5, whole_length-30-55]) {
        translate([-69.5, i, -1]) {
          cylinder(h=depth+2*wall+1, d=3, $fn=16);
          cylinder(h=3.5, d=6.5, $fn=6);
        }
      }
    }
    if(extra_cable) {
      translate([15, whole_length-1, 10]) {
        rotate([-90, 0, 0]) {
          cylinder(h=5, d=7, $fn=16);
        }
      }
    }
    translate([raspi_width-1, 7, wall]) {
      cube([wall+2, 12.5, 7.5]);
    }
    translate([raspi_width+wall/2, 8+10.5/2, wall+7.5/2+depth/2]) {
      cube([wall, 20, depth-wall], center=true);
      cube([1, 20, depth-wall+0.5], center=true);
    }
    translate([raspi_width+wall/2, whole_length/2, wall+50/2+depth/2]) {
      cube([wall, whole_length+2*wall, depth-wall], center=true);
      cube([1, whole_length+2*wall, depth-wall+0.5], center=true);
    }
    translate([-wall/2, whole_length/2, wall+11/2+depth/2]) {
      cube([wall, whole_length+2*wall, depth-wall], center=true);
      cube([1, whole_length+2*wall, depth-wall+0.5], center=true);
    }
    translate([raspi_width/2, -wall/2, wall+55/2+depth/2]) {
      cube([raspi_width, wall, depth-wall], center=true);
      cube([raspi_width, 1, depth-wall+0.5], center=true);
    }
    translate([46, whole_length+1.5, (depth-13)/2+13+wall]) {
      cube([2*(raspi_width-46), 1, depth-13+2*wall+0.5], center=true);
      cube([2*(raspi_width-46), wall, depth-13+2*wall-0.5], center=true);
    }
    translate([46/2-(raspi_width-46)/2, whole_length+1.5, (depth-10)/2+10+wall]) {
      cube([raspi_width-2*(raspi_width-46), 1, depth-10+2*wall+0.5], center=true);
      cube([raspi_width-2*(raspi_width-46), wall, depth-10+2*wall-0.5], center=true);
    }
  }
}

module top() {
  difference() {
    union() {
      top_base();
      for(i=[3, raspi_width-3]) {
        for(j=[whole_length-3, 3]) {
          translate([i, j, 30]) {
            cylinder(h=depth-30+2*wall, d=3+2*wall, $fn=16);
          }
        }
      }
      if(T3_SSD) {
        translate([-63-wall-10, whole_length-30, 10]) {
          cube([63+10, 10, 10]);
        }
        translate([-63-wall-10, whole_length-30-75+15, 10]) {
          cube([63+10, 10, 10]);
        }
        difference() {
          translate([-63-wall-10, whole_length-30-2, 20-0.2]) {
            cube([63+10, 2, depth+2*wall-20+0.2]);
          }
          translate([-wall, whole_length-29, 20-0.2]) {
            rotate([180, 45+180, 0]) {
              cube([63+10, 5, depth+2*wall-20+0.2]);
            }
          }
        }
        difference() {
          translate([-63-wall-10, whole_length-20, 20-0.2]) {
            cube([63+10, 2, depth+2*wall-20+0.2]);
          }
          translate([-wall, whole_length-17, 20-0.2]) {
            rotate([180, 45+180, 0]) {
              cube([63+10, 5, depth+2*wall-20+0.2]);
            }
          }
        }
        difference() {
          translate([-63-wall-10, whole_length-60-30-2, 20-0.2]) {
            cube([63+10, 2, depth+2*wall-20+0.2]);
          }
          translate([-wall, whole_length-60-29, 20-0.2]) {
            rotate([180, 45+180, 0]) {
              cube([63+10, 5, depth+2*wall-20+0.2]);
            }
          }
        }
        difference() {
          translate([-63-wall-10, whole_length-20-60, 20-0.2]) {
            cube([63+10, 2, depth+2*wall-20+0.2]);
          }
          translate([-wall, whole_length-60-17, 20-0.2]) {
            rotate([180, 45+180, 0]) {
              cube([63+10, 5, depth+2*wall-20+0.2]);
            }
          }
        }
      }
    }
    bottom();
    for(i=[3, raspi_width-3]) {
      for(j=[whole_length-3, 3]) {
        translate([i, j, -1]) {
          cylinder(h=depth+2*wall+2, d=3, $fn=16);
        }
      }
    }
    translate([27.5, whole_length-1, 10]) {
      rotate([-90, 0, 0]) {
        cylinder(h=5, d=4, $fn=16);
      }
    }
    translate([46, whole_length-1, 13]) {
      rotate([-90, 0, 0]) {
        cylinder(h=5, d=7, $fn=16);
      }
    }
    if(T3_SSD) {
      translate([9, whole_length-1, 10]) {
        rotate([-90, 0, 0]) {
          cylinder(h=5, d=4, $fn=16);
        }
      }
      translate([-63.5, whole_length-30-75+8, 5]) {
        ssd();
      }
      translate([(-63.5)/2-wall, whole_length-22.5, 10]) {
        cube([30, 10, 10], center=true);
      }
      for(i=[whole_length-30+5, whole_length-30-55]) {
        translate([-69.5, i, -1]) {
          cylinder(h=depth+2*wall+1, d=3, $fn=16);
        }
      }
    }
    if(extra_cable) {
      translate([15, whole_length-1, 10]) {
        rotate([-90, 0, 0]) {
          cylinder(h=5, d=7, $fn=16);
        }
      }
    }
    translate([raspi_width-1, 8, wall]) {
      cube([wall+2, 10.5, 7.5]);
    }
    translate([raspi_width/2, raspi_length/2, depth+1.5*wall]) {
      cube([8.3, 8.3, wall+2], center=true);
    }
    for(hole=camera_holes) {
      translate([raspi_width/2+hole[0], raspi_length/2+hole[1], depth+wall-1]) {
        cylinder(h=wall+2, d=2, $fn=16);
      }
    }
    /*translate([raspi_width/2, 10, depth+wall-1]) {
      cylinder(h=wall+2, d=switch_hole_diameter, $fn=32);
    }*/
    translate([11, 10, depth+wall-1]) {
      cylinder(h=wall+2, d=switch_hole_diameter, $fn=32);
    }
    for(i=[0]) {
      translate([11, 25+i*10, depth+wall-1]) {
        cylinder(h=wall+2, d=button_hole_diameter, $fn=32);
      }
    }
  }
}

bottom();
//color([0.8, 0.2, 0.2]) top();

rotate([0, 180, 0]) translate([-2*wall-2-2*raspi_width, 0, -depth-2*wall]) top();