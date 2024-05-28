package dk.cphbusiness;


/**
 * Purpose: Show how to make a thread by giving it an instance of a class that implements Runnable
 * Now we can reuse the thread runnable code
 * Author: Thomas Hartmann
 * */
public class Demo02Runnable {

    public static void main( String[] args ) throws Exception {
//        New thread with runnable
        Thread t = new Thread( new StringPrinterTask() );
        System.out.println( "Starting new thread" );
        t.start();
//        t.join();
        Thread.sleep( 3000 );
        t.interrupt();
        System.out.println( "Main thread is done" );
        Thread t2 = new Thread(System.out::println);
    }
}

class StringPrinterTask implements Runnable {
    @Override
    public void run() {
        String[] strings = { "String 01", "String 02", "String 03", "String 04", "String 05" };
        for ( String str : strings ) {
            System.out.println( str );
            try {
                Thread.sleep( 1000 );
            } catch ( InterruptedException ex ) {
                System.out.println( "I was interrupted" );
            }
        }
    }
}
