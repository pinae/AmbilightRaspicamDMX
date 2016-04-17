rotate([0, -90, 0]) {
  difference() {
    union() {
      cube([12, 25, 14]);
      translate([12, 1, 0]) {
        cylinder(d=2, h=14, $fn=16);
      }
    }
    translate([-1, 7, 7]) {
      rotate([0, 90, 0]) {
        translate([0, 0, 5.2]) {
          cylinder(d=5, h=14, $fn=16);
        }
        cylinder(d=8.7, h=5, $fn=16);
      }
    }
  }
}