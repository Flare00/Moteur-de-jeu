# Moteur-de-jeu
Florentin DENIS
Khélian LARVET

Le CMakeFile a été legerement modifié pour permettre l'utilisation de la librairie "stb_image.h",
afin de pouvoir facilement lire des images PNG ou JPEG (./external/stb/stb_image.h).

-- FONCTIONNEMENT CMAKE  --

--- Technique_1
./InitCMake.sh
./Launch.sh

--- Technique_2
mkdir build
cd build
cmake ..
make -j
./launch-moteur.sh

Pour recompiler (Make et non CMake) avant de lancer le programme :
./Launch -m


-- DERNIERES FONCTIONNALITES AJOUTEES --

> Ajout d'un système de Physique / Collision.
> Ajout d'un système de LOD (HighPoly ; LowPoly ; Impostor[non utlisé]).


-- EXPLICATIONS --

- Pour l'implémentation du système physique, nous avons mis en place un système de gravité basique et un système de gestion de collision
basé sur les bounding box (en l'occurence AABB). Lors d'une collision, nous stoppons l'effet de la gravité sur les éléments concernés.

- Pour le LOD, nous calculons la distance entre la caméra et les différentes coordonnées de la bounding box de l'objet concerné.
Si celle-ci dépasse la distance de vision de la caméra, nous ne l'affichons pas.
Sinon si elle se situe entre la distance maximum et une première borne arbitraire (ici 25.0f) nous affichons un imposteur (si existant). 
Sinon si elle se situe entre la première borne et la deuxième borne, nous affichons le modèle "LowPoly" (si existant).
Sinon nous affichons le modèle "HighPoly" qui correspond au modèle initial.

- (BONUS) Pour afficher un octree, nous pourrions créer un modèle utilisant les coordonnées de l'octree et l'afficher en wireframe.


-- COMMANDES --

Souris : Peut orienter la caméra selon Yaw et Pitch (callback GLFW).
Z / S / Q / D / Shift / Controle : Déplacement de la camera.
UP / DOWN / LEFT / RIGHT : Déplacement du soleil.

W : Met en Mode Wireframe pour voir le LOD.
P : Met l'animation et les contrôles en pause.
I : Affiche dans la console des informations relatives à la caméra. (Position, Yaw Pitch Roll, Position du "Front").
Esc : Pour quitter la simulation.
