package app;

import app.config.ThymeleafConfig;
import app.entities.item;
import app.persistence.ConnectionPool;
import app.persistence.ItemMapper;
import io.javalin.Javalin;
import io.javalin.http.Context;
import io.javalin.rendering.template.JavalinThymeleaf;

import java.util.List;

import static app.persistence.ItemMapper.listAllItems;

public class Main {

    private static final String USER = "postgres";
    private static final String PASSWORD = "postgres";
    private static final String URL = "jdbc:postgresql://localhost:5432/%s?currentSchema=public";
    private static final String DB = "secondhand";

    private static final ConnectionPool connectionPool = ConnectionPool.getInstance(USER, PASSWORD, URL, DB);


    public static void main(String[] args)
    {
        // Initializing Javalin and Jetty webserver

        Javalin app = Javalin.create(config -> {
            config.staticFiles.add("/public");
            config.fileRenderer(new JavalinThymeleaf(ThymeleafConfig.templateEngine()));
        }).start(7070);

        // Routing

        app.get("/", Main::index);


        List<item> itemlist = ItemMapper.listAllItems();
        for (item i : itemlist){
            System.out.println(i.title + " : " + i.desc + " : " + i.price);
        }
    }

    public static void index(Context ctx){
        List<item> il = ItemMapper.listAllItems();
        ctx.sessionAttribute("itemList", il);
        ctx.render("index.html");
    }



}