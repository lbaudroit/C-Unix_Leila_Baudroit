**Performance : Quelle version est la plus rapide ? Pourquoi ?**
La version la plus rapide est la version parallèle, car elle effectue de manière "simultanée" les calculs plutôt que les
effectuer séquentiellement.

**Mesurez : Différentes valeurs pour MULTIPLICATEUR, jusqu’a à 10000000 et observez la différence**
C'est long.

**Résultats : Les résultats sont-ils identiques entre les versions ? Pourquoi n’y a-t-il pas de race condition ?**
Pour un même multiplicateur, les résultats ne changent pas.
Il n'y a pas de *race condition*, car le résultat ne dépend pas de l'ordre d'exécution. Chaque thread travaille sur des
données séparées, et n'a pas de risque d'interférer avec les autres. De plus, la somme faite dans le main ne dépend pas
de l'ordre dans lequel les threads se sont terminés.

**Vous pouvez écrire les résultats dans un fichiers au format csv (simple)**
1000	87	89	87	87	92
10000	839	855	853	850	855

**Scalabilité : Testez avec différents nombres de threads.**


**Combien de coeur a votre machine virtuel ? vous pouvez regarder dans /proc/cpuinfo**
12

**Que se passe-t-il si vous créez 100 threads pour 10 valeurs ?**


**Proposé un graphique telque visible ci-dessous (bonus)**

