package org.example;

import junit.framework.TestCase;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

/**
 * Unit test for simple App.
 */
public class AppTest
    extends TestCase
{
    App testApp;

    @Test
    @BeforeEach
    void init(){
        testApp = new App();
    }

    @Test
    @DisplayName("test add method")
    void add(){
        int a = 3;
        int b = 2;

        int result = testApp.add(a, b);
        int expected = a + b;

        assertEquals(result, expected);
    }

    @Test
    @DisplayName("Requirement 1")
    void Req1(){
        String name = "bob";
        String exp = "Hello, " + name + ".";

        String res = testApp.greet(name);

        assertEquals(exp, res);
    }
    @Test
    @DisplayName("Requirement 2")
    void Req2(){
        String exp = "Hello, my friend.";

        String res = testApp.greet(null);

        assertEquals(exp, res);
    }
    @Test
    @DisplayName("Requirement 3")
    void Req3(){
        String name = "JERRY";
        String exp = "HELLO " + name + ".";

        String res = testApp.greet(name);

        assertEquals(exp, res);
    }
    @Test
    @DisplayName("Requirement 4")
    void Req4(){
        String[] names = {"Jill", "Jane"};

        String exp = "Hello, " + names[0] + " and " + names[1] + ".";

        String res = testApp.greet(names);

        assertEquals(exp, res);
    }
    @Test
    @DisplayName("Requirement 5")
    void Req5(){
        String[] names = {"Amy", "Brian", "Charlotte"};

        String exp = "Hello, " + names[0] + ", " + names[1] + " and " + names[2] + ".";

        String res = testApp.greet(names);

        assertEquals(exp, res);
    }
    @Test
    @DisplayName("Requirement 6")
    void Req6(){
        String[] names = {"Amy", "BRIAN", "Charlotte"};

        String exp = "Hello, " + names[0] + " and " + names[2] + ". AND HELLO " + names[1] + ".";

        String res = testApp.greet(names);

        assertEquals(exp, res);
    }
    @Test
    @DisplayName("Requirement 7")
    void Req7(){
        String exp = "Hello, Bob, Charlie and Dianne.";

        String res = testApp.greet("Bob", "Charlie, Dianne");

        assertEquals(exp, res);
    }
    @Test
    @DisplayName("Requirement 8")
    void Req8(){
        String exp = "Hello, Bob and Charlie, Dianne.";

        String res = testApp.greet("Bob", "\"Charlie, Dianne\"");

        assertEquals(exp, res);
    }
}
