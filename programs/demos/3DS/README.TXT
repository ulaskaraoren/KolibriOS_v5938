View3ds 0.065 - tiny viewer to .3ds and .asc files.

What's new?
1. Asc files support.

Buttons description:
1. rotary: choosing rotary axle: x, y, x+y.
2. shd. model: choosing shading model: flat, grd (smooth), env (spherical
   environment mapping, bump (bump mapping), tex (texture mapping),
   pos (position shading depend), dots (app draws only points - nodes of object),
   txgrd (texture mapping + smooth shading),  2tex (texture mapping + spherical
   environment mapping), bmap (bump + texture mapping),  cenv (cubic environment
   mapping), grdl (Gouraud lines - edges only).
3. speed: idle, full.
4,5. zoom in, out: no comment.
6. catmull: on( use z buffer ( z coordinate interpolation), off( depth sorting, painters
   alghoritm).Txgrd, 2tex and bumptex models only with catmull  = on.
7. culling: backface culling on/ off.
8. rand. light: Randomize 3 unlinear lights( so called Phong's illumination).
9. Blur: blur N times; N=0,1,2,3,4,5
10.11,12,13. loseless operations (rotary 90, 180 degrees).
12. emboss: Do emboss effect( flat bumps ), use blur to do edges more deep.
       carefull with emboss + fire - it looks annoying.
13. fire: do motion blur ( looks like fire ).
14. move: changes meaning x,y,z +/- buttons  ->  obj: moving object, camr: moving camera, wave: x,y +/- increase,
    decrease wave effect frequency and amplitude.
15. generate: Generates some objects: node, Thorn Crown, heart...
16. bumps: random, according to texture.
17. bumps deep -> create bumps deeper or lighter.
18. re-map tex -> re-map texture and bump map coordinates, to change spherical mapping 
    around axle use 'xchg' and 'mirror' buttons, then press 're-map tex' button.
19. bright + -> increase picture brightness.
20. bright - -> decrease picture brightness.
21. wav effect -> do effect based sine function.

			 Maciej Guba 		 Feb 2015
