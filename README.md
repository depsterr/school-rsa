Kompilering av program
======================

För att kompilera och köra programmet så behöver man köra en UNIX liknande miljö, vilket betyder att man kommer behöver använda sig av MinGW eller liknande för att kompilera på windows.

I unix miljö
------------

Se till att du har gcc, ghc, och make installerat.

Kör efter detta `make`.

I Windows miljö
---------------

Installera en MinGW unix miljö, öppna därefter en MinGW terminal i denna mapp och följ unix miljö instruktionerna.

Dokumentation
=============

Här kommer jag dokumentera all funktioner i mina olika filer, vad de gör och hur de gör det. Vissa filer har även kommentarer men jag har valt att skriva här också.

main.c
------

main.c är programmets startpunkt. (skriven i C)

### datastrukturer

pubkey : representerar en public key (n och e)
privkey :  representerar en private key (n och d)

keyset : representerar en pubkey och en privkey

### Funktioner

hack funktionen tar en pubkey och returnerar d. Detta görs genom att dividera nyckelns n med alla primtal upp till (n / 2) + 1. När den hittar en division som går jämt ut så vet den ju båda primtal och räknar då ut phi och kör findDHs funktionen från math.hs för att hitta d. Efter detta returneras d.

generate\_keyset funktionen tar inga argument och returnerar ett keyset. Den generarar detta genom att slumpa två primtals index (dvs, ett index i en lista av alla primtal). Programmet genererar sedan så många primtal som behövs och ger q och p sina värden. Efter detta så räknar den snabbt ut n och phi. Sedan så hittas e genom att pröva alla värden från och med två och uppåt tills gcd(phi, e) == 1. d genereras sedan med tidigare nämnda findDHs funktionen.

main() funktionen:

main funktionen är starten för vårt program och hanterar bara vår input och ser till att rätt funktion körs med rätt värden.

math.hs
-------

math.hs hanterar matte med stora nummer som C inte klarar av utan bibliotek och är skrivit i haskell, ett annat kompilerat språk som är otroligt snabb på matte.

Denna fil kan vara lite svår att förstå om man inte kan haskell, men jag har kommenterat alla funktioner i filen. Och kommer endast förklara rollen av funktionerna, inte hur det funkar.

Det viktigaste är att denna fil implementerar utökade euklides algorithm, vilket är det funktionen findDHs använder sig av, samt att den implementerar rsaCryptHs vilket är den upphöjt i modulo som används för att faktiskt kryptera och avkryptera.

prim.c
------

prim.c ansvarar för generation av primtal. (skriven i C)

den har två olika funktioner, en som genererar n antal primtal (get\_n\_primes) och en som genererar primtal tills de blir störra än ett tal (get\_primes\_to). Dock funkar dessa nästan lika dant så jag kommer endast dokumentera de en gång.

Den skapar först en ny lista och lägger in 2 och 3, sedan så börjar den loopa över alla tal från och med 5, den försöker sedan dividera dessa med alla primtal som redan finns i listan, tills det att primtalen i listan tar slut eller blir större än roten av talet. Detta är effektivt då vi bara dividerar med primtal istället för alla tal, då alla tal som inte är primtal kan primtals faktoriseras. Att avbryta vid roten ur primtalet är också användbart vid stora tal då vi redan skulle hittat en annan faktor vid det laget, och på så sett slipper kolla alla primtal vi har.

util.c
------

util.c har några diverse hjälp funktioner. (skriven i C)

### Funktioner

die funktionen printar ett medellande och stoppar programmet

pdie funktionen printar ett medellande och stoppar programmet med standardbibliotekets senaste error medellande

xmalloc är en wrapper kring malloc, en funktion för att allokera minne till processen. xmalloc stoppar programmet om det inte går att allokera mer minne, till skillnad från malloc.

xrealloc är en wrapper kring realloc, en funktion för att omallokera minne i processesen (t.ex minska eller öka en buffert). xrealloc stoppar programmet om det inte gick att omallokera minnet, till skillnad från realloc.

fast\_sqrt är en funktion som estimerar roten ur ett tal genom att använda sig av newtons metod. Anledningen till att denna används istället för en precis funktion är att vi endast behöver heltal och att den måste gå mycket snabbt då den kallas för alla tal i vår primtals generations loop.

gcd är en funktion som returnerar gcd för två givna tal genom att använda sig av euklides metod.

vec.c
-----

vec.c implementerar den lista jag använder mig av. (skriven i C)

Jag kommer inte gå igenom denna fil i detalj då det ändast rör sig om minnes hantering i C.
