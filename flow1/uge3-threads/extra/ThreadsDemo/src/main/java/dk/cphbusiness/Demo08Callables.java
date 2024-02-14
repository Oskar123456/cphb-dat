package dk.cphbusiness;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

/**
 * Purpose of this demo is to show how to use Callables and Futures to get a
 * return value from a task (and to be able to throw exceptions)
 * Author: Thomas Hartmann
 */
public class Demo08Callables {

    public static void main(String[] args) {
        System.out.println("Main starts.\nTasks will be executed in same sequence as entered");
        ExecutorService workingJack = Executors.newCachedThreadPool();
        List<Future<String>> stringFutureList = new ArrayList<>();
        for (int count = 1; count <= 100; count++) {
            Callable<String> task = new CallableTask(count);
            Future<String> fut = workingJack.submit(task);
            stringFutureList.add(fut);
        }
        for (Future<String> fut : stringFutureList) {
            try {
                System.out.println(fut.get()); // get() blocks until result is available
            } catch (Exception ex) {
                System.out.println("Exception: " + ex.getMessage());
            }
        }
        System.out.println("Main is done");
        workingJack.shutdown();

    }

}

class CallableTask implements Callable<String> {

    private int count = 0;

    CallableTask(int cnt) {
        count = cnt;
    }

    @Override
    public String call() throws Exception {
        Thread.sleep(1000);
        return "Task: " + count;
    }
}
