#esep-tut 

---
# 1. Installation
## Windows
- [Download-Seite](https://mosquitto.org/download/)
- Mit **winget**: `winget install EclipseFoundation.Mosquitto`
## Mac 
- `brew install mosquitto`
# 2. Konfiguration
- Kopie der `mosquitto.conf` im Installationsverzeichnis erstellen
- Kopie anpassen:
	- **Zeile 227**: `listener <port> <ip>` 
		- Standard-Port bei MQTT ist ``1883``
		- IP-Adresse ist die des eigenen Computers.
	- **Zeile 524**: `allow_anonymous true` 
		- erlaubt Clients die Registrierung ohne Credentials
- Weitere Einstellungen nach eigenem Ermessen
# 3. Nutzung
- Wenn `mosquitto.exe` in der ``PATH``-Umgebungsvariable zu sehen ist oder man sich im Installationsverzeichnis befindet:
```
mosquitto[.exe] -c mosquitto.conf
```
- `.exe` ist optional. 

