# Moteur-de-jeu
Lien Github : https://github.com/Flare00/Moteur-de-jeu  
Florentin DENIS  
Khélian LARVET  

## FONCTIONNEMENT CMAKE

### Technique_1  
./InitCMake.sh  
./Build.sh  
./Launch.sh  

### Technique_2  
mkdir build  
cmake . -B"build"  
cmake --build "build" --config Release  
./build/launch-moteur.sh  

## FONCTIONNALITES

- Système Physique pyBullet 
- Gestion des lampes
- Gestion de scène
- Tesselation de terrain
- Graphe de scène
- Shadow Mapping
- Lecture de Fichier OBJ
- Gestion des inputs
- LOD, Frustum Culling

## COMMANDES --  

Contrôles globaux :
- Z / S / Q / D / Shift / Controle : Déplacement de la camera.  
- W : Met en Mode Wireframe pour voir le LOD.  
- P : Met l'animation et les contrôles en pause.
- Enter : Changer de Scène  
- Esc : Pour quitter la simulation.  
- Souris : Peut orienter la caméra selon Yaw et Pitch (callback GLFW).  

SceneLight :
- T : Allume/Eteint une des deux lampes.

SceneProjectile : 
- UP / DOWN / LEFT / RIGHT : Déplacement du canon
- K : Tire avec le canon 
