# Planet Sim

## Installing
Simply put https://github.com/nothings/stb/blob/master/stb_image.h and https://github.com/Tencent/rapidjson/blob/master/include/rapidjson/ into include/
and install GLFW, GLM and GLAD on your system.
You have to be connected to the internet while running `make` because
it uses glad to generate OpenGL Headers Includes.

## TODO
- [x] player movement
- [x] config file parsing
- [x] lighting
- [x] multiple light sources
- [x] lights on the dark side of planets (earth), make ambient light settable
- [x] textures
- [x] non lambertian reflectors
- [x] attentuation
- [ ] ellipsoids instead of spheres
- [ ] delta time in rendering and camera movement (maybe move input processing into update phase?)
- [ ] Skybox
- [x] Tessellation Shader
- [x] orbits
- [x] fps counter
- [x] fix obliquities
- [ ] fix gimbal lock and other bugs in global camera model
- [x] local camera model
- [ ] faster image loading (better library)b
- [x] normal maps (courtesy of http://www.thetenthplanet.de/archives/1180)
