package org.example;


import java.util.LinkedList;

class syntaxtree {
    public syntaxtree lc;
    public syntaxtree rc;

    public token tok;

    public syntaxtree() {
    }

    public static syntaxtree buildtree(LinkedList<token> tokens) {
        return parseExpr(tokens, 0, tokens.size());
    }

    public static syntaxtree buildtree(lexer lxr) {
        return parseExpr(lxr.tokens, 0, lxr.tokens.size());
    }

    private static syntaxtree parseExpr(LinkedList<token> tokens, int start, int end) {
        // expr -> epxr + term | expr - term | term
        syntaxtree ret = null;
        int split = 0;
        for (int i = start; i < end; i++){
            if (tokens.get(i).name.equals("+") || tokens.get(i).name.equals("-")){
                split = i; break;
            }
        }
        if (split > 0){
            ret = new syntaxtree();
            ret.tok = new token(tokens.get(split).name, 0);
            ret.lc = parseTerm(tokens, start, split);
            ret.rc = parseExpr(tokens, split + 1, end);
        }
        else {
            ret = parseTerm(tokens, start, end);
        }
        return ret;
    }

    private static syntaxtree parseTerm(LinkedList<token> tokens, int start, int end) {
        // term -> term * factor | term / factor | factor
        syntaxtree ret = null;
        int split = 0;
        for (int i = start; i < end; i++){
            if (tokens.get(i).name.equals("*") || tokens.get(i).name.equals("/")){
                split = i; break;
            }
        }
        if (split > 0){
            ret = new syntaxtree();
            ret.tok = new token(tokens.get(split).name, 0);
            ret.lc = parseFactor(tokens, start, split);
            ret.rc = parseTerm(tokens, split + 1, end);
        }
        else {
            ret = parseFactor(tokens, start, end);
        }
        return ret;
    }

    private static syntaxtree parseFactor(LinkedList<token> tokens, int start, int end) {
        // factor -> digit | (expr)
        syntaxtree ret = null;
        if (end - start == 1){
            ret = new syntaxtree();
            ret.tok = tokens.get(start);
        }
        return ret;
    }

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
