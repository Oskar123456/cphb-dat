/* 
 *
 * WINDOW/GFX etc. FUNCTIONS 
 *
 * */

static bool _win_paused;

static u32 _win_scr_width;
static u32 _win_scr_height;

static bool _win_cursor_first_time = true;
static float _win_cursor_last_x;
static float _win_cursor_last_y;

void
winGetScrDims(u32 *width, u32 *height)
{
    *width = _win_scr_width;
    *height = _win_scr_height;
}

float
winGetScrAspect()
{
    return (float)_win_scr_width / (float)_win_scr_height;
}

bool
winIsPaused()
{
    return _win_paused;
}

void
framebuffer_size_callback(GLFWwindow *win, int width, int height)
{
    _win_scr_width = width;
    _win_scr_height = height;
    glViewport(0, 0, width, height);
}


GLFWwindow*
winInit(int width, int height, int gl_ver_maj, int gl_ver_min)
{
    _win_scr_width = width;
    _win_scr_height = height;
    _win_cursor_last_x = width / 2.0;
    _win_cursor_last_y = height / 2.0;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = 
        glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return NULL;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    return window;
}
