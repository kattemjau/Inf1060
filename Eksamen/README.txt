README:
Kompilerer og kjører programmet med: make
Kan eventuelt bare kompilere programmet med: make compile
make, kjører både compile og deretter run.

make run, kjører programmet med default settings.
Disse er localhost med port 7777 for client med job linus.job
Programmene kan kjøres manuelt med ./server alice.job 7777
for server.c hvis du er i mappen /Server/
klient.c kan startes med ./client localhost 7777
i mappen /Klient/
Kan eventuelt kjøres med /Klient/client localhost 7777

server.c programmet ligger i Server mappen, og klient.c programmet ligger i Klient.

Bruker usleep for å time parrent med child prosesser. Child prosessen printer senere enn parrent prosessen grunnet cpu scheduling.
Dette kunne vært fikset med å implimentere en cpu scheduler, eller en verifikasjon fra child prosessen,
at den er ferdig med å printe ut innholdet av meldingen, slik at parrenten kan fortsette.

main ligger øverst i programmet, rett under deklarasjonen av de følgende funksjonene.

Koden min er basert på plenumstimer og forelesning. Har også brukt google som kilder.
