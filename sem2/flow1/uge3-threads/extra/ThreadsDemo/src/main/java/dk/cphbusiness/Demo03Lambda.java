package dk.cphbusiness;

/**
 * Purpose: To demonstrate the use of lambda expressions (replacing the need for explicit written classes)
 * Author: Thomas Hartmann
 */
public class Demo03Lambda {

    public static void main( String[] args ) throws Exception {
        System.out.println( "Starting main thread");
        Runnable myTask = () -> {
            String[] strings = { "String 01", "String 02", "String 03", "String 04" };
            for ( String str : strings ) {
                System.out.println( str );
                try {
                    Thread.sleep( 1000 );
                } catch ( InterruptedException ex ) {
                    System.out.println( "I was interrupted" );
                }
            }
        };

        Thread t = new Thread( myTask);
        System.out.println( "Starting one new thread" );
        t.start();
//        t.join();
        Thread.sleep( 3000 );
        t.interrupt();
        System.out.println( "Main thread is done" );
    }
}
