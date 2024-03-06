package org.example;

import java.beans.beancontext.BeanContextSupport;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
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
    public static void main(String[] args) throws IOException {
        if (args.length < 1) {
            System.out.println("usage: $ calc <file path>");
            return;
        }
        System.out.println("attempting to open " + args[0]);
        BufferedReader filebuf = new BufferedReader(new FileReader(args[0]));
        if (!filebuf.ready()) {
            System.out.println("file not found");
            filebuf.close();
            return;
        }
        lexer lxr = new lexer();

        lxr.lex(filebuf);
        System.out.println("Lexer tokens:" + System.lineSeparator() + lxr.getTokens().toString());
        filebuf.close();

        syntaxtree stxt = syntaxtree.buildtree(lxr);
        stxt.print();
        System.out.println();

        System.out.println("Result: " + evaluator.evalCalc(stxt));
    }
}

class token {
    String name;
    int pos;

    public token(String n, int p) {
        name = n;
        pos = p;
    }

    public String toString() {
        return "[" + name + ", " + pos + "]";
    }
}

class evaluator {
    public static double evalCalc(syntaxtree tree){
        if (tree == null)
            return 0.0;
        switch (tree.tok.name) {
        case "+":
            return evalCalc(tree.lc) + evalCalc(tree.rc);
        case "-":
            return evalCalc(tree.lc) - evalCalc(tree.rc);
        case "*":
            return evalCalc(tree.lc) * evalCalc(tree.rc);
        case "/":
            return evalCalc(tree.lc) / evalCalc(tree.rc);
        default:
            try {return Double.parseDouble(tree.tok.name);}
            catch (NumberFormatException e) {e.printStackTrace();}
        }
        return 0.0;
    }
}
