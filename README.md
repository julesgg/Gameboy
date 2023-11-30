# Gameboy Project

Fabrication d'une gameboy, codage d'un jeu de chasse et modélisation d'un moule pour le boitier

***Matthieu Mombert, Jules Gueguen***

## Matériel requis

2 x ESP32
2 x Écrans OLED
Claviers matriciels
Câbles et connexions

## Bibliothèques Requises

Wire.h
Adafruit_GFX.h, Adafruit_SSD1306.h
Keypad.h
WiFi.h, WiFiUdp.h


## But du jeu

"Wildlife Rescue" est un jeu de chasse en 1 contre 1 où les deux joueurs s'affrontent chacun sur leur gameboy qui sont connectées en réseau. Des animaux et des humains défilent devant les deux joueurs munis d'un pistolet, leur but est de tirer sur les humains et d'éviter les animaux. Lorsqu'un joueur touche un humain, il gagne 1 point et lorsqu'il touche un animal il perd un point. Le premier à 10 points gagne la partie.

## Caractéristiques principales 

-jeu de chasse
-1 contre 1 en réseau 
-1 map interactive
-incrémentation des scores
-10 points pour gagner la partie

## Description du système

Ce jeu a été codé sur Visual Studio Code, la première étape a été de coder un jeu fonctionnel sur une seule gameboy. Nous avons codé pas à pas en rajoutant les fonctionnalités une à une. Lorsque le jeu fonctionnait sur une gameboy, nous nous sommes occupés de connecter le jeu en réseau afin de pouvoir jouer sur deux gameboy en simultané.

Le jeu a été implémenté sur deux gameboy créées à partir d'une ESP32 reliée à une autre PCB composée d'un écran OLED 128x64 pixels et des boutons de la gameboy.

## Comment jouer 

-Allumer les deux gameboy et les connecter en wifi
-Attendre quelques secondes qu'un message affiche que la connection a été effectuée
-Un menu s'affiche pour le choix des joueurs "joueur 1" ou "joueur 2"
-Commencer la partie en pressant le bouton "Start" en bas à droite
-Le joueur 1 est positionné en bas à gauche de l'écran et le joueur 2 est positionné en bas à droite
-Les joueurs tirent avec le bouton A situé le plus à droite de la gameboy
-Lorsqu'un joueur touche une cible, son score augmente ou diminue selon la cible atteinte au niveau du compteur situé dans le coin au dessus du joueur
-Lorsqu'un joueur atteint le socre de 10 points, il gagne et la partie s'arrête
-Ils peuvent relancer une partie en pressant "Start" à nouveau

## Comment le jeu a été créé

Le code est composé d'un seul fichier réunissant le rôle de client et de serveur
Le fichier contient les différentes classes et éléments permettant au jeu de fonctionner. Il permet de créer une connection wifi et procède aux échanges de données entre les gameboy. Nous avons effectué quelques modifications sur le code après avoir tourné la vidéo, le code final affiche une interface qui demande aux joueurs de choisir entre "joueur 1" et "joueur 2", lorsque les joueurs ont fait un choix le jeu commence et fonctionne en réseau.
