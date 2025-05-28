# Hanabi
## Règles du jeu
Le Hanabi est un jeu de cartes coopératif. Les cartes utilisées sont de 5 couleurs : rouge, jaune, vert, bleu et blanc; et elles représentent les chiffres de 1 à 5.
Pour chaque couleur, il y a 3 cartes 1, 2 cartes 2, 2 cartes 3, 2 cartes 4 et une seule carte 5.

Le but du jeu pour tous les joueurs est de, pour chaque couleur, poser les cartes de 1 à 5 dans l'ordre, sans doublons.

Ce qui rend le jeu intéressant, c'est que les joueurs ne peuvent pas voir leur propre main. Seuls les autres joueurs le peuvent. L'échange d'informations est donc limité à l'utilisation d'indices, partagés entre les joueurs.

À son tour, un joueur a 3 possibilités :
- Jouer une carte et en repiocher une.
- Défausser une carte, en repiocher une, et remettre un indice dans la réserve d'indices.
- Utiliser un indice pour donner une information à un joueur sur les cartes dans sa main. L'information peut être :
  - Une couleur, auquel cas toutes les cartes de la couleur doivent être indiquées
  - Un chiffre, auquel cas toutes les cartes représentant le chiffre doivent être indiquées

Le jeu est terminé lorsque toutes les cartes ont été posées, ou bien lorsque les joueurs ont effectué 3 erreurs, ou bien lorsque la personne ayant pioché la dernière carte a joué une fois de plus.

## Ambitions du jeu
Le but était de coder le jeu de Hanabi afin de pouvoir y jouer sur plusieurs cartes en réseau à l'aide de LWIP. Les joueurs devaient pouvoir créer une partie d'un côté, et rejoindre une partie de l'autre.
De cette façon le serveur aurait pu se trouver sur n'importe quelle carte. Le choix devait se faire sur un écran dédié avant le jeu.

## Exécution
Le jeu devait utiliser les tâches de FreeRTOS, communicant avec des queues. Le jeu devait être codé comme une machine à état, et chaque tâche devait avoir un comportement différent en fonction de l'état.

L'état est codé dans une variable globale.

Les tâches implémentées sont :
- Une tâche d'affichage, afin de ne pas avoir à utiliser un mutex, et pour centrer les fonctions d'affichage qui prennent de la place visuellement.
- Une tâche gérant les appuis sur l'écran pour garder la philosophie consistente avec l'affichage.
- Une tâche gérant le changement d'état.

Ces tâches communiquent à l'aide de queues destinées à chaque tâche.

Les états et les messages à communiquer par queue sont codées à l'aide d'enums qui permettent de rendre le code beaucoup plus lisible.

Par manque de temps, seul l'écran titre et l'écran de choix de créer ou rejoindre une partie ont été créés. Le temps manquant a été utilisé pour tenter de faire fonctionner LWIP, en vain.
Le jeu n'ayant aucun intérêt sans multijoueur, le développement s'est arrêté là.

## Schéma de fonctionnement: 
