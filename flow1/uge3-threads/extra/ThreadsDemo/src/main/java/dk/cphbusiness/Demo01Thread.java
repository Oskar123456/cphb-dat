package dk.cphbusiness;

/**
 * Purpose: Show how to make a thread and start it by overriding the run method
 * Disadvantage: The thread class is now tied to the task (not good if we want to reuse the thread class)
 * Author: Thomas Hartmann
 * */
public class Demo01Thread {

    public static void main( String[] args ) throws Exception{
        System.out.println( "Starting main thread");
        String[] strings = { "String 01", "String 02", "String 03", "String 04", "String 05" };
        // New thread by making an anonymous subclass of Thread
        Thread t = new Thread() {
            @Override
            public void run() {
                for ( String str : strings ) {
                    System.out.println( str );
                    try {
                        Thread.sleep( 1000 ); // Simulate that the thread is doing some work
                    } catch ( InterruptedException ex ) {
                        System.out.println( "I was interrupted" );
                    }
                }
            }
        };
        System.out.println( "New Thread is being started" );
        t.start();
//        t.join(); // Insert this line to make the main thread wait for t to finish
        System.out.println( "Main thread is done" );
    }

}
