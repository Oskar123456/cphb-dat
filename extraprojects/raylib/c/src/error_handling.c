void
system_error()
{
    fprintf(stderr, "%s\n", strerror(errno));
}
