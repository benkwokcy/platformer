Platformer Game

Features:
- import SDL DONE
- get a window DONE
- set loop to 60 fps DONE
- get a player animated DONE
- accept inputs DONE
- player can attack DONE
- player can jump + player/map physics DONE
- sprite drawing offset, object bounding box DONE
- single layered tile world with collisions DONE
- physics system (gravity) DONE
- scrolling camera, level vs screen coordinates DONE
- spawn points DONE
- enemies, path AI, component pattern DONE
- player/enemy physics, attack collisions, hit animations, character death DONE
- healthbar DONE
- door, respawn DONE
- fall to death DONE

- sound effects
    - find some sound effects
    - link in the sdl2 audio library
    - load sounds
    - have things trigger sounds
- menus
    - main menu
    - pause menu
    - level complete menu
- exit door

Thin platforms should be non collidable if you are jumping from below
Camera should smoothly center the player on y axis if you pass out of the screen

Parts of a Game/Engine:
- input DONE
- graphics DONE
- tile DONE
- camera DONE
- physics DONE
- AI
- audio
- GUI

Stack:
Languages: C++
Tools: Make, Tiled
Libraries: SDL2, TinyXML2