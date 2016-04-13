hole_distance = 91.6;
hole_position = (hole_distance/2)/cos(30);
whole_diameter = 141+45;
extra_distance = 20;

difference() {
  union() {
    difference() {
      cylinder(r=hole_position+4, h=8, $fn=128);
      translate([0, 0, -1]) {
        cylinder(r=hole_position-4, h=10, $fn=128);
      }
    }
    for(i=[0, 120, 240]) {
      rotate([0, 0, i]) {
        translate([hole_position, 0, 0]) {
          cylinder(d=12, h=8, $fn=32);
          cylinder(d=5, h=10, $fn=32);
        }
      }
    }
    rotate([0, 0, 180]) {
      translate([whole_diameter/2-17, 0, 0]) {
        cylinder(d=25+2*8, h=8, $fn=64);
      }
      translate([hole_position, -4, 0]) {
        cube([whole_diameter/2-hole_position+extra_distance, 8, 8]);
      }
      translate([whole_diameter/2+extra_distance, -4, 5]) {
        rotate([-90, 0, 0]) {
          cylinder(d=16, h=8, $fn=32);
        }
      }
    }
  }
  for(i=[0, 120, 240]) {
    rotate([0, 0, i]) {
      translate([hole_position, 0, -1]) {
        cylinder(d=3, h=12, $fn=16);
        cylinder(d=6.5, h=3.5, $fn=6);
      }
    }
  }
  rotate([0, 0, 180]) {
    translate([whole_diameter/2-17, 0, -1]) {
      cylinder(d=25, h=10, $fn=32);
    }
    translate([whole_diameter/2+extra_distance, 5, 5]) {
      rotate([90, 30, 0]) {
        cylinder(d=5, h=10, $fn=16);
        cylinder(d=sqrt(8*8+4*4), h=4.3, $fn=6);
      }
      translate([0, -5, -9]) {
        cube([16, 10, 8], center=true);
      }
    }
  }
}