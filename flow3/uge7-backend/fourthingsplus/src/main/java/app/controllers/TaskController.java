package app.controllers;

import app.entities.Task;
import app.entities.User;
import app.exceptions.DatabaseException;
import app.persistence.ConnectionPool;
import app.persistence.TaskMapper;
import io.javalin.Javalin;
import io.javalin.http.Context;
import org.jetbrains.annotations.NotNull;

import java.util.List;

public class TaskController {
    public static void addRoutes(Javalin app, ConnectionPool connPool) {
        app.post("addtask", ctx -> addTask(ctx, connPool));
    }

    private static void addTask(@NotNull Context ctx, ConnectionPool connPool) {
        String taskName = ctx.formParam("taskname");
        User user = ctx.sessionAttribute("currentUser");

        if (!(taskName == null) && !(user == null)) {
            try {
                TaskMapper.addTask(user, taskName, connPool);
            } catch (DatabaseException e) {
                ctx.attribute("addtaskmessage", "kunne ikke tilføje opgaven " + taskName);
            }
        }
        List<Task> taskList = null;
        try {
            taskList = TaskMapper.getAllTasksPerUser(user.id(), connPool);
        } catch (DatabaseException e) {
            ctx.attribute("addtaskmessage", "databasefejl (task)");
        }
        ctx.attribute("taskList", taskList);
        ctx.render("task.html");
    }

}
