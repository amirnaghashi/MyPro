
[Git Branches in a Nutshell](https://git-scm.com/book/en/v2/Git-Branching-Branches-in-a-Nutshell)
[Git Branching and Merging](https://git-scm.com/book/en/v2/Git-Branching-Basic-Branching-and-Merging)
# Git Branches 

## Erstellen von Branches
Erstellen eines Branches vom aktuellen Branch:
```bash
git branch branch1
```
Erstellen eines Branches und folgendem Wechsel zu dem neuen branch
```bash
git checkout -b branch1
```

Normales Wechseln von Branches mit 
```bash
git checkout branch1
```

## Löschen von Branches
```bash
git branch -d branch1
```

## Merging

Auf den Branch wechseln, in den ge-merged werden soll (z.b. Master):
```bash
git checkout master
```
Dann denn gewünschten Branch mergen:
```bash
git merge branch1
```

### Lösen von Merge Conflicts
```cpp
...

<<<<<<< HEAD:main.cpp
	int a = 0;  // Lokale/eigene Änderungen
=======
	int a = 1; // Änderungen des anderen Commits 
>>>>>>> 77976da35a11db4580b80ae27e8d65caf5208086:main.cpp
...
```

#### Lösen durch
- manuelles editieren der Dateien
	- **Nicht Vergessen**: Nachdem die Dateien angepasst wurden, müssen sie noch commited werden
- ein visuellen Git-Clients wie SourceTree, Tortoise GitKraken, Fork 
- mit IDE integrierten Tools wie von Jetbrains IDEAs oder VS Code



## Pushen von Branches
Bei jedem ersten Push von einem lokal erstellten Branch muss der zugehörige *Remote-Branch* gesetzt werden.
1. Wenn ein Remote-Branch existiert:
```bash
git branch -u origin/remoteBranch1
```
2. Wenn er nicht existiert, beim ersten Pushen

```bash
git push --set-upstream origin branch1
git push -u origin branch1
```

In den meisten Fällen ist das *origin*. Aber wenn man mehrere Remotes hat, kann das auch ein anderer Name sein.

**Wichtig**: *Fetchen* nicht vergessen, damit man auch neue Remote-Branches lokal sehen kann.

## Status von Branches
Zeigt für alle lokalen Branches den aktuellen HEAD und zugehörige Remotes
```bash
git branch -v
git branch -vv (verbose)
```

Informationen über den Remote, die Beziehungen zwischen lokalen Banches zu Remote-Branches.
```bash
git remote show origin
```


## Rebase 
Praktisch fürs integrieren neuer Änderungen in den eigenen Branch.

Alternative zu Merge. Die Änderungen eines Branches können auf einem anderen *wieder abgespielt* werden.
Hierbei werden die Änderungen des Branches `experiment` auf `master` abgespielt.
```bash
git checkout experiment
git rebase master
```
oder in einem 
```bash
git rebase master experiment
```


Dann wechseln auf den Master und ein *Fast-Forward Merge* durchführen, welcher den HEAD aktualisiert (beide Branches haben die selbe History).
```bash
git checkout master
git merge experiment
```
Dadurch ist der Commit `C4'` der gleiche wie `C5` nach dem Merge (siehe Bilder)

Die Commit-History ist dadurch linear, da kein Branching und Merging stattfindet.
Ob das Mergin oder Rebase besser ist, ist schwierig zu sagen. Durch Rebase wird es aufgeräumter, aber es geht die eigentliche History verloren bzw. verändert.

### Rebase
![[s2r9gs1g.bmp| 700]]
### Merge:
![[5b5lnn3p.bmp|700]]

**WICHTIG**: Keine Commits *rebasen* auf denen die Arbeit anderer basiert. Am besten nur für lokale Branches verwenden.
Mehr dazu: [The Perils of Rebasing](https://git-scm.com/book/en/v2/Git-Branching-Rebasing)

# Workflows / Branching Models

## Trunk-based
Workflow mit nur einem Branch (manchmal kurzlebige Feature-Branches) meisten *Master*.

- Für Teams ist hierbei sind strikte Coding-Styles und allgemein Erfahrung gefordert. Nichts für Anfänger.
- Für einen schnellen Prototypen oder eine privates Projekt ausreichend.

## Mehrere Branches 

### Branches
- Main
- Develop
- Features

- verbreitete Modelle 
	- Git-Flow (Main + Dev +features + hotfixes + releases)
	- Github-Flow (Main + Feautres)
### Git Flow 
![[augtij3n.bmp|700]]
(https://www.toptal.com/software/trunk-based-development-git-flow)
#### Branches:
- Main / Master → Stable 
- Develop / Dev (etc.) → Entwicklungsstand des Projektes 
- Feature Branches → kurzlebig, neue Features, Änderungen, etc. 
- Hotfix Branches → schnelle, kleine fixes
- Release → Für die Vorbereitung für eine Release; kleine Bugfixes, Metadaten (Dev kann schon für nächsten Release vorbereiten werden)

#### Git Flow Extension
Erweiterung für Git für die Arbeit mit Git-Flow. Kapselt git-commands in neue Git-Flow commands.
[Git-Flow Cheatsheet](https://danielkummer.github.io/git-flow-cheatsheet/)


## Github Flow
[Github Flow](https://docs.github.com/de/get-started/using-github/github-flow)


