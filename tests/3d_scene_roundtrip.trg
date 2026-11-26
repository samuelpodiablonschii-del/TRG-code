// 3D scene persistence regression test
add (3d)

3d box floor x 0 y -2 z 0 w 12 h 1 d 12 color #64748B
3d sphere player x 0 y 1 z 0 size 1 color #8B5CF6
3d rotate player 0 45 0
3d scale player 1.2 0.8 1.1
3d save "scene-roundtrip.trgscene"

3d clear
3d load "scene-roundtrip.trgscene"
3d move player 1 0 0
3d scale player 1 1 1
3d color floor #22C55E

say "TRG_3D_SCENE_ROUNDTRIP_PASSED"
