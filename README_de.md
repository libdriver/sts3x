[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver STS3X
[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/sts3x/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der STS3x-DIS ist Sensirions neuer hochpräziser digitaler Temperatursensor. Er basiert auf der branchenerprobten CMOSens®-Technologie und bietet im Vergleich zu seinen Vorgängern mehr Intelligenz, Zuverlässigkeit und verbesserte Genauigkeitsspezifikationen. Seine Funktionalität umfasst eine verbesserte Signalverarbeitung, zwei unterschiedliche und vom Benutzer wählbare I2C-Adressen und Kommunikationsgeschwindigkeiten von bis zu 1 MHz. Das DFN-Gehäuse hat eine Grundfläche von 2,5 x 2,5 mm2 bei einer Höhe von 0,9 mm. Dies ermöglicht die Integration des STS3x-DIS in eine Vielzahl von Anwendungen. Darüber hinaus garantiert der breite Versorgungsspannungsbereich von 2,15 V bis 5,5 V Kompatibilität mit einer Vielzahl von Anwendungen. Alles in allem steckt im STS3x-DIS mehr als 15 Jahre digitales Sensor-Know-how von Sensirion.

LibDriver STS3X ist der voll funktionsfähige Treiber von STS3X, der von LibDriver eingeführt wurde. Er bietet Funktionen für die Temperaturanzeige im Dauermodus und die Temperaturanzeige im Einzelmodus. LibDriver ist MISRA-kompatibel.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver STS3X-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver STS3X IIC.

/test enthält den Testcode des LibDriver STS3X-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver STS3X-Beispielcode.

/doc enthält das LibDriver STS3X-Offlinedokument.

/Datenblatt enthält STS3X-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

#### example basic

```C
#include "driver_sts3x_basic.h"

uint8_t res;
uint8_t i;
float temperature;

res = sts3x_basic_init(STS3X_ADDRESS_0);
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    sts3x_interface_delay_ms(1000);
    res = sts3x_basic_read((float *)&temperature);
    if (res != 0)
    {
        (void)sts3x_basic_deinit();

        return 1;
    }
    sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature);
    
    ...
    
}

...

(void)sts3x_basic_deinit();

return 0;
```

#### example shot

```c
#include "driver_sts3x_shot.h"

uint8_t res;
uint8_t i;
float temperature;

res = sts3x_shot_init(STS3X_ADDRESS_0);
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    sts3x_interface_delay_ms(1000);
    res = sts3x_shot_read((float *)&temperature);
    if (res != 0)
    {
        (void)sts3x_shot_deinit();

        return 1;
    }
    sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature);
    
    ...
    
}

...

(void)sts3x_shot_deinit();

return 0;
```

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/sts3x/index.html](https://www.libdriver.com/docs/sts3x/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.