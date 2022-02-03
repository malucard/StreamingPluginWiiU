# Wii U screen streaming - malucart fork

This is a fork with the goal of rewriting and optimizing it as much as possible! It runs much better, and unlike the original, it works on Breath of the Wild. I think I've already pushed the code to its limits, so I think not much more development can be done until hardware can be implemented (I don't know how to do that).

Please use my fork of the client, which works with my fork of the plugin: https://github.com/malucard/StreamingPluginClient. It features Discord Rich Presence support, but you need to input the name of the game.

Download in the [Releases](https://github.com/malucard/StreamingPluginWiiU/releases) page.

## Still a proof of concept. Don't expect magic.
This is a simple plugin that streams the content of the Gamepad or TV screen to a PC. Frame rate is not exactly incredible
You can adjust various settings via the config menu (Press **L, DPAD DOWN and MINUS** on your Wii U Gamepad whenever using the home menu is allowed). It adjusts them dynamically, but you can configure the limits, and please try to lower the settings if it's not running well.

But general notes:
- This is still a PoC.
- Encoding is done in software, not hardware. Performance is not that good.
- Audio is not streamed at all. This might never be done.
- Somewhat unstable.

## Usage
Simply load the plugin with the plugin loader, after that start the [StreamingPluginClient](https://github.com/malucard/StreamingPluginClient). The client requires Java. Double click on the `.jar` and enter the IP address of your Wii U console.

If you don't know the IP of your Wii U, you can start for example [ftpiiu](https://github.com/dimok789/ftpiiu) which shows the IP when running.

## Wii U Plugin System
This is a plugin for the [Wii U Plugin System (WUPS)](https://github.com/Maschell/WiiUPluginSystem/). To be able to use this plugin you have to place the resulting `.mod` file into the following folder:

```
sd:/wiiu/plugins
```

When the file is placed on the SDCard you can load it with [plugin loader](https://github.com/Maschell/WiiUPluginSystem/).

### Building using the Dockerfile
This way you don't need anything installed on your host system. Install docker (for example: sudo pacman -S docker) and run docker build as below, and then docker run in order to build the module file. You'll probably need to run these with sudo.

```
# Build docker image (only needed once)
docker build . -t screenstreamer-builder

# make (this one is also included in the file 'build.sh' for convenience)
docker run -it --rm -v ${PWD}:/project screenstreamer-builder make

# make clean
docker run -it --rm -v ${PWD}:/project screenstreamer-builder make clean
```
