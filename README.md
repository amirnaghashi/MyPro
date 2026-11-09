# ESEP-2026WS-Team-3-2

GitLab Projektmanagement:
https://docs.google.com/document/d/1ANJYAGmpav6I6pRFSckeOulPYVWuURhD4uvncWscbXw/edit?usp=sharing





Pre-Semester-Analysis:
https://docs.google.com/spreadsheets/d/1v6pex1WEeF98CodFj0WCmi8vHdUO2rkuaGQ5gfnAmQc/edit?gid=0#gid=0 

RDD Google docs link:
https://docs.google.com/document/d/1fqZ3rDjY321fd6XFRxcy1SNBNh5RE4jPUOTGVN0CaRQ/edit?tab=t.0

Anleitung Simulation:
https://hawhamburgedu-my.sharepoint.com/:w:/r/personal/wtu994_haw-hamburg_de/_layouts/15/Doc.aspx?sourcedoc=%7BA2D4030A-6CF9-45B4-9D4F-004D5ED27975%7D&file=Dokument%201.docx&action=editnew&mobileredirect=true&wdPreviousSession=a1e3780b-f6ea-4dfc-1745-d73abf4e9d6c&wdNewAndOpenCt=1792924194281&wdo=4&wdOrigin=wacFileNew&wdPreviousCorrelation=24df020d-89b3-4c6d-b54d-ac9312bd5e0e&wdnd=1

## Header und Includes

Header liegen neben der zugehörigen .cpp im Modulordner, z. B. `sorter/src/hal/Sensor.h` und `sorter/src/hal/Sensor.cpp`. Eingebunden werden sie immer relativ zu `sorter/src`, also `#include "hal/Sensor.h"` und nicht mit `../`. Den Pfad setzt das Makefile über `INCLUDES += -Isorter/src`.
