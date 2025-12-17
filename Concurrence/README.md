# Exercice 1

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

---

# Exercice 2
**Observation typique : Vous devriez voir des résultats variables, souvent inférieurs à 9999999.**
**Plus il y a de threads, plus les “pertes” sont fréquentes. Que ce passe t’il ?**
Les threads écrivent les uns par dessus les autres la valeur du compteur, ce qui entraîne des pertes.

**Changez le nombre de threads (1, 2, 5, 10, 20, 40, 60, 100)**
- **Impact sur la fréquence des race conditions ?**
- **Avec 1 thread, devrait toujours être correct**
Avec 1 seul thread, le résultat est toujours bon.
Plus on augmente le nombre de threads et le nombre d'incréments, plus le taux d'erreur est élevé.
Pour nb_inc = 100 et nb_th = 50, par exemple, on a que 92% d'erreur.
Par contre, avec un nombre d'incréments élevés, on a dès problèmes dès le 2e thread.

**Changez le nombre d’incréments par thread**
- Plus d’opérations = plus de problèmes ?
  Compilez avec optimisations : gcc -O2 -pthread test_compteur.c
- Constatez vous des différences, si oui les quelles ?

Le nombre d'incrément impacte fortement le nombre d'erreur.
Avec optimisations, on diminue de manière importante le nombre d'erreur : 92% pour nb_th=500 et nb_inc=100000