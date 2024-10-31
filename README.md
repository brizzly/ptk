![Grizzly](logo.png)<br>
<img src="logo.png" width="256" height="256">

# PTK 2D engine C++/OpenGL ES2

- This game engine is open source and forever free. Please buy me a coffee is you like it!
- Send your game name and I will add it in the powered by ptk list.

[![Buy Me A Coffee](https://cdn.buymeacoffee.com/buttons/default-orange.png)](https://www.buymeacoffee.com/jmapp)

PTK is a very easy to use 2D engine for C++/OpenGL ES2 games.

## Why C++/OpenGL?

Your project will be buildable for Android, PC, Web, and not only iOS.

- Renderer uses [fglm](https://github.com/brackeen/glfm)
- **KGraphic**: Blit super fast, alpha, zoom, rotation.
- **KInput**: Key and touch input.
- **KMusic**: Music player
- **KSound**: Sound management with OpenAL.

## Getting Started

Example : xcode examples project in: /examples/Simple
- iOS Landscape
- iOS Portrait
- Android Portait

## Usage : 

- Load and display a sprite :
```bash
	test = new KGraphic();
	test->loadPicture("box.png");
	test->srcX = 0;
	test->srcY = 0;
	test->destX = 0;
	test->destY = 0;
	test->sizeW = 640;
	test->sizeH = 480;
	test->angle = 0;
	test->zoom = 1.0;
	test->blend = 1.0;
	test->render();
```

- Load and play a sound or music :
```bash
	sound1 = new KSound();
	sound1->loadSample(KMiscTools::makeFilePath("boing2.caf"));
	sound1->setVolume(50);

	music1 = new KMusic();
	music1->playMusic(KMiscTools::makeFilePath("menu.mp3"));
```


### Requirements

- C++11 compiler / xCode or Android Studio
- OpenGL ES2
- GLFM
- OpenAL

### Build Instructions

1. Clone this repository:
   ```bash
   git clone https://github.com/brizzly/ptk.git
   ```
2. include /iPTKSources into your project   


### Games prowered by PTK

# iPhone
- ULTRANIUM https://apps.apple.com/app/space-breakout-ultranium/id6736717749
- GRIZZLY https://apps.apple.com/app/grizzly-adventures-crazy-bear-platformer/id357410945
- BARNEY https://apps.apple.com/app/barney-adventures/id502135523
- METAL-ANGEL https://apps.apple.com/app/metal-angel-best-shootem-up/id287261924

# Android
coming-soon

# macOS
- GRIZZLY (macos) https://apps.apple.com/app/grizzly-retro-platformer/id413938360?mt=12
- ANDROID LODERUNNER (macos) https://apps.apple.com/app/android-best-retro-lode-runner/id412859612
- ANDROID2 (macos) https://apps.apple.com/app/android-2-retro-loderunner/id1347741576
- ANDROKIDS (macos) https://apps.apple.com/app/notebook-city-androkids/id1516357274
- BARNEY (macos) https://apps.apple.com/app/barney/id481853130
- ANDROKIDS2 (macos) https://apps.apple.com/app/androkids-2/id1516721499
- MARTY MCBLAST (macos) https://apps.apple.com/app/marty-mcblast/id870219886
- METAL-ANGEL (macos) https://apps.apple.com/app/metal-angel-shooter/id6544782954
- ULTRANIUM 3D (macos) https://apps.apple.com/app/ultranium3d/id413890023
- ULTRANIUM4 (macos) https://apps.apple.com/app/ultranium-4-breakout-shooter/id412207082
- ULTRANIUM5 (macos) https://apps.apple.com/app/ultranium-5-crazy-arcade-game/id431295311
- HALLOWEEN 3D (macos) https://apps.apple.com/app/halloween-3d/id1276055212

# PC
- GRIZZLY (pc) https://brizzlyy.itch.io/grizzly-adventures
- ANDROID LODERUNNER (pc) https://brizzlyy.itch.io/android-lode-runner
- ANDROID2 (pc) https://brizzlyy.itch.io/androkids2
- ANDROKIDS (pc) https://brizzlyy.itch.io/androkids
- BARNEY (pc) https://brizzlyy.itch.io/barney-adventures
- MARTY MCBLAST (pc) https://brizzlyy.itch.io/marty-mcblast
- METAL-ANGEL (pc) https://brizzlyy.itch.io/metal-angel-shoot-em-up
- ULTRANIUM4 (pc) https://brizzlyy.itch.io/ultranium-4
- ULTRANIUM5 (pc) https://brizzlyy.itch.io/ultranium-5


