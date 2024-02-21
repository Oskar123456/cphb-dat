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

