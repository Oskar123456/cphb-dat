package org.example;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Collections;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        System.out.println( "Hello World!" );

        App testApp = new App();

        String test = testApp.greet(null);
        System.out.println(test);

        test = testApp.greet("JERRY");
        System.out.println(test);
    }

    public App() {
    }

    public int add(int a, int b){
        return a + b;
    }

    private int numOfMatches(String needle, String haystack){
        String tempStr = haystack.replace(needle, "");
        return (haystack.length() - tempStr.length()) / needle.length();
    }

    private boolean isAllUpperCase(String str){
        return (str != null) && str.equals(str.toUpperCase());
    }

    public String greet(String... name){
        if (name == null)
            return "Hello, my friend.";

        String escapeChars = "\"";

        ArrayList<String> namesList = new ArrayList<String>();
        ArrayList<String> NAMESList = new ArrayList<String>();

        for (String tempStr : name) {
            ArrayList<String> targetList = (isAllUpperCase(tempStr)) ? NAMESList : namesList;
            if (tempStr.startsWith(escapeChars) && tempStr.endsWith(escapeChars)){
                targetList.add(tempStr.replace(escapeChars, ""));
            }
            else {
                tempStr = tempStr.replace(" ", "");
                String[] tempStrs = tempStr.split(",");
                Collections.addAll(targetList, tempStrs);
            }
        }

        String retval = "";
        boolean regularNamesExist = !namesList.isEmpty();

        for (int i = 0; i < namesList.size(); i++){
            if (i == 0)
                retval += "Hello, ";
            retval += namesList.get(i);
            if (i < namesList.size() - 2)
                retval += ", ";
            else if (i == namesList.size() - 2)
                retval += " and ";
            else
                retval += ".";
        }
        for (int i = 0; i < NAMESList.size(); i++){
            if (i == 0){
                if (!regularNamesExist)
                    retval += "HELLO ";
                else
                    retval += " AND HELLO ";
            }
            retval += NAMESList.get(i);
            if (i < NAMESList.size() - 2)
                retval += ", ";
            else if (i == NAMESList.size() - 2)
                retval += " AND ";
            else
                retval += ".";
        }

        return retval;
    }
}
