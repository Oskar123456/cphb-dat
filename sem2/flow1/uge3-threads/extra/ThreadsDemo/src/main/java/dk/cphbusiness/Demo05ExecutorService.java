package dk.cphbusiness;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Purpose of this demo is to show how to use an ExecutorService with a single thread
 */
public class Demo05ExecutorService {

    public static void main( String[] args ) {
        ExecutorService workingJack = Executors.newSingleThreadExecutor();
        System.out.println( "Creates tasks for workingJack" );
        for ( int i = 0; i < 5; i++ ) {
            workingJack.submit( new Runnable() {
                @Override
                public void run() {
                    System.out.println( "Hello to us" );
                }
            } );
        }
        System.out.println( "Waits for workingJack to finish" );
        workingJack.shutdown();
        System.out.println( "All done" );
    }
}
