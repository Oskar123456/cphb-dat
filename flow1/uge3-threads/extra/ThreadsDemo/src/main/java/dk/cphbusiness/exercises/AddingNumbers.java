package dk.cphbusiness.exercises;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class AddingNumbers {

    public static void main(String[] args) {
        ExecutorService workingJack = Executors.newFixedThreadPool(17);
        System.out.println("Main starts");
        IntegerList integerList = new IntegerList();
        for (int count = 0; count < 1000; count++) {
            workingJack.submit(new TaskToAddCount(integerList, count));
        }
        System.out.println("Main is done");
        workingJack.shutdown();
    }

    private static class IntegerList {
        private static List<Integer> list = new ArrayList<>();

        public synchronized void addCount(int count) {
            list.add(count);
            System.out.println("Task: " + count + ": List size = " + list.size());
        }
    }

    private static class TaskToAddCount implements Runnable {
        // Gets a reference to the shared list and the count to add
        private IntegerList integerList;
        private int count;

        TaskToAddCount(IntegerList integerList, int count) {
            this.integerList = integerList;
            this.count = count;
        }

        @Override
        public void run() {
            try {
                Thread.sleep((int) Math.random() * 800 + 200);
                integerList.addCount(count);
            } catch (InterruptedException ex) {
                System.out.println("Thread was interrupted");
            }
        }
    }
}
