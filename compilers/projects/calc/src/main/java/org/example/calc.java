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
    }
}

class lexer {
    static private Pattern letters[] = { Pattern.compile("[a-zA-Z_]"), Pattern.compile("[^a-zA-Z_]") };
    static private Pattern digits[] = { Pattern.compile("[0-9]"), Pattern.compile("[^0-9]") };
    static private Pattern ops[] = { Pattern.compile("[+\\-\\*/%=]"), Pattern.compile("[^+\\-\\*/%=]") };

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

class syntaxtree {
    public syntaxtree lc;
    public syntaxtree rc;

    public token tok;

    public syntaxtree() {}

    public static syntaxtree buildtree(LinkedList<token> tokens) {
        return parseExpr(tokens, 0, tokens.size());
    }

    public static syntaxtree buildtree(lexer lxr) {
        return parseExpr(lxr.tokens, 0, lxr.tokens.size());
    }

    private static syntaxtree parseExpr(LinkedList<token> tokens, int start, int end) {
        syntaxtree ret = null;
        for (int i = start; i < end; i++) {
            if (tokens.get(i).name.equals("-") || tokens.get(i).name.equals("+")) {
                // System.out.println("parseExpr found " + tokens.get(i).name);
                ret = new syntaxtree();
                ret.tok = new token(tokens.get(i).name, 0);
                ret.lc = parseExpr(tokens, 0, i);
                ret.rc = parseTerm(tokens, i + 1, end);
            }
        }
        if (ret == null)
            ret = parseTerm(tokens, start, end);
        return ret;
    }

    private static syntaxtree parseTerm(LinkedList<token> tokens, int start, int end) {
        syntaxtree ret = null;
        for (int i = start; i < end; i++) {
            if (tokens.get(i).name.equals("*") || tokens.get(i).name.equals("/")) {
                // System.out.println("parseTerm found " + tokens.get(i).name);
                ret = new syntaxtree();
                ret.tok = new token(tokens.get(i).name, 0);
                ret.lc = parseTerm(tokens, 0, i);
                ret.rc = parseFactor(tokens, i + 1, end);
            }
        }
        if (ret == null)
            ret = parseFactor(tokens, start, end);
        return ret;
    }

    private static syntaxtree parseFactor(LinkedList<token> tokens, int start, int end) {
        syntaxtree ret = null;
        if ((end - start) == 1){
            // System.out.println("parseFactor found " + tokens.get(start).name);
            ret = new syntaxtree();
            ret.tok = new token(tokens.get(start).name, 0);
        }
        return ret;
    }

    // public void print(int depth){
    //     if (lc != null)
    //         lc.print(depth + 1);
    //     else
    //         System.out.println("   ".repeat(depth + 1) + "n");
    //     System.out.println("   ".repeat(depth) + tok.name);
    //     if (rc != null)
    //         rc.print(depth + 1);
    //     else
    //         System.out.println("   ".repeat(depth + 1) + "n");
    // }
    public String traversePreOrder(syntaxtree root) {

        if (root == null) {
            return "";
        }

        StringBuilder sb = new StringBuilder();
        sb.append(root.tok.name);

        String pointerRight = "└──";
        String pointerLeft = (root.rc != null) ? "├──" : "└──";

        traverseNodes(sb, "", pointerLeft, root.lc, root.rc != null);
        traverseNodes(sb, "", pointerRight, root.rc, false);

        return sb.toString();
    }

    public void traverseNodes(StringBuilder sb, String padding, String pointer, syntaxtree node,
                              boolean hasRightSibling) {
        if (node != null) {
            sb.append("\n");
            sb.append(padding);
            sb.append(pointer);
            sb.append(node.tok.name);

            StringBuilder paddingBuilder = new StringBuilder(padding);
            if (hasRightSibling) {
                paddingBuilder.append("│  ");
            } else {
                paddingBuilder.append("   ");
            }

            String paddingForBoth = paddingBuilder.toString();
            String pointerRight = "└──";
            String pointerLeft = (node.rc != null) ? "├──" : "└──";

            traverseNodes(sb, paddingForBoth, pointerLeft, node.lc, node.rc != null);
            traverseNodes(sb, paddingForBoth, pointerRight, node.rc, false);
        }
    }

    public void print() {
        System.out.print(traversePreOrder(this));
    }


}

class parser {

    // expr -> epxr + term | expr - term | term
    // term -> term * factor | term / factor | factor
    // factor -> digit | (expr)

}
