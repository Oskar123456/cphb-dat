package app.entities;

public record Task(int taskId, String title, Boolean done, int userId) {}