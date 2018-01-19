# Planet Sim

## Installing
Simply put https://github.com/nothings/stb/blob/master/stb_image.h and https://github.com/Tencent/rapidjson/blob/master/include/rapidjson/ into include/
and install GLFW and GLM on your system.
You have to be connected to the internet while running `make` because
it uses glad to include OpenGL Headers.

## TODO
- [x] player movement
- [x] config file parsing
- [x] lighting
- [ ] multiple light sources
- [x] lights on the dark side of planets (earth), make ambient light settable
- [x] textures
- [x] non lambertian reflectors
- [ ] attentuation
- [ ] ellipsoids instead of spheres
- [ ] delta time in rendering and camera movement (maybe move input processing into update phase?)
- [ ] Skybox
- [x] Tessellation Shader
- [ ] orbits
- [ ] fps counter
- [ ] fix obliquities
- [ ] local camera model