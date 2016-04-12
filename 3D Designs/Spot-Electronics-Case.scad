wall = 3;
right_side = true;

mainboard_diameter = 87;
mainboard_hole_distance = 60;
mainboard_hole_position = (mainboard_hole_distance/2)/cos(30);
transformer_width = 85.5;
transformer_depth = 55;
transformer_height = 42.5;
transformer_hole_distance = 73;
transformer_hole_diameter = 4;
height = transformer_height+1*wall+2.5;

module dip_panel() {
  dip_holes_distance = 47.5;
  dip_poti_position = 9;
  dip_position = 15.5;
  dip_width = 22.5;
  dip_height = 10;
  dip_switch_position = 39;
  dip_switch_width = 4.5;
  dip_switch_height = 9;
  dip_module_height = 20;
  dip_module_width = 55;
  difference() {
    cube([dip_module_width, dip_module_height, wall]);
    first_hole_position = (dip_module_width-dip_holes_distance)/2;
    for(i=[first_hole_position, first_hole_position+dip_holes_distance]) {
      translate([i, dip_module_height/2, -1]) {
        cylinder(h=wall+2, d=3, $fn=16);
        translate([0, 0, 1+wall/2]) {
          cylinder(h=wall, d=5.3, $fn=16);
        }
      }
    }
    translate([first_hole_position+dip_poti_position, dip_module_height/2, -1]) {
      cylinder(h=wall+2, d=7, $fn=24);
    }
    translate([first_hole_position+dip_position, dip_module_height/2-dip_height/2, -1]) {
      cube([dip_width, dip_height, wall+2]);
    }
    for(i=[dip_module_height/2-dip_height/2, dip_module_height/2+dip_height/2]) {
      translate([first_hole_position+dip_position+dip_width/2, i, wall]) {
        rotate([45, 0, 0]) {
          cube([dip_width, wall, wall], center=true);
        }
      }
    }
    translate([first_hole_position+dip_switch_position, dip_module_height/2-dip_switch_height/2, -1]) {
      cube([dip_switch_width, dip_switch_height, wall+2]);
    }
  }
}

module XLR_panel() {
  hole_distance = 28.5;
  connector_diameter = 22.4;
  connector_distance = 31.5;
  xlr_module_width = 58.7;
  hole_position = sqrt((hole_distance/2*hole_distance/2)/2);
  connector_position = (xlr_module_width-connector_distance)/2;
  difference() {
    cube([xlr_module_width, 35+wall, wall]);
    for(i=[connector_position, connector_position+connector_distance]) {
      translate([i, 35/2+wall, -1]) {
        cylinder(d=connector_diameter, h=wall+2, $fn=48);
        for(j=[-1, 1]) {
          translate([j*hole_position, j*-1*hole_position, 0]) {
            cylinder(d=3, h=wall+2, $fn=16);
            translate([0, 0, 1+wall/2]) {
              cylinder(d=5.4, h=wall, $fn=16);
            }
          }
        }
      }
    }
  }
}

module case() {
  difference() {
    union() {
      translate([0, mainboard_diameter/2+transformer_depth/2, height/2]) {
        cube([transformer_width+2*wall, transformer_depth+2*wall, height], center=true);
      }
      difference() {
        cylinder(d=mainboard_diameter+2*wall, h=height, $fn=128);
        translate([-58.7/2, -mainboard_diameter/2-wall, height-35]) {
          cube([58.7, mainboard_diameter+wall, 35]);
        }
      }
      translate([-58.7/2, -mainboard_diameter/2, height-35-wall]) {
        rotate([90, 0, 0]) {
          XLR_panel();
        }
        translate([-wall, 0, 0]) {
          cube([58.7+2*wall, mainboard_diameter, 35+wall]);
        }
      }
      for(i=[-1, 1]) {
        translate([i*58.7/2, -mainboard_diameter/2, height-35-wall]) {
          difference() {
            cylinder(r=wall, h=35+wall, $fn=16);
            translate([0, (i-1)*wall, wall]) {
              rotate([0, 0, (i+1)*90]) {
                cube([2*wall, 2*wall, 35]);
              }
            }
          }
        }
      }
      if(!right_side) {
        translate([30, -30, 0]) {
          cube([30, 10, 33]);
        }
        translate([31.7, -5.8, 0]) {
          rotate([0, 0, -45]) {
            cube([30, 10, 33]);
          }
        }
        translate([50, -45, 0]) {
          cube([10, 25, 33]);
        }
      }
      if(right_side) {
        translate([-60, -30, 0]) {
          cube([30, 10, 33]);
        }
        translate([-38.8, 1.2, 0]) {
          rotate([0, 0, -45-90]) {
            cube([30, 10, 33]);
          }
        }
        translate([-60, -45, 0]) {
          cube([10, 25, 33]);
        }
      }
    }
    for(i=[-1, 1]) {
      translate([i*40, -37, 24]) {
        rotate([0, i*90, 0]) {
          cylinder(d=5, h=30, $fn=16);
          cylinder(d=sqrt(8*8+4*4), h=14.3, $fn=6);
        }
      }
    }
    translate([0, 0, wall+1.5]) {
      cylinder(d=87, h=height, $fn=128);
    }
    translate([-transformer_width/2, mainboard_diameter/2, wall+2.5]) {
      cube([transformer_width, transformer_depth, transformer_height]);
    }
    for(i=[-1, 1]) {
      translate([i*transformer_hole_distance/2, mainboard_diameter/2+transformer_depth/2, -1]) {
        cylinder(d=4, h=wall+4.5, $fn=18);
        cylinder(d=sqrt(7*7+3.5*3.5)+0.25, h=1+3.4, $fn=6);
      }
    }
    translate([-58.7/2, -mainboard_diameter/2, height-35]) {
      cube([58.7, mainboard_diameter, 35]);
    }
    for(i=[0, 120, 240]) {
      rotate([0, 0, i]) {
        translate([0, mainboard_hole_position, -1]) {
          cylinder(d=3, h=wall+3.5, $fn=16);
          cylinder(d=6.5, h=3.5, $fn=6);
        }
      }
    }
    translate([transformer_width/2-11, mainboard_diameter/2+transformer_depth-1, height-11-wall]) {
      rotate([-90, 0, 0]) {
        cylinder(d=16, h=wall+2, $fn=32);
      }
    }
  }
}

case();
difference() {
  translate([0, -87/2-wall, 5.1]) {
    cube([58.7+2*wall, 2*wall, 10.2], center=true);
  }
  translate([-58.7/2, -87/2-wall, 0]) {
    cube([58.7, wall+1, 10]);
  }
  cylinder(d=87+2*wall+0.5, h=10, $fn=128);
  for(i=[-1, 1]) {
    translate([i*58.7/2, -87/2, 0]) {
      cylinder(d=2*wall, h=10, $fn=16);
    }
  }
}

module top() {
  union() {
    difference() {
      union() {
        cylinder(d=mainboard_diameter+2*wall, h=wall, $fn=128);
        for(i=[-1, 1]) {
          translate([i*58.7/2, -mainboard_diameter/2, 0]) {
            cylinder(d=2*wall, h=wall, $fn=16);
          }
        }
        translate([-58.7/2, -mainboard_diameter/2-wall, 0]) {
          cube([58.7, wall, wall]);
        }
        translate([-58.7/2-wall, -mainboard_diameter/2, 0]) {
          cube([58.7+2*wall, mainboard_diameter+wall, wall]);
        }
        translate([0, mainboard_diameter/2+transformer_depth/2, wall/2]) {
          cube([transformer_width+2*wall, transformer_depth+2*wall, wall], center=true);
        }
      }
      cube([55, 20, 2*wall+1], center=true);
    }
    translate([-55/2, -20/2, 0]) {
      dip_panel();
    }
  }
}

color([0.2, 0.2, 0.8]) {
  translate([0, 0, height]) {
    top();
  }
}