![Grizzly](logo.png)

# PTK Simple 2D engine C++/OpenGL ES2

- This game engine is open source and forever free. Please buy me a coffee is you like it!
- Send your game name and I will add it in the powered by ptk list.

[![Buy Me A Coffee](https://cdn.buymeacoffee.com/buttons/default-orange.png)](https://www.buymeacoffee.com/jmapp)

PTK is a very simple 2D engine for C++/OpenGL ES2 games.

## Why C++/OpenGL?

Your project will be buildable for Android, PC, Web, and not only iOS.

- Renderer uses [fglm](https://github.com/brackeen/glfm)
- **KGraphic**: Blit super fast, alpha, zoom, rotation.
- **KInput**: Key and touch input.
- **KMusic**: Music player
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
- ANDROID LODERUNNER (macos) https://apps.apple.com/us/app/android-best-retro-lode-runner/id412859612
- ANDROID2 (macos) https://apps.apple.com/us/app/android-2-retro-loderunner/id1347741576
- ANDROKIDS (macos) https://apps.apple.com/us/app/notebook-city-androkids/id1516357274
- BARNEY https://apps.apple.com/us/app/barney-adventures/id502135523
- BARNEY (macos) https://apps.apple.com/us/app/barney/id481853130
- ANDROKIDS2 (macos) https://apps.apple.com/us/app/androkids-2/id1516721499
- MARTY MCBLAST (macos) https://apps.apple.com/us/app/marty-mcblast/id870219886
- METAL-ANGEL https://apps.apple.com/us/app/metal-angel-best-shootem-up/id287261924
- METAL-ANGEL (macos) https://apps.apple.com/us/app/metal-angel-shooter/id6544782954
- ULTRANIUM 3D (macos) https://apps.apple.com/us/app/ultranium3d/id413890023
- ULTRANIUM4 (macos) https://apps.apple.com/us/app/ultranium-4-breakout-shooter/id412207082
- ULTRANIUM5 (macos) https://apps.apple.com/us/app/ultranium-5-crazy-arcade-game/id431295311







### Requirements

- C++11 compiler
- OpenGL ES2
- GLFM

### Build Instructions

1. Clone this repository:
   ```bash
   git clone https://github.com/brizzly/ptk.git
   include /iPTKSources into your xcode project
   ```



