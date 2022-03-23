# Moteur-de-jeu
Pour executer :

Technique 1 :

./InitCMake.sh
./Launch.sh

Technique 2:

mkdir build
cd build
cmake ..
make -j
./launch-moteur.sh

Pour recompiler (Make et non CMake) avant de lancé le programme :
./Launch -m

--- 

Le CMakeFile à été legerement modifié pour permettre l'utilisation de la librairie "stb_image.h"
à fin de pouvoir facilement lire des images PNG ou JPEG (./external/stb/stb_image.h)

---

Commandes :

Souris: Tourne la camera selon Yaw et Pitch.
W : Met en Mode Wireframe pour voir le LOD

Z / S / Q / D / Shift / Controle : déplacé la camera.
UP / DOWN / LEFT / RIGHT : déplacé le soleil

P : met l'animation et les controles en pause.
I : Affiche dans la console des informations sur la caméra. (Position, Yaw Pitch Roll, Position du "Front").
