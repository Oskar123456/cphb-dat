package dk.cphbusiness;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Purpose of this demo is to show how to solve the problem with the final variable
 */
public class Demo06FinalError {

    public static void main( String[] args ) {
        ExecutorService workingJack = Executors.newSingleThreadExecutor();
        for ( int count = 0; count < 25; count++ ) {
            workingJack.submit( () -> {
                // Explain why this line is a problem (Remove the comment)
//                System.out.println( "Count is: "+count );
            } );
        }
        workingJack.shutdown();
    }
}
