package dk.cphbusiness;

/**
 * Purpose of this demo is to show how to use synchronized methods to ensure thread safety
 * Author: Thomas Hartmann
 */
public class Demo10ThreadsSyncSafe {
    public static void main(String[] args) throws InterruptedException {
        // Create a shared Counter instance
        Counter counter = new Counter();

        // Create two threads that increment the counter
        CounterThread thread1 = new CounterThread(counter);
        CounterThread thread2 = new CounterThread(counter);

        // Start both threads
        thread1.start();
        thread2.start();

        // Wait for both threads to finish
        thread1.join();
        thread2.join();

        // Print the final count value
        System.out.println("Final Count: " + counter.getCount());
    }
    private static class Counter {
        private int count = 0;

        // Method to increment the count, synchronized to ensure thread safety
        public synchronized void increment() {
            count++;
        }

        // Method to retrieve the current count value
        public int getCount() {
            return count;
        }
    }

    private static class CounterThread extends Thread {
        private Counter counter;

        public CounterThread(Counter counter) {
            this.counter = counter;
        }

        @Override
        public void run() {
            for (int i = 0; i < 10000; i++) {
                // Increment the counter in a synchronized manner
                counter.increment();
            }
        }
    }
}

