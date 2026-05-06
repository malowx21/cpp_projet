Commit du rendu intermédiaire : 36d6747
Commit du rendu final : 2a97cf7


# Simulation de Particule dans un univers en utilisant le potentiel de Lennard-Jones

> Simulation en C++ avec grille de cellules liées, conditions aux limites  et export VTK pour visualisation de la démonstration en utilisant  ParaView.

---

## Table des matières

1. [Description du projet](#description-du-projet)
2. [Architecture du code](#architecture-du-code)
3. [Modèle physique](#modèle-physique)
4. [Compilation et dépendances](#compilation-et-dépendances)
5. [Utilisation](#utilisation)
6. [Tests unitaires](#tests-unitaires)
7. [Visualisation avec ParaView](#visualisation-avec-paraview)
8. [Résultats — Démonstration](#résultats--démonstration)
9. [Détails d'implémentation](#détails-dimplémentation)
9. [Conclusion](#Conclusion)

---

## Description du projet

Ce projet implémente une simulation de **particule dynamique dans un univers**  en C++. Cet ensemble de particules ponctuelles interagit via le **potentiel de Lennard-Jones** tronqué (rayon de coupure $r_{\text{cut}}$). L'intégration temporelle utilise l'algorithme de **Störmer–Verlet** , qui conserve l'énergie à long terme.

Le cas de démonstration reproduit l'**expérience du disque tombant dans une mer de particules** du tp6 : un disque rigide  chute sous l'effet de la gravité dans un fluide de particules, avec conditions aux limites réfléchissantes.

---

## Architecture du code

```
.
├── CMakeLists.txt
├── Doxyfile
├── include/                        # Headers (.hpp)
|   ├── Vecteur.hpp
│   ├── Particule.hpp               
│   ├── Cellule.hpp               
│   └── Univers.hpp                              
├── src/                            # Sources (.cxx)
│   ├── Vecteur.cxx                 # Classe Vecteur 
│   ├── Particule.cxx               # Classe Particule
│   └── Univers.cxx                 # Classe Univers 
├── demo/
│   └── demo_univers.cxx            # Programme principal de la  simulation 
├── test/
│   ├── test_vecteur.cxx            # Tests unitaires de la classe Vecteur
│   ├── test_particule.cxx          # Tests unitaires de la classe Particule
│   ├── test_univers.cxx            # Tests unitaires de la  classe Univers
│   └── test_force.cxx              # Tests unitaires des forces de Lennard-Jones
├── diagrammes/                     # Diagrammes UML 
│   ├── classeana.puml              # Diagramme de classes
│   ├── sequence.puml               # Diagramme de séquence
│   ├── transitions.puml            # Diagramme d'états transitions
│   └── usecase.puml                # Diagramme de cas d'utilisation
├── docs/                           # Documentation générée (Doxygen)
└── build/                          # Répertoire de compilation (CMake)
```

### Diagramme de dépendances

```
Vecteur
   └── Particule
          └── Cellule
                 └── Univers
                        └── demo_univers (main)
```


### Diagrammes UML
 
Les diagrammes UML du projet sont rédigés en **PlantUML** dans le dossier `diagrammes/`. Ils décrivent la structure et les interactions des classes.
 
| Fichier | Type | Contenu |
|---|---|---|
| `classeana.puml` | Diagramme de classes | Attributs, méthodes et relations entre `Vecteur`, `Particule`, `Cellule`, `Univers` |
| `sequence.puml` | Diagramme de séquence | Déroulement d'un pas de temps (appel à `avancer`) |
| `transitions.puml` | Diagramme d'états | Cycle de vie d'une particule (création ,simulation et  suppression) |
| `usecase.puml` | Diagramme de cas d'utilisation | Interactions utilisateur avec le système de simulation |
 
#### Générer les diagrammes

Vous pouvez visualiser les fichiers `.puml` de plusieurs façons :

- Avec l’extension PlantUML dans VS Code (en effectuant alt+d sur le fichier `.puml`)
- En générant des images :
  plantuml diagrammes/*.puml


---

## Modèle physique

### Potentiel de Lennard-Jones tronqué

$$V_{LJ}(r) = 4\varepsilon \left[ \left(\frac{\sigma}{r}\right)^{12} - \left(\frac{\sigma}{r}\right)^{6} \right], \quad r < r_{\text{cut}}$$

La **force** entre deux particules $i$ et $j$ séparées de $\vec{r}_{ij}$ est :

$$\vec{F}_{ij} = \frac{24\,\varepsilon}{r^2} \left[ \left(\frac{\sigma}{r}\right)^6 - 2\left(\frac{\sigma}{r}\right)^{12} \right] \vec{r}_{ij}$$

- Distance d'équilibre : $r_{eq} = 2^{1/6}\,\sigma$
- Pour $r < r_{eq}$ : force **répulsive** ; pour $r > r_{eq}$ : force **attractive**
- Troisième loi de Newton appliquée : $\vec{F}_{ji} = -\vec{F}_{ij}$

### Intégrateur de Störmer–Verlet 

$$\vec{x}(t+\Delta t) = \vec{x}(t) + \vec{v}(t)\,\Delta t + \frac{\Delta t^2}{2m}\vec{F}(t)$$

$$\vec{v}(t+\Delta t) = \vec{v}(t) + \frac{\Delta t}{2m}\bigl[\vec{F}(t) + \vec{F}(t+\Delta t)\bigr]$$

### Conditions aux limites

| Type | Valeur `type_border` | Comportement |
|------|---------------------|--------------|
| **Réflexion** | `0` | La composante normale de la vitesse est inversée |
| **Périodique** | `1` | La particule réapparaît du côté opposé |
| **Absorption** | `2` | La particule est supprimée |

### Optimisation en utilisant une grille de cellules liées

L'espace est découpé en cellules de taille $r_{\text{cut}}$. Seules les particules dans des cellules voisines (au plus 27 voisins en trois dimensiosn) sont candidates à l'interaction, réduisant la complexité de $O(N^2)$ à $O(N)$ pour des distributions homogènes.

---

## Compilation et dépendances

### Prérequis

- Compilateur C++17 
- [CMake](https://cmake.org/) 
- [Google Test](https://github.com/google/googletest) (pour les tests)
- [ParaView](https://www.paraview.org/) (pour la visualisation VTK)

### Build avec CMake

```bash
mkdir build && cd build
cmake .. 
make 
```

Les exécutables générés sont :
 
| Exécutable | Chemin dans `build/` | Description |
|---|---|---|
| `demo_univers` | `demo/demo_univers` | Simulation principale  |
| `test_vecteur` | `test/test_vecteur` | Tests unitaires la classe Vecteur |
| `test_particule` | `test/test_particule` | Tests unitaires de la classe Particule |
| `test_univers` | `test/test_univers` | Tests unitaires de la classe Univers |
| `test_force` | `test/test_force` | Tests unitaires des  Forces Lennard-Jones |
 
---

## Utilisation

### Lancer la simulation de démonstration

```bash
./build/demo/demo_univers
```

La simulation tourne jusqu'à $t = 29.5$ avec $\Delta t = 0.0005$ (59 000 pas).  
Les fichiers VTK sont écrits dans `vtk_output/` toutes les 50 itérations (après un warm-up de 100 pas).

### Paramètres configurables (dans `demo_univers.cxx`)

| Paramètre | Valeur par défaut | Description |
|---|---|---|
| `EPSILON` | `1.0` | Profondeur du puits Lennard-Jones |
| `SIGMA` | `1.0` | Diamètre de collision |
| `RCUT` | `2.5 x SIGMA` | Rayon de coupure |
| `DT` | `0.0005` | Pas de temps |
| `T_END` | `29.5` | Temps final |
| `G` | `-12.0` | Accélération gravitationnelle (axe Y) |
| `ECIBLE` | `0.005 × N` | Énergie cinétique cible pour le rescaling |



---

## Tests unitaires

Les tests utilisent **Google Test** et couvrent les quatre composants principaux.


### Lancer tous les tests

```bash
cd build
ctest --verbose
# ou individuellement :
./test_vecteur
./test_particule
./test_univers
./test_force
# ou avec filtrage
./tests --gtest_filter=VecteurTest.Norme
#tous de même fichier
./tests --gtest_filter=VecteurTest.*
#ou plusieurs suites
./tests --gtest_filter=VecteurTest.*:ParticuleTest.*
#ou tout sauf une suite
./tests --gtest_filter=-ForcesLJTest.*


```

### Couverture des tests

#### `test_vecteur` de la classe `Vecteur`
| Test | Description |
|---|---|
| `Constructeurs` | Constructeur par défaut et paramétré |
| `Norme` | Norme euclidienne (cas dégénérés inclus) |
| `Addition / Soustraction` | Opérateurs `+`, `-`, `+=`, `-=` |
| `Scalaire` | Multiplication et division par scalaire |
| `DivisionByZero` | Exception levée si diviseur est presque  nul |
| `CopyAndAssignment` | Constructeur par copie et opérateur d'affectation `=` |
| `ConsistencyAdd` | Cohérence entre `+` et `+=` |

#### `test_particule` de la  classe `Particule`
| Test | Description |
|---|---|
| `ConstructeurParDefaut` | Initialisation à zéro |
| `SettersGetters` | Accesseurs et mutateurs |
| `ForceMutable` | Modification de la force via référence |
| `CycleDeVie` | Copie, affectation, indépendance |
| `AutoAffectation` | Protection contre `p = p` |
| `FluxSortie` | Opérateur de flux `<<` |

#### `test_univers` de la classe  Classe `Univers`
| Test | Description |
|---|---|
| `GrilleDimensions` | Découpage correct en cellules |
| `AffectationCellule` | Indexation d'une particule dans sa cellule |
| `Voisins` | Nombre de voisins (coin, bord, centre) |
| `ConservationQuantiteMouvement` | Conservation de la quantité de mouvement |
| `AjoutParticules` | Incrémentation du conteneur |
| `ReflexionX` | Condition réfléchissante |
| `PeriodiqueX` | Condition périodique |
| `Absorption` | Suppression de la particule |
| `RescalingWorks` | Mise à l'échelle des vitesses |

#### `test_force` des  forces de Lennard-Jones
| Test | Description |
|---|---|
| `DistanceEquilibre` | Force nulle à $r_{eq} = 2^{1/6}\sigma$ |
| `SigneForce` | Répulsion/attraction selon $r$ vs $r_{eq}$ |
| `Newton3` | Troisième loi de Newton : $\vec{F}_{ij} = -\vec{F}_{ji}$ |
| `Rcut` | Aucune interaction au-delà de $r_{\text{cut}}$ |
| `ValeurAnalytique` | Comparaison simulation vs calcul analytique |

---

## Visualisation avec ParaView

1. Lancer la simulation , les fichiers `vtk_output/frame_*.vtk` sont générés
2. Ouvrir ParaView : `File > Open` → sélectionner `frame_..vtk` (série temporelle)
3. Colorier par le champ scalaire `group` :
   - `0` = particules de la **mer** (fluide)
   - `1` = particules du **disque** (solide tombant)
4. Appuyer sur **Play** pour visualiser l'animation

---

## Résultats — Démonstration

La simulation reproduit l'expérience du **disque tombant dans un fluide** :

- Le disque (~395 particules de masse 3) chute sous la gravité ($g = -12$)
- Le fluide (~17 227 particules de masse 1) se déforme et s'écoule autour du disque
- Un rescaling des vitesses est appliqué toutes les 1000 itérations (après $t > 2$) pour stabiliser la température numérique à $E_c^{\text{cible}} = 0.005 \times N$
- La validité de la simulation (positions et énergie finies) est vérifiée toutes les 100 itérations

**Domaine de simulation** : $250 \times 180$ unités, conditions aux limites réfléchissantes.

---

## Détails d'implémentation

### Optimisations performances

- **Grille de cellules liées** : complexité $O(N)$ au lieu de $O(N^2)$ pour le calcul des forces
- **Symétrie Newton** : chaque paire $(i, j)$ traitée une seule fois (`jc >= ic`, `j > i`)
- **Évitement des copies** : accès aux attributs des particules par référence (`Vecteur&`)
- **Clamp de divergence** : distance minimale forcée à $0.5\sigma$ pour éviter les singularités

### Stabilité numérique

- Vérification de la finitude des positions et de l'énergie cinétique (`check_validite`)
- Rescaling périodique des vitesses pour contrôler la dérive thermique
- Pas de temps $\Delta t = 0.0005$ choisi pour la stabilité de l'intégrateur Verlet

### Forces de paroi

Une **force de Lennard-Jones répulsive** est appliquée aux particules proches des bords (distance < $2^{1/6}\sigma$), empêchant les particules de traverser les parois même en mode réfléchissant.


---

## Conclusion

Ce projet propose une implémentation complète et cohérente d'une simulation de particules en C++ basée sur le potentiel de Lennard-Jones. Il met en évidence plusieurs aspects fondamentaux de la simulation numérique en physique :

- la modélisation d’interactions microscopiques réalistes
- l’intégration temporelle stable avec l’algorithme de Störmer–Verlet
- l’optimisation des performances grâce à une grille de cellules liées
- la validation du comportement physique via des tests unitaires rigoureux

L’architecture modulaire (Vecteur, Particule, Cellule, Univers) facilite la compréhension, la maintenance et l’extension du code. L’intégration avec l’export VTK et la visualisation sous ParaView permet d’observer concrètement les phénomènes simulés, renforçant l’intérêt pédagogique du projet.

Enfin, ce travail constitue une base solide pour des extensions futures, telles que :
- l’ajout de nouveaux potentiels d’interaction
- la simulation en trois dimensions
- la parallélisation (OpenMP / MPI)
- ou encore l’introduction de conditions physiques plus complexes

Ce projet illustre ainsi le lien entre modélisation théorique, implémentation algorithmique et observation expérimentale dans le domaine de la simulation numérique.