# Git Anleitung ESEP

Kurze Anleitung für den Alltag. Du suchst dir die Situation raus, in der du gerade bist, und arbeitest die Schritte ab.
Die Regeln dahinter (Branchnamen, Squash, Tags) stehen im Repokonzept.

Jede Situation hat zwei Wege:
- **CLion**: so arbeiten wir normalerweise
- **Terminal**: dieselben Schritte als Befehle, funktioniert in Git Bash und PowerShell

---
 
## Inhalt

1. [Einmalig: Repo einrichten](#1-einmalig-repo-einrichten)
2. [Arbeitstag starten](#2-arbeitstag-starten)
3. [Neue Aufgabe anfangen](#3-neue-aufgabe-anfangen)
4. [Während der Arbeit](#4-während-der-arbeit)
5. [Aufgabe ist fertig: Merge Request](#5-aufgabe-ist-fertig-merge-request)
6. [Review für jemand anderen machen](#6-review-für-jemand-anderen-machen)
7. [Nach dem Merge aufräumen](#7-nach-dem-merge-aufräumen)
8. [Fehler gefunden](#8-fehler-gefunden)
9. [Konflikt beim Rebase](#9-konflikt-beim-rebase)
10. [Stand für einen Praktikumstermin taggen](#10-stand-für-einen-praktikumstermin-taggen)
11. [Vorlagen für Issues und Merge Requests](#11-vorlagen-für-issues-und-merge-requests)
12. [Kleine Notfälle](#12-kleine-notfälle)
13. [Spickzettel](#13-spickzettel)

---

## 1. Einmalig: Repo einrichten

Nur beim allerersten Mal auf einem neuen Rechner.

**CLion**
1. Startbildschirm → **Clone Repository**
2. URL aus GitLab einfügen (Button **Code** → **Clone with HTTPS**)
3. Ordner wählen, z.B. `C:\Users\<name>\CLionProjects\esep-2026ws-team-3-2`

**Terminal**
```bash
git clone https://git.haw-hamburg.de/group-esep-2026ws-team-3-2/esep-2026ws-team-3-2.git
cd esep-2026ws-team-3-2
git config user.name "Vorname Nachname"
git config user.email "vorname.nachname@haw-hamburg.de"
```

> Name und Mail einmal setzen, sonst stehen deine Commits unter einem falschen Namen.

---

## 2. Arbeitstag starten

### Fall A: Ich fange etwas Neues an
Erst `main` aktualisieren, dann weiter mit [Abschnitt 3](#3-neue-aufgabe-anfangen).

**CLion**
1. Unten rechts (oder oben links) auf den Branchnamen klicken → **main** → **Checkout**
2. **Git → Pull** (oder `Ctrl+T`)

**Terminal**
```bash
git switch main
git pull
```

### Fall B: Ich arbeite an meinem Branch weiter
Den neuen Stand von `main` in deinen Branch holen. Mindestens einmal am Tag, dann bleiben Konflikte klein.

**CLion**
1. Auf deinen Branch wechseln
2. **Git → Fetch**
3. Branchmenü → **origin/main** → **Rebase Current onto Selected**
4. **Git → Push**, dabei **Force Push** wählen

**Terminal**
```bash
git switch feature/12-measure-height
git fetch origin
git rebase origin/main
git push --force-with-lease
```

> Force Push nur auf deinem eigenen Arbeitsbranch. Nie auf `main`.

---

## 3. Neue Aufgabe anfangen

**Schritt 1: Issue in GitLab**
1. Gibt es schon ein Issue? Dann dich zuweisen und weiter zu Schritt 2.
2. Sonst: **Issues → New issue**
3. Bei **Description** die passende Vorlage wählen (siehe [Abschnitt 11](#11-vorlagen-für-issues-und-merge-requests))
4. Ausfüllen, Milestone (Praktikumstermin) setzen, dich zuweisen
5. Schätzung als Kommentar: `/estimate 4h`

**Schritt 2: Branch anlegen**

Name: `<typ>/<issuenummer>-<kurzbeschreibung>`, englisch, klein.

| Typ | Wofür |
|---|---|
| `feature/` | neue Funktion |
| `fix/` | Fehler beheben |
| `refactor/` | Umbau ohne Verhaltensänderung |
| `test/` | nur Tests |
| `docs/` | Doku, Diagramme |
| `chore/` | Build, Konfiguration, Aufräumen |

**CLion**
1. Vorher sicherstellen: du bist auf `main` und hast gepullt ([Fall A](#fall-a-ich-fange-etwas-neues-an))
2. Branchmenü → **New Branch**
3. Name eingeben, z.B. `feature/12-measure-height`, **Checkout branch** angehakt lassen

**Terminal**
```bash
git switch main
git pull
git switch -c feature/12-measure-height
```

---

## 4. Während der Arbeit

Klein und oft committen. Am ersten Tag einmal pushen, damit der Branch für alle sichtbar ist.

**CLion**
1. **Commit** (`Ctrl+K`): Dateien anhaken, Nachricht schreiben, **Commit**
2. **Push** (`Ctrl+Shift+K`)

**Terminal**
```bash
git status
git add sorter/src/hal/HeightSensor.cpp
git commit -m "read height values from ADC"
git push -u origin feature/12-measure-height
```

Das `-u` nur beim ersten Push, danach reicht `git push`.

> Commits im Branch dürfen frei benannt werden. Nur der Titel des Merge Requests muss dem Schema folgen, weil wir squashen.

**Zeit buchen** nicht vergessen, als Kommentar am Issue: `/spend 1h30m`

---

## 5. Aufgabe ist fertig: Merge Request

1. Vorher einmal [Fall B](#fall-b-ich-arbeite-an-meinem-branch-weiter) machen, damit dein Branch auf dem neusten `main` liegt
2. In GitLab: **Merge requests → New merge request**, Quelle dein Branch, Ziel `main`
3. **Titel** nach Schema, englisch:
   ```
   feature: read height values from sensor
   ```
4. Die **Beschreibung** ist schon vorausgefüllt (Vorlage). Ausfüllen:
   - **Was** und **Warum**
   - `Closes #NR` → echte Issuenummer eintragen, z.B. `Closes #12`. Dann schließt GitLab das Issue beim Merge.
   - Checkliste **Erledigt** abhaken
5. Eine Person als **Reviewer** eintragen
6. **Create merge request**

---

## 6. Review für jemand anderen machen

Bitte innerhalb eines Werktags, damit niemand blockiert ist.

1. MR öffnen → Tab **Changes**
2. Prüfen:
   - Macht der Code, was in der Beschreibung steht?
   - Verstehe ich ihn ohne Erklärung?
   - Werden gemeinsame Schnittstellen geändert, und wissen die anderen davon?
   - Sind die Punkte unter **Erledigt** wirklich abgehakt?
3. Anmerkungen direkt an die Zeile schreiben
4. Passt alles: **Approve**

Gemergt wird danach von der Person, die den MR geöffnet hat.

---

## 7. Nach dem Merge aufräumen

GitLab löscht den Branch auf dem Server automatisch. Lokal machst du es selbst.

**CLion**
1. Auf **main** wechseln, **Pull**
2. Branchmenü → deinen alten Branch → **Delete**

**Terminal**
```bash
git switch main
git pull
git branch -D feature/12-measure-height
```

> `-D` statt `-d`, weil Git wegen Squash nicht erkennt, dass der Branch schon gemergt ist.

Nächste Aufgabe: wieder bei [Abschnitt 3](#3-neue-aufgabe-anfangen).

---

## 8. Fehler gefunden

Gleicher Ablauf wie bei einer Aufgabe, mit drei Unterschieden:

1. Issue mit der Vorlage **Bug** anlegen, inklusive wie man den Fehler auslöst
2. Branch heißt `fix/<nr>-<kurzbeschreibung>`, z.B. `fix/15-sensor-timeout`
3. Betrifft der Fehler einen getaggten Stand: Fix normal auf `main`, danach neuer Tag (siehe [Abschnitt 10](#10-stand-für-einen-praktikumstermin-taggen))

---

## 9. Konflikt beim Rebase

Passiert, wenn jemand dieselben Zeilen geändert hat wie du. Kein Grund zur Panik.

**CLion**
1. CLion öffnet das Fenster **Conflicts** automatisch
2. Pro Datei **Merge** → links deine Version, rechts die andere, Mitte das Ergebnis
3. Übernehmen, was richtig ist, **Apply**
4. Wenn alle Dateien gelöst sind, läuft der Rebase weiter
5. Kompilieren, Tests laufen lassen, dann Force Push

**Terminal**
```bash
git status                     # zeigt Dateien mit Konflikt
# Datei öffnen, Bereiche zwischen <<<<<<< und >>>>>>> auflösen
git add <datei>
git rebase --continue
git push --force-with-lease
```

Abbrechen und zurück zum Stand vorher:
```bash
git rebase --abort
```

> Unklar, welche Version richtig ist? Die Person fragen, die den anderen Teil geschrieben hat, statt zu raten.

---

## 10. Stand für einen Praktikumstermin taggen

Nur ein Stand, der am Festomodul getestet wurde. Schema: `termin-1` bis `termin-5`, `endabnahme`.

**Terminal** (in CLion: **Git → New Tag**, danach **Push** mit **Push Tags**)
```bash
git switch main
git pull
git tag -a termin-2 -m "Stand für den 2. Praktikumstermin"
git push origin termin-2
```

Tags werden nie verschoben oder gelöscht. Korrektur → neuer Tag, z.B. `termin-2-fix`.

---

## 11. Vorlagen für Issues und Merge Requests

Die Vorlagen liegen im Repo unter `.gitlab/`. Änderungen daran laufen wie Code über einen Merge Request.

```
.gitlab/
├── issue_templates/
│   ├── Feature.md
│   ├── Docs.md
│   ├── Bug.md
│   └── Orga.md
└── merge_request_templates/
    └── Default.md
```

### Welche Vorlage wann?

| Vorlage | Wann | Label wird gesetzt |
|---|---|---|
| **Feature** | Code für eine Funktion | `feature` |
| **Docs** | RDD, Diagramme, Konzepte | `docs` |
| **Bug** | etwas funktioniert nicht | `bug` |
| **Orga** | Meetings, Einrichtung, Absprachen | `Orga` |

Das Label musst du nicht selbst setzen. Die letzte Zeile der Vorlage (z.B. `/label ~feature`) macht das beim Speichern.

### So nutzt du eine Issue Vorlage

1. **Issues → New issue**
2. Über dem Beschreibungsfeld: **Choose a template** → Vorlage wählen
3. Die Abschnitte ausfüllen, Kommentare in `<!-- -->` sind nur Hinweise und werden nicht angezeigt
4. **Akzeptanzkriterien** prüfbar formulieren: Was muss man sehen oder messen können, damit es fertig ist?
5. Milestone setzen, dich zuweisen, **Create issue**

### Was in jeder Vorlage steckt

- **Anforderungen**: Nummern aus der Aufgabenstellung, z.B. `43, 59`
- **Akzeptanzkriterien**: was speziell dieses Issue erfüllen muss
- **Definition of Done**: was für jedes Issue dieser Art gilt. Erst wenn alles abgehakt ist, wird das Issue geschlossen.

### Merge Request Vorlage

Die erscheint automatisch in jedem neuen Merge Request, du musst nichts auswählen.
Die Checkliste **Erledigt** ist die Definition of Done für die Codeänderung.

### Abnahmetests

Die Vorlage für Abnahmetests liegt in `docs/Abnahmetest_Vorlage.md` und wird ins RDD übernommen.
Pro Schritt: **Input** (was du tust), **erwarteter Output** (was passieren soll), **Ist** (was passiert ist).
Betrifft der Test die Sortiermechanik, einmal mit Weiche und einmal mit Auswerfer durchführen.

---

## 12. Kleine Notfälle

**Ich habe auf dem falschen Branch angefangen (noch nicht committet)**
```bash
git stash
git switch richtiger-branch
git stash pop
```
CLion: **Git → Uncommitted Changes → Stash Changes**, Branch wechseln, dann **Unstash Changes**.

**Ich will meine letzten, noch nicht gepushten Änderungen an einer Datei wegwerfen**
```bash
git restore <datei>
```
CLion: Rechtsklick auf die Datei → **Git → Rollback**.

**Ich habe vergessen, was ich geändert habe**
```bash
git status
git diff
```
CLion: Fenster **Commit** (`Ctrl+K`) zeigt alle Änderungen.

**Ich sehe den Branch einer anderen Person nicht**
```bash
git fetch origin
git switch feature/20-control-diverter
```

> Wenn du unsicher bist: nichts mit `--force` oder `reset --hard` ausprobieren, lieber kurz in die Gruppe schreiben.

---

## 13. Spickzettel

| Ich will ... | Terminal | CLion |
|---|---|---|
| auf main wechseln und aktualisieren | `git switch main` + `git pull` | Branchmenü → main → Checkout, `Ctrl+T` |
| neuen Branch anlegen | `git switch -c feature/12-xyz` | Branchmenü → New Branch |
| committen | `git add` + `git commit -m "..."` | `Ctrl+K` |
| pushen | `git push` | `Ctrl+Shift+K` |
| main in meinen Branch holen | `git fetch origin` + `git rebase origin/main` | Fetch, dann origin/main → Rebase Current onto Selected |
| nach Rebase pushen | `git push --force-with-lease` | Push → Force Push |
| alten Branch löschen | `git branch -D feature/12-xyz` | Branchmenü → Branch → Delete |
| Zeit schätzen / buchen | Kommentar am Issue: `/estimate 4h`, `/spend 1h30m` | in GitLab |
