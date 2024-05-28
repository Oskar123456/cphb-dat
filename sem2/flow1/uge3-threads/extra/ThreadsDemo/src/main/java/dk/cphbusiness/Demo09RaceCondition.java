package dk.cphbusiness;

/**
 * Purpose of this demo is to show the problem with 'race condition' when two threads access the same variable
 * Author: Thomas Hartmann
 */
public class Demo09RaceCondition {
    public static void main(String[] args) throws InterruptedException {
        // A single instance of the counter is used in 2 threads
        Counter counter = new Counter();

        // Create two threads that concurrently increment the counter
        Count1000Times thread1 = new Count1000Times(counter);
        Count1000Times thread2 = new Count1000Times(counter);

        thread1.start();
        thread2.start();

        // Wait for both threads to finish
        thread1.join();
        thread2.join();

        // Print the final count value (may vary due to race condition)
        System.out.println("Final Count: " + counter.getCount());
    }
    private static class Counter {
        private int count = 0;

        public void increment() {
            // Get the value of count
            int temp = count;
            // Simulate some processing time
            try { Thread.sleep(20); } catch (InterruptedException e) { e.printStackTrace(); }
            // Set vaule of count to old value + 1
            count = temp + 1;
        }

        public int getCount() {
            return count;
        }
    }

    private static class Count1000Times extends Thread {
        // Take a reference to a Counter instance and increment it 1000 times
        private Counter counter;

        public Count1000Times(Counter counter) {
            this.counter = counter;
        }

        @Override
        public void run() {
            for (int i = 0; i < 1000; i++) {
                counter.increment();
                System.out.println("Thread: " + this.getName() + " Count: " + counter.getCount());
            }
        }
    }

}

