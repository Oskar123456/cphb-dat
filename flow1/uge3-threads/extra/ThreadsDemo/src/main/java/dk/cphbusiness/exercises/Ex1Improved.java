package dk.cphbusiness.exercises;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class Ex1Improved {
    public static void main(String[] args) {
        ExecutorService jobqueue = Executors.newFixedThreadPool(4);
        List<Future<String>> futList = new ArrayList<>();
        for (char c = 'A'; c <= 'Z'; c++) {
            Callable<String> task = new stringPrinterCallable(c);
            Future<String> fut = jobqueue.submit(task);
            futList.add(fut);
        }
        for (Future<String> f : futList) {
            try {
                System.out.println(f.get()); // get() blocks until result is available
            } catch (Exception ex) {
                System.out.println(ex.getMessage());
            }
        }
        System.out.println("Main is done");
        jobqueue.shutdown();

    }
}

class stringPrinterCallable implements Callable<String> {
    private final char c;

    public stringPrinterCallable(char arg) {
        c = arg;
    }

    @Override
    public String call() throws Exception {
        return String.valueOf(c).repeat(3);
    }
}
