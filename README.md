# Operativsystem 2022: Datorövning 3

Den grundläggande koden och testerna finns i repositoryn från början. Du får skapa egna filer, men huvudfilerna (där `main`-funktionen finns) ska heta `ovn*.c`, där * är uppgiftsnummern. Du får även skapa egna branches, men din lösning ska lämnas in på `main`-branchen.

## Automatiska tester
Uppgiften kommer till viss del att bedömas automatiskt beroende på om de program du gjort uppfyller vissa test. Testerna finns i en gitsubmodul i katalogen test. Ifall testerna har uppdaterats (t.ex. om det varit någon bugg i något av testerna) kör följande kommando för att uppdatera till senaste version: 
```
~ git submodule update --remote
```
Det lönar sig att köra det här programmet före du skickar in uppgiften för att verifiera att du har senaste test-versionen.

## Inlämning
Den version du har på `main`-branchen då tiden gått ut är den som bedöms. Ändringar som görs senare beaktas nödvändigtvis inte.

Före inlämning se till att du har en fil `student.txt` som innehåller en rad enligt följande mönster:

```git_användar_namn ditt hela namn```

Du får inte använda extern kod (förutom standard C-bibliotek) om det inte specifikt nämns i uppgiftstexten.

**Att göra egna modifikationer i filerna i test-katalogen (eller .gitmodules-filen) räknas som fusk!** 

## Bedömning 
- Om ditt program inte kompilerar får du noll poäng för den uppgiften. 
- Den automatiska bedömningen som görs är nödvändigtvis inte slutgiltig. 
- Automatisk plagiatgranskning kan köras på koden.

## Bakgrundsinfo
- Tutorial för Posix trådar: https://randu.org/tutorials/threads/ 

## Uppgift 1. Kritiska sektioner och kapplöpning (max 10 p)

Undersök effekten av kritiska sektioner i programmering. Använd koden i `ovn1.c` som startpunkt. Din rapport (alltså svaren på frågorna nedan) ska skrivas i markdown format i en fil `ovn1_rapport.md` i repositoryn. 

Kompilera och kör det givna programmet i `ovn1.c`. Observera att det även finns en header-fil `ovn1.h` där parametrar definieras (används för de automatiska testen). För att kompilera program som använder sig av POSIX threads (pthreads) måste man ge `-pthread` flaggan åt kompilatorn. Denna flagga aktiverar andra flaggor för både preprocessorn och länkaren. Exempel:
```
$ gcc -pthread -o ovn1 ovn1.c
```
Kompileringen görs automatiskt om du använder:
```
$ make ovn1
```

## Är programmet deterministiskt?

**Fråga 1** Är det ursprungliga programmet i `ovn1.c` deterministiskt? Motivera svaret. **1 p.**

**Fråga 2** Då du kör programmet, stöder det dina slutsatser i punkt 1 ovan? **1 p.**


## Införande av kritisk sektion

Inkludera följande makro i filen 
`ovn1.h`:
```
#define UPDATE_COUNT 1000
```
och definiera följande globala variabel i `ovn1.c`:
```
int count;
```
Lägg även till en loop i trådfunktionen `print_hello()`:
```
for (i = 0; i < UPDATE_COUNT; i++) 
{
    tmp = count; 
    count = tmp + 1;
}
```
Definiera variabler enligt behov så koden fungerar.  

**Fråga 3**  Varför bildar denna loop en kritisk sektion i koden? **1 p.**

## Införande av barriär

Skapa en barriär som gör att programmet inte avslutas *förrän alla trådar kört färdigt*. Detta kan göras genom att använda `pthread_join()`. 

Om allt "skulle gå rätt",  skulle alltså 
```
count == UPDATE_COUNT * N_PTHREADS
```
men i praktiken är resultatet ofta mindre. 

**Fråga 4** Förklara sambandet mellan `count`, `UPDATE_COUNT` och `N_PTHREADS` och varför `count` inte alltid får rätt värde till slut. **1 p.**

Skriv ut en rapport före ditt program terminerar som jämför det slutgiltiga värdet på `count` och vad värdet egentligen borde ha varit, dvs `UPDATE_COUNT * N_PTHREADS`, samt förhållandet mellan dessa i procent. **3 p.**

Exempel på output
```
count = 154604928, should be 705032704, percentage 21.928760%
```

**Fråga 5** Ett dåligt barriäralternativ istället för `pthread_join` är att låta programmet sova en viss tid med `sleep`-kommandot. Varför är inte detta en bra idé? **1 p.**


## Införande av mutex

Använd mutex i `print_hello()`-funktionen för att göra att `count`-variabeln alltid får rätt värde, dvs. att endast en tråd åt gången kan köra loopen där `count` modifieras. Obs! Det finns färdigt inbygga funktioner för detta i `pthread`-biblioteket. **2 p.**

## Automatiska tester

Det är svårt att testa att program med flera trådar är skrivna på rätt sätt och fungerar korrekt. Därför finns det bara en begränsad mängd uppsättning tester för denna uppgift och största delen av bedömningen görs manuellt. I princip kontrollerar testen bara att mutexen i sista delen av uppgiften ger rätt resultat och att rätt returkod returnerats. 

Följande test finns:
```
$ make test-ovn1a
```
som första testar med 
```
#define N_PTHREADS (100)
#define UPDATE_COUNT (100)
```
och 
```
$ make test-ovn1b
```
som första testar med 
```
#define N_PTHREADS (5000)
#define UPDATE_COUNT (10000)
```

## Uppgift 2. Flertrådigt program för statistiska beräkningar (max 10 p)

Skriv ett flertrådigt program som beräknar olika statistiska värden för en lista med heltal. 

Indata till programmet är en uppsättning av nummer på kommandoraden. Sedan skapar programmen en tråd per beräkning av följande:
1. medelvärdet 
2. maxvärdet
3. minvärdet.

Första tråden ska alltså beräkna medelvärdet av talen, den andra maxvärdet, och den tredje minvärdet. 
Arbetartrådarna ska beräkna dessa värden, och den överordnade tråden kommer att skriva ut rapporten enligt nedan när arbetarna har avslutats. Observera att medelvärdet inte nödvändigtvis är ett heltal. 

Anta till exempel att ditt program körs med följande indata:
```
./ovn2 90 81 78 95 79 72 85
```
Programmet ska då rapportera
```
The average value is 82.857143
The minimum value is 72
The maxiumum value is 95
```
samt returnera `0` för att indikera att programmet avslutats korrekt. 

En prototyp finns i filen `ovn2.c`. Enklast är att lagra variablerna som representerar medel-, maximi- och minimivärdet globalt. Det finns en global konstant `MAX_LENGTH` som har värdet 100 som anger hur många tal som maximalt kan läsas in. Lösningen kräver alltså inte (nödvändigtvis) någon dynamisk minnesallokering utan vi kan anta att det ges in som mest 100 tal. Men, det ska gå att ändra `MAX_LENGTH` *i koden* till ett annat värde så din lösning ska inte vara hårdkodad till att max ta in 100 tal.