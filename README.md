# Tetris en C++

## Features

Notre tetris supporte les fonctionnalités suivantes :

- Ghost Piece

- Hard Drop

- Réserve et affichage de la prochaine pièce

- Thèmes pour les pièces

- Musique audio réglable

- Mode multijoueur contre une IA

- score, level

## Compilation

### Pré requis

Il faudra installer au préalable un compilateur C++, sdl2 ainsi que les librairire mixer, image et ttf de la sdl2 : 

```shell
sudo apt install libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl2-mixer-2.0-0 libsdl2-ttf-2.0-0
```

## Compilation et exécution

Pour compiler ce projet, il suffit de lancer la commande make dans le dossier principale. Entrez alors la commande ./bin/tetris pour lancer le tetris. 



# Screenshots

![](/home/bastien/Documents/TPS/2020-2021/2.%20semestre%202/Programmation%20avancée/Tetris/.ressources/2021-04-17-12-05-31-image.png)

![](/home/bastien/Documents/TPS/2020-2021/2.%20semestre%202/Programmation%20avancée/Tetris/.ressources/2021-04-17-12-06-14-image.png)

![](/home/bastien/Documents/TPS/2020-2021/2.%20semestre%202/Programmation%20avancée/Tetris/.ressources/2021-04-17-12-09-21-image.png)

TODO :

Ajouts :

- [x] afficher le score et le niveau sur le côté, ainsi que la prochaine pièce

- [ ] nettoyer la fonction loop

- [ ] modifier le constructeur

- [x] faire des couleurs plus chouettes

- [x] afficher les pièces "fantômes"

- [ ] faire une gestion des high mscores

- [x] ajouter la musique

- [ ] ajouter des effets sonores

- [x] faire un menu pause

- [x] faire un menu de lancement

- [x] faire des menus:
  
  - [x] paramètres audio
  - [x] graphiques

- [ ] mettre un random différent (pool de 7 ou 14)

- [x] faire un vrai "hard drop"

- [x] utiliser la fonction draw pour la pièce sur le côté

- [x] (gros ajout) mode multi contre l'IA
