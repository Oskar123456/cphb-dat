package dk.cphbusiness.exercises;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class Ex2 {
    public static void main(String[] args) {
        int timesToCount = 5;
        int numbertoCountTo = 10000;
        int cores = Runtime.getRuntime().availableProcessors();
        System.out.println("Main Starting..." + System.lineSeparator() + "\tCores : " + cores);
        for (int i = 0; i < timesToCount; i++) {
            countToN(numbertoCountTo, cores, false);
            counter.reset();
        }
        for (int i = 0; i < timesToCount; i++) {
            countToN(numbertoCountTo, cores, true);
            counter.reset();
        }
        System.out.println("Main is done...");
    }

    private static void countToN(int n, int cores, boolean synchronize) {
        ExecutorService jobqueue = Executors.newFixedThreadPool(cores);
        for (int i = 0; i < 10000; i++) {
            jobqueue.submit(new worker(synchronize));
        }
        jobqueue.shutdown();
        /* remember to await termination, otherwise things can go wrong :) */
        try {
            jobqueue.awaitTermination(10, TimeUnit.SECONDS);
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
        String isItSync = (synchronize) ? "synchronized" : "unsynchronized";
        System.out.println("Counting (" + isItSync + ") is done..."
                + System.lineSeparator() + "\t Result: " + counter.getCount());

    }
}

class worker implements Runnable {
    boolean sync;

    public worker(boolean arg) {
        sync = arg;
    }

    @Override
    public void run() {
        if (sync)
            counter.incrementSync();
        else
            counter.increment();
    }
}

class counter {
    private static int count = 0;

    public static /* synchronized */ void increment() {
        count++;
    }

    public static synchronized void incrementSync() {
        count++;
    }

    public static int getCount() {
        return count;
    }

    public static void reset() {
        count = 0;
    }
}
