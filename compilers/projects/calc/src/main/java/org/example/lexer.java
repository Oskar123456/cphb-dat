package org.example;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.LinkedList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class lexer {
    static private Pattern letters[] = {Pattern.compile("[a-zA-Z_]"), Pattern.compile("[^a-zA-Z_]")};
    static private Pattern digits[] = {Pattern.compile("[0-9]"), Pattern.compile("[^0-9]")};
    static private Pattern ops[] = {Pattern.compile("[+\\-\\*/%=]"), Pattern.compile("[^+\\-\\*/%=]")};

    String wordType;

    LinkedList<token> tokens;
    int tokensCurIndx;

    public lexer() {
        tokens = new LinkedList<token>();
        tokensCurIndx = 0;
        wordType = "none";
    }

    public boolean lex(BufferedReader textBuffer) throws IOException {
        StringBuilder curWord = new StringBuilder();
        boolean read = true;
        int i = 0;
        char c;
        while (true) {
            if (read)
                i = textBuffer.read();
            c = (char) i;
            Matcher matchLetter = letters[0].matcher(String.valueOf(c));
            Matcher matchDigits = digits[0].matcher(String.valueOf(c));
            Matcher matchOps = ops[0].matcher(String.valueOf(c));
            if (curWord.length() == 0) {
                wordType = "none";
                if (matchLetter.matches())
                    wordType = "id";
                if (matchDigits.matches())
                    wordType = "digit";
                if (matchOps.matches())
                    wordType = "op";
                read = true;
            }
            if (wordType.equals("id")) {
                if (matchLetter.matches() || matchDigits.matches())
                    curWord.append(c);
                else {
                    tokens.add(new token(curWord.toString(), tokensCurIndx++));
                    curWord.setLength(0);
                    read = false;
                }
            }
            if (wordType.equals("digit")) {
                if (matchDigits.matches())
                    curWord.append(c);
                else {
                    tokens.add(new token(curWord.toString(), 0));
                    curWord.setLength(0);
                    read = false;
                }
            }
            if (wordType.equals("op")) {
                curWord.append(c);
                tokens.add(new token(curWord.toString(), 0));
                curWord.setLength(0);
            }
            if (i < 1)
                break;
        }

        return true;
    }

    public LinkedList<token> getTokens() {
        return tokens;
    }

}
