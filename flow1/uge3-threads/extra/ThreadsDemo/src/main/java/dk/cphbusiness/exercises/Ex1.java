package dk.cphbusiness.exercises;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Ex1 {
    public static void main(String[] args) {
        ExecutorService jobqueue = Executors.newFixedThreadPool(4);
        char c = 'A';
        for (int i = 0; i < 26; i++) {
            jobqueue.submit(new stringPrinter(c));
            ++c;
        }
        jobqueue.shutdown();
    }
}

class stringPrinter implements Runnable {
    private final char c;

    public stringPrinter(char arg) {
        c = arg;
    }

    @Override
    public void run() {
        String str = String.valueOf(c).repeat(3);
        System.out.println(str);
    }
}
