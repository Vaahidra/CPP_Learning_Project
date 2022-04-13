## Templates

### Objectif 1 - Devant ou derrière ? 

La fonction `Aircraft::add_waypoint` permet de rajouter une étape au début ou à la fin du parcours de l'avion.
Pour distinguer ces deux cas, elle prend un argument booléen `front` (on parle alors de "flag") qui est évalué à l'exécution.
Votre objectif consistera à modifier cette fonction afin d'économiser cette évaluation.  

1. Aujourd'hui, cette fonction n'est pas utilisée dans le code (oups).
Du coup, pour être sûr que vos futurs changements fonctionneront correctement, commencez par modifier le code de la classe `Aircraft` de manière à remplacer :
```cpp
waypoints = control.get_instructions(*this);
```
par
```cpp
auto front = false;
for (const auto& wp: control.get_instructions(*this))
{
   add_waypoint(wp, front);
}
```

2. Modifiez `Aircraft::add_waypoint` afin que l'évaluation du flag ait lieu à la compilation et non à l'exécution.
Que devez-vous changer dans l'appel de la fonction pour que le programme compile ?

3. **BONUS** En utilisant [GodBolt](https://godbolt.org/), comparez le code-assembleur généré par les fonctions suivantes:
<table border="0">
 <tr>
    <td><pre lang="c++">
    int minmax(const int x, const int y, const bool min) {
        return x &lt; y ? (min ? x : y) : (min ? y : x);
    }
    </pre></td>
    <td><pre lang="c++">
    template&lt;bool min&gt;
    int minmax(const int x, const int y){
        return x &lt; y ? (min ? x : y) : (min ? y : x);
    }
    </pre></td>
 </tr>
</table>

### Objectif 2 - Points génériques

1. Reprenez les classes dans `geometry.hpp` et inspirez-vous de `Point2D` et `Point3D` pour définir une unique classe-template `Point` paramétrée par la dimension (nombre de coordonnées) et leur type (entier/float/double).
Pour ce qui est des constructeurs, vous n'ajouterez pour le moment que le constructeur par défaut.

2. Ajoutez une fonction libre `test_generic_points` à votre programme, que vous appelerez depuis le `main`. 
Placez le code suivant dans cette fonction et modifiez-le plusieurs fois, pour vérifier que le compilateur est capable de générer des classes à partir de votre template sans problème :
```cpp
Point<...> p1;
Point<...> p2;
auto p3 = p1 + p2;
p1 += p2;
p1 *= 3; // ou 3.f, ou 3.0 en fonction du type de Point
```

3. Ajoutez le constructeur à 2 paramètres de `Point2D` et le constructeur à 3 paramètres de `Point3D` dans votre classe-template.
Modifiez `Point2D` et `Point3D` afin d'en faire des alias sur des classes générées à partir du template `Point` (respectivement, 2 floats et 3 floats).
Vérifiez que votre programme compile et fonctionne comme avant.

4. Dans la fonction `test_generic_points`, essayez d'instancier un `Point2D` avec 3 arguments.
Que se passe-t-il ?
Comment pourriez-vous expliquer que cette erreur ne se produise que maintenant ?

5. Que se passe-t-il maintenant si vous essayez d'instancier un `Point3D` avec 2 arguments ?
Utilisez un `static_assert` afin de vous assurez que personne ne puisse initialiser un `Point3D` avec seulement deux éléments.
Faites en de même dans les fonctions `y()` et `z()`, pour vérifier que l'on ne puisse pas les appeler sur des `Point` qui n'ont pas la dimension minimale requise.

6. Plutôt qu'avoir un constructeur pour chaque cas possible (d'ailleurs, vous n'avez pas traité tous les cas possibles, juste 2D et 3D), vous allez utiliser un variadic-template et du perfect-forwarding pour transférer n'importe quel nombre d'arguments de n'importe quel type directement au constructeur de `values`.  
Vous conserverez bien entendu le `static_assert` pour vérifier que le nombre d'arguments passés correspond bien à la dimension du `Point`.\
En faisant ça, vous aurez peut-être désormais des problèmes avec la copie des `Point`.
Que pouvez-vous faire pour supprimer l'ambiguité ?

7. **BONUS** En utilisant SFINAE, faites en sorte que le template `Point` ne puisse être instancié qu'avec des types [arithmétiques](https://en.cppreference.com/w/cpp/types/is_arithmetic).
