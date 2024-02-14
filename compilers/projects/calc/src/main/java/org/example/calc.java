package org.example;

import java.beans.beancontext.BeanContextSupport;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.stream.Stream;

// calculator:
//
// left-associative: + -
// left-associative: * / (higher presedence)
//
// factor -> digit | (expr)
//
// term -> term * factor
// -> term / factor
// -> factor
//
// expr -> expr + term
// -> expr - term
// -> term
//
// Hence:
//
// expr -> expr + term | expr - term | term
// term -> term * factor | term / factor | factor
// factor -> digit | (expr)

public class calc {
    public static void main(String[] args) {
        lexer lex = new lexer(System.in);
        try {
            lex.lex();
        } catch (Exception e) {
            // TODO: handle exception
        }
    }
}

class lexer {
    BufferedReader in;
    HashMap<String, Integer> tokens;

    public lexer(InputStream arg) {
        in = new BufferedReader(new InputStreamReader(arg));
    }

    public boolean lex() throws IOException {
        char c;
        while (isValid(c = (char) in.read())) {

        }
    }

    private boolean isValid() {
        return true;
    }

    private boolean isDigit() {
        return true;
    }

    private boolean isOperator() {
        return true;
    }
}
