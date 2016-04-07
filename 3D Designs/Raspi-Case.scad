raspi_width=58;
raspi_length=90;
whole_length=136;
raspi_hole_positions=[[5, 4], [53, 4], [5, 64], [53, 64]];
depth=20;
wall=3;
camera_holes=[[-10.5, 0], [10.5, 0], [-10.5, -12.5], [10.5, -12.5]];

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

module bottom() {
  difference() {
    union() {
      bottom_base();
      for(pos=raspi_hole_positions) {
        translate([pos[0], pos[1], 0]) {
          cylinder(h=5, d=6, $fn=16);
        }
      }
    }
    for(pos=raspi_hole_positions) {
      translate([pos[0], pos[1], -1]) {
        cylinder(h=7, d=3, $fn=16);
        cylinder(h=2.5, d=6.5, $fn=6);
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
    translate([raspi_width-1, 8, wall]) {
      cube([wall+2, 10.5, 7.5]);
    }
    translate([raspi_width+wall/2, 8+10.5/2, wall+7.5/2+depth/2]) {
      cube([wall+2, 20, depth-wall], center=true);
      cube([1, 20, depth-wall+0.5], center=true);
    }
    translate([raspi_width+wall/2, whole_length/2, wall+15/2+depth/2]) {
      cube([wall, whole_length+2*wall, depth-wall], center=true);
      cube([1, whole_length+2*wall, depth-wall+0.5], center=true);
    }
    translate([-wall/2, whole_length/2, wall+15/2+depth/2]) {
      cube([wall, whole_length+2*wall, depth-wall], center=true);
      cube([1, whole_length+2*wall, depth-wall+0.5], center=true);
    }
    translate([raspi_width/2, -wall/2, wall+20/2+depth/2]) {
      cube([raspi_width, wall, depth-wall], center=true);
      cube([raspi_width, 1, depth-wall+0.5], center=true);
    }
    translate([46, whole_length+1.5, (depth-13)/2+13+wall]) {
      cube([2*(raspi_width-46), 1, depth-13+2*wall+0.5], center=true);
      cube([2*(raspi_width-46), 5, depth-13+2*wall-0.5], center=true);
    }
    translate([46/2-(raspi_width-46)/2, whole_length+1.5, (depth-10)/2+10+wall]) {
      cube([raspi_width-2*(raspi_width-46), 1, depth-10+2*wall+0.5], center=true);
      cube([raspi_width-2*(raspi_width-46), 5, depth-10+2*wall-0.5], center=true);
    }
  }
}

module top() {
  difference() {
    union() {
      top_base();
    }
    bottom();
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
    translate([raspi_width-1, 8, wall]) {
      cube([wall+2, 10.5, 7.5]);
    }
    translate([raspi_width/2, raspi_length/2, depth+1.5*wall]) {
      cube([8, 8, wall+2], center=true);
    }
    for(hole=camera_holes) {
      translate([raspi_width/2+hole[0], raspi_length/2+hole[1], depth+wall-1]) {
        cylinder(h=wall+2, d=2, $fn=16);
      }
    }
  }
}

bottom();
//color([0.8, 0.2, 0.2]) top();

rotate([0, 180, 0]) translate([2*wall+1, 0, -depth-2*wall]) top();