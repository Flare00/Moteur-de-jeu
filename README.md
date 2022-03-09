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

Z / Q / S / D : Tourner la camera selon Yaw et Pitch.
Haut / Bas / Gauche / Droite / Shift / Controle : déplacé la camera.
+ / - : Augmente/Réduit la résolution du mesh de terrain.
P : met l'animation et les controles en pause.
I : Affiche dans la console des informations sur la caméra. (Position, Yaw Pitch Roll, Position du "Front").
C : Mode orbital (Non Fonctionnel)

Page UP / Page DOWN : Augmenté / Réduire la vitesse de rotation du terrain.
Touche R : arreter / redemarrer la rotation du terrain.