# Se familiariser avec l'existant

## A- Exécution

Compilez et lancez le programme.

Allez dans le fichier `tower_sim.cpp` et recherchez la fonction responsable de gérer les inputs du programme.
> create_keystrokes()

Sur quelle touche faut-il appuyer pour ajouter un avion ?
> c

Comment faire pour quitter le programme ?
> press q or x

A quoi sert la touche 'F' ?
> Enable/Disable fullscreen

Ajoutez un avion à la simulation et attendez.
Que est le comportement de l'avion ?
> Fly > Land > Go to Terminal > Fly off

Quelles informations s'affichent dans la console ?
> When the aircraft land, lift off and when the terminal start/stop servicing an aircraft

Ajoutez maintenant quatre avions d'un coup dans la simulation.
Que fait chacun des avions ?
> They fly until they can land (only 3 aircrafts can land at the same time)

## B- Analyse du code

Listez les classes du programme à la racine du dossier src/.
Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.
> Aircraft : C'est un avion  
> Airport : C'est l'aéroport  
> AirportType : Fournit la position des éléments de l'aéroport  
> Terminal : Prend un aircraft et s'occupe de lui pendant un certain temps  
> Tower : Donne les instructions aux avions.  
> TowerSimulation : Gère les actions utilisateur  
> Waypoint : Représente un point en 3D ayant une connaissance de s'il est au sol ou non


Pour les classes `Tower`, `Aircaft`, `Airport` et `Terminal`, listez leurs fonctions-membre publiques et expliquez précisément à quoi elles servent.
Réalisez ensuite un schéma présentant comment ces différentes classes intéragissent ensemble.
> Tower :  
> * get_instructions(Aircraft&) : Génère des instructions pour l'avion
> * arrived_at_terminal(Aircraft&) : Récupère le terminal sur lequel doit se poser l'avions et ensuite lui donne l'avion
> 
> Aircraft : 
> * get_flight_num() : Renvoi le numéro de vol de cet avion
> * distance_to(): Renvoi la distance entre cet avion et un point
> * display() : Affiche l'avion
> * move() : Met à jours la position de l'avion
> 
> Airport :
> * get_tower() : Renvoi la tour de control
> * display() : Affiche l'aeroport
> * move() : fait bouger tous les terminaux
> 
> Terminal : 
> * in_use() : Renvoi true s'il est en cours d'utilisation
> * is_servicing() : Renvoi true tant qu'il n'a pas fini de s'occuper de l'avion
> * assign_craft() : Assigne un nouvel avions
> * start_service() : Affiche un message et commence à s'occuper de l'avion
> * finish_service() : Affiche un message et supprime sa ref sur l'avion
> * move() : Ajoute 1 au compteur de traitement de l'avion

Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ?
> On commence dans Aircraft::move() qui appel 
> Tower::get_instruction() si il n'a plus de waypoints
> Ensuite appel essaie de reserver un terminal avec Airport::reserve_terminal()
> qui appel Terminal::assign_craft() si c'est réussi et met à jour le chemin de l'avion  
> sinon assigne un nouveau chemin aérien à l'avion
> Dans le cas où l'avion est dans un terminal si l'avion a fini d'etre servit,
> retire l'avion/terminal des reservations et fait repartir l'avion

Quel conteneur de la librairie standard a été choisi pour représenter le chemin ?
Expliquez les intérêts de ce choix.
> une queue parce qu'on veut ajouter des points à la fin et pouvoir les récupérer à partir du début

## C- Bidouillons !

1) Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion.
Le Concorde est censé pouvoir voler plus vite que les autres avions.
Modifiez le programme pour tenir compte de cela.
> AircraftTypes (l.34) changer le 2eme paramètre de .05f à .2f

2) Identifiez quelle variable contrôle le framerate de la simulation.
Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur.
Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ?\
Ajoutez une nouvelle fonctionnalité au programme pour mettre le programme en pause, et qui ne passe pas par le framerate.
> GL::tick_per_frame iniialisé avec config::DEFAULT_TICKS_PER_SEC.
> ajout de 2 inputs : 'a' pour incrémenter de 1 le framerate et 'z' pour décrémenter de 1 le framerate.
> Pour le pause on crée un boolean dans opengl_interface qu'on va utiliser dans la fonction GL::timer, on fait un move du aircraft que si "is_paused vaut false"

3) Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.
> config::SERVICE_CYCLES -> 40u

4) Lorsqu'un avion a décollé, il réattérit peu de temps après. Assurez-vous qu'à la place, il soit supprimé de la move_queue.
Pour tester, il suffit de dézoomer et de vérifier que les avions suffisament éloignés ne bougent plus. Indices :
A quel endroit pouvez-vous savoir que l'avion doit être supprimé ? -> Aircraft::move\
Pourquoi n'est-il pas sûr de procéder au retrait de l'avion dans cette fonction ? -> parce que le seul qui own les avions est GL::move_queue\
A quel endroit de la callstack pourriez-vous le faire à la place ? -> GL::timer\
Que devez-vous modifier pour transmettre l'information de la première à la seconde fonction ? -> un bool

5) Lorsqu'un objet de type `Displayable` est créé, il faut ajouter celui-ci manuellement dans la liste des objets à afficher.
Il faut également penser à le supprimer de cette liste avant de le détruire.
Faites en sorte que l'ajout et la suppression de `display_queue` soit "automatiquement gérée" lorsqu'un `Displayable` est créé ou détruit.
Pourquoi n'est-il pas spécialement pertinent d'en faire de même pour `DynamicObject` ?
> on met l'ajout et la suppression des elements de la display queue dans le constructeur et le destructeur de Displayable.
> On peut donc retirer les ajouts et les suppressions

6) La tour de contrôle a besoin de stocker pour tout `Aircraft` le `Terminal` qui lui est actuellement attribué, afin de pouvoir le libérer une fois que l'avion décolle.
Cette information est actuellement enregistrée dans un `std::vector<std::pair<const Aircraft*, size_t>>` (size_t représentant l'indice du terminal).
Cela fait que la recherche du terminal associé à un avion est réalisée en temps linéaire, par rapport au nombre total de terminaux.
Cela n'est pas grave tant que ce nombre est petit, mais pour préparer l'avenir, on aimerait bien remplacer le vector par un conteneur qui garantira des opérations efficaces, même s'il y a beaucoup de terminaux.\
Modifiez le code afin d'utiliser un conteneur STL plus adapté. Normalement, à la fin, la fonction `find_craft_and_terminal(const Aicraft&)` ne devrait plus être nécessaire.
> Utiliser une unordered_map

## D- Théorie

1) Comment a-t-on fait pour que seule la classe `Tower` puisse réserver un terminal de l'aéroport ?
> On a fait en sorte que seul Airport appel assign_craft

2) En regardant le contenu de la fonction `void Aircraft::turn(Point3D direction)`, pourquoi selon-vous ne sommes-nous pas passer par une réference ?
Pensez-vous qu'il soit possible d'éviter la copie du `Point3D` passé en paramètre ?
> Parce qu'on veut appliquer un traitement à `direction` avant de l'ajouter à la vitesse.
> C'est possible mais pour ça il faudrait faire les traitements différement.

## E- Bonus

Le temps qui s'écoule dans la simulation dépend du framerate du programme.
La fonction move() n'utilise pas le vrai temps. Faites en sorte que si.
Par conséquent, lorsque vous augmentez le framerate, la simulation s'exécute plus rapidement, et si vous le diminuez, celle-ci s'exécute plus lentement.

Dans la plupart des jeux ou logiciels que vous utilisez, lorsque le framerate diminue, vous ne le ressentez quasiment pas (en tout cas, tant que celui-ci ne diminue pas trop).
Pour avoir ce type de résultat, les fonctions d'update prennent généralement en paramètre le temps qui s'est écoulé depuis la dernière frame, et l'utilise pour calculer le mouvement des entités.

Recherchez sur Internet comment obtenir le temps courant en C++ et arrangez-vous pour calculer le dt (delta time) qui s'écoule entre deux frames.
Lorsque le programme tourne bien, celui-ci devrait être quasiment égale à 1/framerate.
Cependant, si le programme se met à ramer et que la callback de glutTimerFunc est appelée en retard (oui oui, c'est possible), alors votre dt devrait être supérieur à 1/framerate.

Passez ensuite cette valeur à la fonction `move` des `DynamicObject`, et utilisez-la pour calculer les nouvelles positions de chaque avion.
Vérifiez maintenant en exécutant le programme que, lorsque augmentez le framerate du programme, vous n'augmentez pas la vitesse de la simulation.

> Pour faire ça j'ai donc ajouté une variable dans le .hpp permettant de se souvenir de la valeur précédente du timer.
> Ensuite dans timer je calcule la différence entre l'instant précédent et l'instant actuel que je multiplie par une constante (~1/60)
> pour obtenir une simulation fluide.
> Après je passe cette valeur au fonctions move qui vont donc l'utiliser pour : déplacer l'avion et s'occuper de l'avions dans un terminal

Ajoutez ensuite deux nouveaux inputs permettant d'accélérer ou de ralentir la simulation.

> 'o' : permet d'accelerer et 'l' permet de ralentir la vitesse de simulation.


