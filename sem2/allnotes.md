## Frontend

##### web versioner

1.0 :
 - statisk, read-only.
 - Ren frontend.

2.0 :
 - interaktivt, bruger- defineret/skabt indhold (sociale medier).
 - Backend.

##### ux love

https://lawsofux.com/


lol


### LIFEHACK

##### board

 - Idea: Når du får en god ide opretter du opgaven i “Idea”, så kan du på dit næste teammøde præsentere ideen for dine kollegaer, hvor de andre kan være med til at vurdere, om det vil give værdi for virksomheden.

 - Start: Hvis din ide bliver godkendt, kan du trykke den over i “Start”, hvor den vil vente, indtil I har ressourcer til at kigge på den.

 - Doing: Når I har fået noget fra hånden, og “Doing” kolonnen er ved at være tom, så kan man vælge, hvilke kort der fra “Start” skal rykkes til “Doing”, som så bliver de opgaver, man skal kigge på den kommende tid.

 - Pending: En opgave rykkes til pending, når opgaven er færdig, og skal godkendes af en stakeholder, eller opgaven er stoppet midlertidigt grundet manglende input fra en stakeholder (internt eller eksternt) eller manglende ressourcer (penge, værktøjer, tid). Derefter rykkes den tilbage til “Doing”, hvis du stadig skal arbejde på den, efter du har fået de givne ressourcer.

 - Done: Når en opgave er helt færdig, og den er blevet godkendt, kan man derefter trække den over i “Done”.

##### projekt

###### Frisk Forslag

###### user story

 - kig i køleskab; indtast ingredienser i app; app'en spytter
 en liste ud med opskrifter, der indeholder 1 - n af ingredienserne.
 Muligvis sorteret på en smart måde, f.eks. efter hvor tæt opskriften
 ligger op ad brugerens indtastede værdier.

###### HVIS vi henter opskrifter fra en hjemmeside

https://opskrifter.coop.dk/opskrifter/stegte-sild-i-krydderlage-5836


# threads


    What is a thread?

Som et barn af en process, men som deler virtuelt adresserum. 

    What is the difference between a process and a thread?

^

    What is the purpose of the start method in a thread?

Eksekverer Run() metoden.

    What is the purpose of the run method in a thread?

udgangspunktet for en tråd.

    What is the purpose of the sleep method in a thread?



    What is the purpose of the join method in a thread?

Vente på at en tråd bliver færdig

    What is the purpose of the getName method in a thread?

At få trådens ID & måske også pool'en/ExecutorService'en den tilhører.

    What is a Callable?

En runnable, med returværdi samt evnen til at smide en exception.

    What is the purpose of the ExecutorService?

At uddele arbejde til de samme n tråde, så vi ikke bare instantiere tråde nonstop.

    What is the purpose of the submit method in the ExecutorService?

At smide et arbejde på køen / jobqueue'en.

    What is returned by the submit method in the ExecutorService?

En future, som er en type der fortløbende vil indeholde trådens resultater.

    What is a Future?

^. Et output object til en sideløbende tråd. Man kan bede om resultatet med get(), men 
dette kald blokerer. Dog kan man give et timeout-parameter, så man ikke behøver vente for evigt. 
isDone() blokerer ikke.

    What is the purpose of the get method in the Future?
    What is the purpose of the isDone method in the Future?
    What is the purpose of the awaitTermination method in the ExecutorService?

Venter på alle jobs er færdiggjorte. 

    What is the purpose of the newFixedThreadPool method in the Executors class?

Oprette en arbejdskø med et vist antal tråde til.

    What is the purpose of the newCachedThreadPool method in the Executors class?

I en cached threadpool skal jobkøen være tom, dvs. en ny tråd startes, hvis der ikke 
er frie hænder til at nyt arbejde.

    Why do we need 2 loops to get the result from the executor service?

Måske fordi trådene først skal startes, og derefter kan vi ligge main til at vente 
på resultater?



# Database

## normalisering

### normalformer

* **første normalform**:
  1. Der skal være en nøgle der entydigt identificerer
  en enkelte række i tabellen.
  2. De enkelte felter må kun indeholde en værdi (atomare værdier).
  3. Der må ikke være kolonner der gentages.

* **anden normalform**:
  1. Opfylder ovenstående
  2. Ingen attributter/egenskaber der ikke selv tilhører nøglen,
  må afhænge af en del af nøglen.
* **tredje normalform**:
  1. Opfylder ovenstående.
  2. Ingen attributter/egenskaber må afhænge af andre attributter,
  der ikke selv er nøgler.

## crow's foot notation - relationship symbols, read diagrams



