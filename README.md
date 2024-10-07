![Grizzly](logo.png)

# PTK Simple 2D engine C++/OpenGL ES2

[![Buy Me A Coffee](https://cdn.buymeacoffee.com/buttons/default-orange.png)](https://www.buymeacoffee.com/jmapp)

PTK is a very simple 2D engine for C++/OpenGL ES2 games.

## Why C++/OpenGL?

Your project will be buildable for Android, PC, Web, and not only iOS.

- Renderer uses [fglm](https://github.com/brackeen/glfm)
- **KGraphic**: Blit super fast, alpha, zoom, rotation.
- **KInput**: Key and touch input.
- **KTime**: FPS synchronization, timers.
- **KSound**: Sound management with OpenAL.

## Getting Started

Example : xcode examples project in: /examples/Simple

Usage : 
```bash
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
```

### Games prowered by PTK

- GRIZZLY (macos) https://apps.apple.com/us/app/grizzly-retro-platformer/id413938360?mt=12
- GRIZZLY https://apps.apple.com/us/app/grizzly-adventures-crazy-bear-platformer/id357410945


### Requirements

- C++11 compiler
- OpenGL ES2
- GLFM

### Build Instructions

1. Clone this repository:
   ```bash
   git clone https://github.com/brizzly/ptk.git
   include /iPTKSources into your xcode project
	```bash



