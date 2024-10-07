![Grizzly](logo.png)
# PTK Simple 2D engine C++/OpenGL ES2

<a href="https://www.buymeacoffee.com/jmapp" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" height="41" width="174"></a>

PTK is a very simple 2D engine for C++/OpenGL ES2 games.

Why C++/OpenGL ?
Your project will be buildable for Android and PC, and Web not only iOS.

Renderer uses fglm : https://github.com/brackeen/glfm

KGraphic : Blit super fast, alpha, zoom, rotation.
KInput : multi touch 
KSound : play sound effets
KMusic : play audio tracks
KWeb : open URL and makes webservice calls

Example : xcode examples project in: /examples/Simple

Usage : 

KGraphic * test;
test = new KGraphic();
test->loadPicture("box.png");
testGraphic7->srcX = 0;
testGraphic7->srcY = 0;
testGraphic7->destX = 0;
testGraphic7->destY = 0;
testGraphic7->sizeW = 640;
testGraphic7->sizeH = 480;
testGraphic7->angle = 0;
testGraphic7->zoom = 1.0;
testGraphic7->blend = 1.0;
testGraphic7->render();


Games powered by PTK :

GRIZZLY (macos) https://apps.apple.com/us/app/grizzly-retro-platformer/id413938360?mt=12
GRIZZLY https://apps.apple.com/us/app/grizzly-adventures-crazy-bear-platformer/id357410945



