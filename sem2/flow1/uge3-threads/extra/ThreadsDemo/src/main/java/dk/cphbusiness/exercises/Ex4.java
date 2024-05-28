package dk.cphbusiness.exercises;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

/**
 * Ex4
 */
public class Ex4 {
    public static void main(String[] args) {
        int n = (args.length > 1) ? Integer.parseInt(args[1]) : 40;
        System.out.println("Calculating the " + n + "th fibonacci number...");
        int cores = Runtime.getRuntime().availableProcessors();
        System.out.println("Main Starting..." + System.lineSeparator() + "\tCores : " + cores);
        ExecutorService jobqueue = Executors.newFixedThreadPool(cores);
        for (int i = 0; i < cores; i++) {
            jobqueue.submit((() -> System.out.println(
                    n + "th fibonacci number is : "
                            + fibonacci.nthfib(n))));
        }
        jobqueue.shutdown();
        try {
            jobqueue.awaitTermination(20, TimeUnit.SECONDS);
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}

class fibonacci {
    public static int nthfib(int n) {
        return (n <= 1) ? 1 : nthfib(n - 1) + nthfib(n - 2);
    }
}

class test {
    public static void printsomething() {
        System.out.println("smth");
    }
}
