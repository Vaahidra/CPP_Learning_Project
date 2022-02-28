# Gestion mémoire

## Analyse de la gestion des avions

La création des avions est aujourd'hui gérée par les fonctions `TowerSimulation::create_aircraft` et `TowerSimulation::create_random_aircraft`.
Chaque avion créé est ensuite placé dans les files `GL::display_queue` et `GL::move_queue`.

Si à un moment quelconque du programme, vous souhaitiez accéder à l'avion ayant le numéro de vol "AF1250", que devriez-vous faire ?
> chercher dans la move_queue ou dans la display_queue

---

## Objectif 1 - Référencement des avions

### A - Choisir l'architecture

Pour trouver un avion particulier dans le programme, ce serait pratique d'avoir une classe qui référence tous les avions et qui peut donc nous renvoyer celui qui nous intéresse.

Vous avez 2 choix possibles :
- créer une nouvelle classe, `AircraftManager`, qui assumera ce rôle,
- donner ce rôle à une classe existante.

Réfléchissez aux pour et contre de chacune de ces options.
> AircraftManager -> une classe = une responssabilité mais force à trainer un objet de plus dans tous le programme  
> Dans une classe existante -> on a déjà des classes présente dans tous le programme mais on regroupe plusieurs responssabilitées dans une seul classe

Pour le restant de l'exercice, vous partirez sur le premier choix.

### B - Déterminer le propriétaire de chaque avion

Vous allez introduire une nouvelle liste de références sur les avions du programme.
Il serait donc bon de savoir qui est censé détruire les avions du programme, afin de déterminer comment vous allez pouvoir mettre à jour votre gestionnaire d'avions lorsque l'un d'entre eux disparaît.

Répondez aux questions suivantes :
1. Qui est responsable de détruire les avions du programme ? (si vous ne trouvez pas, faites/continuez la question 4 dans TASK_0)
> opengl_interface.cpp dans la move_queue

2. Quelles autres structures contiennent une référence sur un avion au moment où il doit être détruit ?
> GL::display_queue et reserved_terminals

3. Comment fait-on pour supprimer la référence sur un avion qui va être détruit dans ces structures ?
> On doit faire une recherche et supprimer manuellement

4. Pourquoi n'est-il pas très judicieux d'essayer d'appliquer la même chose pour votre `AircraftManager` ?
> Parce qu'on veut que AircraftManager controlle la durée de vie des avions

Pour simplifier le problème, vous allez déplacer l'ownership des avions dans la classe `AircraftManager`.
Vous allez également faire en sorte que ce soit cette classe qui s'occupe de déplacer les avions, et non plus la fonction `timer`.

### C - C'est parti !

Ajoutez un attribut `aircrafts` dans le gestionnaire d'avions.
Choisissez un type qui met bien en avant le fait que `AircraftManager` est propriétaire des avions.
> std::unique_ptr<Aircraft> 

Ajoutez un nouvel attribut `aircraft_manager` dans la classe `TowerSimulation`.

Faites ce qu'il faut pour que le `AircraftManager` puisse appartenir à la liste `move_queue`.
Ajoutez la fonction appropriée dans `AircraftManager` pour demander de bouger (`move`) les avions.
Supprimez les ajouts d'`Aircraft` dans la `move_queue`. En effet, ce n'est plus `timer` qui est responsable de déplacer les avions mais l'`AircraftManager`.

> On fait donc en sorte que `AircraftManager` soit un DynamicObject pour qu'il puisse bouger par la suite les avions.
> On peut supprimer le retour de move de DynamicObject puisqu'elle n'est utile que pour les `Aircraft`
> qui auront donc une autre fonction de déplacement (la même qu'avant mais sans dépendre de DynamicObject)

Enfin, faites ce qu'il faut pour que `create_aircraft` donne l'avion qu'elle crée au gestionnaire.
Testez que le programme fonctionne toujours.

---

## Objectif 2 - Usine à avions

### A - Création d'une factory

La création des avions est faite à partir des composants suivants :
- `create_aircraft`
- `create_random_aircraft`
- `airlines`
- `aircraft_types`.

Pour éviter l'usage de variables globales, vous allez créer une classe `AircraftFactory` dont le rôle est de créer des avions.

Définissez cette classe, instanciez-là en tant que membre de `TowerSimulation` et refactorisez-le code pour l'utiliser.
Vous devriez constater que le programme crashe.

En effet, pour que la factory fonctionne, il faut que le `MediaPath` (avec la fonction `MediaPath::initialize`) et que `glut` (avec la fonction `init_gl()`) aient été initialisés.
Comme ces appels sont faits depuis le corps du constructeur de `TowerSimulation`, ils sont actuellement exécutés après la construction de la factory.
Afin de faire en sorte que les appels aient lieu dans le bon ordre, vous allez créer une structure `ContextInitializer` dans le fichier `tower_sim.hpp`.
Vous lui ajouterez un constructeur dont le rôle sera d'appeler les fonctions d'initialisation de `MediaPath`, `glut` et `srand`.

Vous pouvez maintenant ajoutez un attribut `context_initializer` de type `ContextInitializer` dans la classe `TowerSimulation`.
A quelle ligne faut-il définir `context_initializer` dans `TowerSimulation` pour s'assurer que le constructeur de `context_initializer` est appelé avant celui de `factory` ?

Refactorisez le restant du code pour utiliser votre factory.
Vous devriez du coup pouvoir supprimer les variables globales `airlines` et `aircraft_types`.

### B - Conflits

Il est rare, mais possible, que deux avions soient créés avec le même numéro de vol.
Ajoutez un conteneur dans votre classe `AircraftFactory` contenant tous les numéros de vol déjà utilisés.
Faites maintenant en sorte qu'il ne soit plus possible de créer deux fois un avion avec le même numéro de vol.

### C - Data-driven AircraftType (optionnel)

On aimerait pouvoir charger les paramètres des avions depuis un fichier.

Définissez un format de fichier qui contiendrait les informations de chaque `AircraftType` disponible dans le programme.\
Ajoutez une fonction `AircraftFactory::LoadTypes(const MediaPath&)` permettant de charger ce fichier.
Les anciens `AircraftTypes` sont supprimés.

Modifiez ensuite le `main`, afin de permettre à l'utilisateur de passer le chemin de ce fichier via les paramètres du programme.
S'il ne le fait pas, on utilise la liste de type par défaut.

Si vous voulez de nouveaux sprites, vous pouvez en trouver sur [cette page](http://www.as-st.com/ttd/planes/planes.html)
(un peu de retouche par GIMP est necessaire)

---

## Objectif 3 - Pool de textures (optionnel)

Pour le moment, chacun des `AircraftType` contient et charge ses propres sprites.
On pourrait néanmoins avoir différents `AircraftType` qui utilisent les mêmes sprites.
Ils seraient donc chargés plusieurs fois depuis le disque pour rien.

Pour rendre le programme un peu plus performant, implémentez une classe `TexturePool` qui s'occupe de charger, stocker et fournir les textures.
Pour exprimer correctement ce type d'ownership, vous devez utiliser le smart-pointer `std::shared_ptr`.

Commencez par aller sur la documentation de `std::shared_ptr`.
Pouvez-vous expliquer comment le compilateur arrive à déterminer à quel moment l'objet n'a plus aucun owner, afin de le détruire ?

Listez les classes qui ont besoin de `TexturePool`.
Sachant que vous n'aurez qu'une seule instance de `TexturePool` dans votre programme, quelle classe devra assumer l'ownership de cet objet ?\
Instanciez `TexturePool` au bon endroit et refactorisez le code afin que tous les chargements de textures utilisent ce nouvel objet.
Assurez-vous que votre implémentation ne génère pas des fuites de mémoire au moment de sa destruction.
