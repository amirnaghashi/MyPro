#esep-tut

--- 
# festo-simulation-qnx 
1. Projekt runterladen
2. Ordner in `project/src` in eigenes Projekt kopieren/linken (z.B. in den `src` Ordner oder ins "root")
   - Powershell Symlink: 
     ```Powershell
      New-Item -Path C:\LinkDir -ItemType SymbolicLink -Value F:\RealDir
     ```
   3. Im Makefile *includes* hinzufügen (je nach Position ist der Pfad anders)
	```Makefile
	INCLUDES += -Isrc/simulationbase
	INCLUDES += -Isrc/simulationadapterqnx
	INCLUDES += -Isrc/simulationudpqnx
	INCLUDES += -Isrc/simulationcore
	```
4.  Die Socket Library wird benötigt
	1. Im Makefile hinzufügen: `LIBS += -lsocket`
	2. Nach erstmaliger Ausführung: In der Launch-Config, im Reiter *Upload* sicherstellen, dass **Libsocket** ge-uploaded wird.
5. Bei Verwendung der VM müssen zwei andere Header-Dateien verwendet werden, als auf der Hardware
```
/* nur Simulation */
#include "simqnxgpioapi.h"
#include "simqnxirqapi.h"

/* nur Hardware */
#include <sys/mman.h>
#include <hw/inout.h>
```

# VMs
1. Installation von VMWare Workstation Player (Windows) bzw. VMWare Fusion (Mac, Non-M*-Chips)
2. Herunterladen der VMs aus Moodle
3. Entpacken und VMs mit VMWare öffnen
	1. Für VM1: `\Qnet_und_GNS_und_VMs\VMs\QNX\VM1\x86_64\VM1.vmx` 
	2. Für VM2: `\Qnet_und_GNS_und_VMs\VMs\QNX\VM1\x86_64\VM2.vmx` 
4. Die Pfade der virtuellen Festplatten der VMs werden nicht korrekt sein. Dies wird spätestens beim ersten Starten erkannt. 
	1. In den Einstellungen der VM Pfade für Hardisk1 und Hardisk2 setzten
		1. Harddisk1: ``VM*\ifs\x86_64\ifs.vmdk`` → **IFS**
		2. Harddisk2: ``VM*\ifs\x86_64\if.vmdk``   → **FS**
5. Es gibt Probleme im Zusammenspiel mit dem Hypervisor von Windows, wenn z.B. Docker installiert ist ⇒ Lange Startzeit
	1. Am besten die VM einmal starten und dann nur *pausieren*

# Simudp.conf
- für die Kommunikation zwischen den Simulationen muss pro VM eine Konfigurationsdatei erstellt werden.
	- diese geben die IP-Adressen und Ports der beiden Anlagen und des Hosts an.

- Format:
```
<IP der anderen VM>
<Port der anderen VM>
<IP dieser VM>
<Port dieser VM>
<IP des Hosts>
<Port des Hosts>
```
- IP und Port des Hosts sind unteranderem für die Visualisierung notwendig  
- :FiArrowRightCircle: Bei der zweiten Anlage muss ein "*falscher*" Port (der ins nichts führt) angegeben werden, damit die Werkstücke nicht im Kreis fahren.
![[simudp_conf.png]]

# Makros:
- Werden im Makefile bei der Compilation angegeben
- **Müssen mit einem `-D` vorher angeben werden.** 
	- Beispiel: `-DSIM_TWIN`
#### SIM_TWIN & SIM_TWIN_B
- SIM_TWIN bei dem Projekt der ersten Anlage
- Beides bei dem Projekt für die Zweite.
#### SIM_REPORT:
- Notwendig damit die Simulation die Aktionen an die Konsole und GUI schickt

#### SIM_MANUAL_START
- Ermöglicht das manuelle Starten der Simulation über den Start-Button
- Muss bei der 1. Anlage angegeben werden
- Beim Flashen muss dafür zuerst
#### SIM_PUSHER
- Anlage hat einen Pusher anstatt einer Weiche
#### SIM_SHOW_*
- Alle SHOW Makros geben Informationen auf der Konsole aus
- Überbleibsel von der Version, wo es noch kleine GUI gab
#### VMS
- IP der VM ist keine gültige IP z.B.: 0.0.0.0
- VMware repair command: [https://kb.vmware.com/s/article/65156](https://kb.vmware.com/s/article/65156)