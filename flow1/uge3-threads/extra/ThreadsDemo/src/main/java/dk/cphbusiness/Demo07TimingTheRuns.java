package dk.cphbusiness;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Purpose of this demo is to show how to time the execution of a program
 * Author: Thomas Hartmann
 */
public class Demo07TimingTheRuns {

    public static void main( String[] args ) {
        System.out.println( "Starting main thread" );
        long startTime = System.currentTimeMillis();
        runningTasksConcurrently();
        long endTime = System.currentTimeMillis();
        System.out.println( String.format("Execution time for concurrent run was: %s milli seconds",endTime-startTime) );
        startTime = System.currentTimeMillis();
        runningTasksSequentially();
        endTime = System.currentTimeMillis();
        System.out.println( String.format("Execution time for sequential run was: %s milli seconds",endTime-startTime) );
        System.out.println( "Main thread is done" );
    }

    private static void runningTasksConcurrently() {
        // Using a cached thread pool, we dont have to worry about the number of threads, because it will create new threads as needed
        ExecutorService workingJack = Executors.newCachedThreadPool();
        for ( int count = 0; count < 50; count++ ) {
            Runnable task = new MyTask( count );
            workingJack.submit( task );
        }
        workingJack.shutdown();
    }
    private static void runningTasksSequentially() {
        // This method is faster, because we don't have the overhead of creating threads
        for ( int count = 0; count < 50; count++ ) {
            Runnable task = new MyTask( count );
            task.run();
        }
    }

}

class MyTask implements Runnable {

    private int count = 0;
    private int sleepTime = 0;

    MyTask( int cnt ) {
        count = cnt;
        sleepTime = (int) ( Math.random() * 500 + 500); // Sleep between 500 and 1000 milli seconds
    }

    @Override
    public void run() {
        System.out.println( "Task: " + count );
        try {
            Thread.sleep( sleepTime );
        } catch ( InterruptedException ex ) {
            System.out.println( "I was interrupted" );
        }
    }
}
