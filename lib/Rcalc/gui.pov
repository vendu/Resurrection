#include "colors.inc"
#include "metals.inc"

camera {
	location <0, 0, -12>
	look_at <0, 0, 0>
}

light_source {
	<0, 0, -32>
	color White
	spotlight
	point_at <0, 0, 0>
	adaptive 1
}

#declare box_finish=finish { ambient 0.5 diffuse 0.5 }
#declare text_finish=finish { ambient 0.4 diffuse 0.4 }

union {
	box {
		<-7, -6, 2>,
		<-3, 4, 4>
		pigment {
			granite
			color_map {
				[0 Black]
				[1.0 Gray50]
			}
		}
		finish {
		       box_finish
		}
	}
	cylinder {
		<-7, 4, 2>,
		<-3, 4, 2>,
		0.1
		pigment {
			granite
			color_map {
				[0 Green]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	text {
		ttf "lcd.ttf" "hex" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-5.75, 3, 0.5>
		finish {
		       text_finish
		}
	}
	cylinder {
		<-5, 4, 2>,
		<-5, 3, 2>,
		0.1
		pigment {
			granite
			color_map {
				[0 Green]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	text {
		ttf "lcd.ttf" "shl" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-3.75, 2.825, 0>
		finish {
		       text_finish
		}
	}
	cylinder {
		<-7, 3, 2>,
		<-3, 3, 2>,
		0.1
		pigment {
			granite
			color_map {
				[0 Green]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	text {
		ttf "lcd.ttf" "dec" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-5.75, 2, 0>
		finish {
		       text_finish
		}
	}
	cylinder {
		<-5, 3, 2>,
		<-5, 2, 2>,
		0.1
		pigment {
			granite
			color_map {
				[0 Green]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	text {
		ttf "lcd.ttf" "shra" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-4, 2, 0>
		finish {
		       text_finish
		}
	}
	cylinder {
		<-7, 2, 2>,
		<-3, 2, 2>,
		0.1
		pigment {
			granite
			color_map {
				[0 Green]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	text {
		ttf "lcd.ttf" "oct" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-5.75, 1.125, 0>
		finish {
		       text_finish
		}
	}
	cylinder {
		<-5, 2, 2>,
		<-5, 1, 2>,
		0.1
		pigment {
			granite
			color_map {
				[0 Green]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	text {
		ttf "lcd.ttf" "shrl" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-4, 1.125, 0>
		finish {
		       text_finish
		}
	}
	cylinder {
		<-7, 1, 2>,
		<-3, 1, 2>,
		0.1
		pigment {
			granite
			color_map {
				[0 Green]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	text {
		ttf "lcd.ttf" "vm" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-5.75, -0.75, 0>
		finish {
		       text_finish
		}
	}
	text {
		ttf "lcd.ttf" "pit" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-5.75, -1.575, 0>
		finish {
		       text_finish
		}
	}
	text {
		ttf "lcd.ttf" "dma" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-5.75, -2.4, 0>
		finish {
		       text_finish
		}
	}
	text {
		ttf "lcd.ttf" "io" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-5.75, -3.225, 0>
		finish {
		       text_finish
		}
	}
	cylinder {
		<-7, -6, 2>,
		<-7, 4, 2>
		0.1
		pigment {
			granite
			color_map {
				[0 Green]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	cylinder {
		<-3, 4, 2>,
		<-3, -6, 2>
		0.1
		pigment {
			granite
			color_map {
				[0 Green]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	cylinder {
		<-7, -6, 2>,
		<-3, -6, 2>
		0.1
		pigment {
			granite
			color_map {
				[0 Green]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
}

union {
	box {
		<-2, -6, 2>,
		<2, 4, 4>
		pigment {
			granite
			color_map {
				[0 Black]
				[1.0 Gray50]
			}
		}
		finish {
		       box_finish
		}
	}
	cylinder {
		<-2, 4, 2>,
		<2, 4, 2>,
		0.1
		pigment {
			granite
			color_map {
				[0 Yellow]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	text {
		ttf "lcd.ttf" "hw" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-1.5, 2.825, 0>
		finish {
		       text_finish
		}
	}
	cylinder {
		<-2, 3, 2>,
		<2, 3, 2>,
		0.1
		pigment {
			granite
			color_map {
				[0 Yellow]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	text {
		ttf "lcd.ttf" "ps2" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-1.5, 1.825, 0>
		finish {
		       text_finish
		}
	}
	text {
		ttf "lcd.ttf" "ac97" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-1.5, 1, 0>
		finish {
		       text_finish
		}
	}
	text {
		ttf "lcd.ttf" "vga" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-1.5, 0.175, 0>
		finish {
		       text_finish
		}
	}
	text {
		ttf "lcd.ttf" "vbe" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-1.5, -0.75, 0>
		finish {
		       text_finish
		}
	}
	text {
		ttf "lcd.ttf" "fd" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-1.5, -1.575, 0>
		finish {
		       text_finish
		}
	}
	text {
		ttf "lcd.ttf" "ata" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <-1.5, -2.4, 0>
		finish {
		       text_finish
		}
	}
	cylinder {
		<-2, 4, 2>,
		<-2, -6, 2>
		0.1
		pigment {
			granite
			color_map {
				[0 Yellow]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	cylinder {
		<-2, -6, 2>,
		<2, -6, 2>
		0.1
		pigment {
			granite
			color_map {
				[0 Yellow]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	cylinder {
		<2, -6, 2>,
		<2, 4, 2>
		0.1
		pigment {
			granite
			color_map {
				[0 Yellow]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
}

union {
	box {
		<3, -6, 2>,
		<7, 4, 4>
		pigment {
			granite
			color_map {
				[0 Black]
				[1.0 Gray50]
			}
		}
		finish {
		       box_finish
		}
	}
	cylinder {
		<3, 4, 2>,
		<7, 4, 2>,
		0.1
		pigment {
			granite
			color_map {
				[0 Green]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	text {
		ttf "lcd.ttf" "kern" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <2.825, 2.825, 0>
		finish {
		       text_finish
		}
	}
	cylinder {
		<3, 3, 2>,
		<7, 3, 2>,
		0.1
		pigment {
			granite
			color_map {
				[0 Green]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	text {
		ttf "lcd.ttf" "unit" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <2.825, 1.825, 0>
		finish {
		       text_finish
		}
	}
	text {
		ttf "lcd.ttf" "sig" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <2.825, 1, 0>
		finish {
		       text_finish
		}
	}
	text {
		ttf "lcd.ttf" "mem" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <2.825, 0.175, 0>
		finish {
		       text_finish
		}
	}
	text {
		ttf "lcd.ttf" "buf" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <2.825, -0.75, 0>
		finish {
		       text_finish
		}
	}
	text {
		ttf "lcd.ttf" "dev" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <2.825, -1.575, 0>
		finish {
		       text_finish
		}
	}
	text {
		ttf "lcd.ttf" "sys" 0.0625, 0
		pigment { OldGold }
		scale <0.5, 0.5, 0.5>
		translate <2.825, -2.4, 0>
		finish {
		       text_finish
		}
	}
	cylinder {
		<3, 4, 2>,
		<3, -6, 2>
		0.1
		pigment {
			granite
			color_map {
				[0 Green]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	cylinder {
		<3, -6, 2>,
		<7, -6, 2>
		0.1
		pigment {
			granite
			color_map {
				[0 Green]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
	cylinder {
		<7, -6, 2>,
		<7, 4, 2>
		0.1
		pigment {
			granite
			color_map {
				[0 Green]
				[1.0 Black]
			}
		}
		finish {
			ambient 0.5
			diffuse 0.5
		}
	}
}

