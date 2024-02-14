package dk.cphbusiness;


/**
 * Purpose: To demonstrate a deadlock when two threads are waiting for each other to release a lock
 * Author: Thomas Hartmann
 */
public class Demo11Deadlock {
    public static void main(String[] args) {
        Resource resource = new Resource();

        // Create two threads causing a potential deadlock
        Thread thread1 = new Thread(() -> resource.method1(), "Thread1"); // method1 locks lock1 and then lock2
        Thread thread2 = new Thread(() -> resource.method2(), "Thread2"); // method2 locks lock2 and then lock1

        // Start the threads
        thread1.start();
        thread2.start();
    }
}

class Resource {
    private final Object lock1 = new Object(); //
    private final Object lock2 = new Object();

    public void method1() {
        synchronized (lock1) {
            System.out.println(Thread.currentThread().getName() + " acquired lock1");

            // Simulate some processing time
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            synchronized (lock2) {
                System.out.println(Thread.currentThread().getName() + " acquired lock2");
            }
        }
    }

    public void method2() {
        synchronized (lock2) {
            System.out.println(Thread.currentThread().getName() + " acquired lock2");

            // Simulate some processing time
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            synchronized (lock1) {
                System.out.println(Thread.currentThread().getName() + " acquired lock1");
            }
        }
    }
}
