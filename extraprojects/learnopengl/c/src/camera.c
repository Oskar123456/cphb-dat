/*
 *
 * */
typedef enum CamDirection {
    DIR_FORWARD,
    DIR_BACKWARD,
    DIR_LEFT,
    DIR_RIGHT
} CamDirection;

typedef enum CamType {
    FLYING,
    FPS
} CamType;

typedef struct Camera {
    vec3s pos;
    vec3s dir;
    vec3s at;
    vec3s up;
    vec3s right;
    vec3s left;
    vec3s up_world;

    float yaw;   // around y-axis
    float pitch; // around x-axis
    bool  restrict_pitch;

    float speed;
    float speed_default;
    float speed_sprint;
    float sens;
    union {
        float zoom;
        float fov;
    };

    bool is_sprinting;

    float draw_dist;

    CamType type;
} Camera;

void
camGetView(Camera *c, mat4 *dest)
{
    glm_vec3_add(c->pos.raw, c->dir.raw, c->at.raw);
    glm_lookat(c->pos.raw, c->at.raw, c->up.raw, *dest);
}

mat4s
camGetViews(Camera *c)
{
    c->at = vec3_add(c->pos, c->dir);
    return glms_lookat(c->pos, c->at, c->up);
}

void
camLook(Camera *c, double x_offset, double y_offset)
{
//    printf("cam_update_dir: %f %f\n", x_offset, y_offset);

    c->yaw   += x_offset * c->sens;
    c->pitch += y_offset * c->sens;

    if (c->restrict_pitch){
        if (c->pitch >  89.0)
            c->pitch =  89.0;
        if (c->pitch < -89.0)
            c->pitch = -89.0;
    }

    vec3 dir, up, left, right;
    dir[0] = cos(glm_rad(c->yaw) * cos(glm_rad(c->pitch)));
    dir[1] = sin(glm_rad(c->pitch));
    dir[2] = sin(glm_rad(c->yaw) * cos(glm_rad(c->pitch)));
    glm_vec3_normalize(dir);
    
    glm_vec3_add(c->pos.raw, dir, c->at.raw);
    glm_vec3_copy(dir, c->dir.raw);

    glm_vec3_cross(c->dir.raw, c->up_world.raw, c->right.raw);
    glm_vec3_cross(c->right.raw, c->dir.raw, c->up.raw);
    glm_vec3_scale(c->right.raw, -1.0, c->left.raw);

    glm_vec3_normalize(c->up.raw);
    glm_vec3_normalize(c->right.raw);
    glm_vec3_normalize(c->left.raw);
}

void
camZoom(Camera *c, double x_offset, double y_offset)
{
    c->zoom -= 1 * y_offset;
    if (c->zoom < 1.0){
        c->zoom = 1.0;
    }
    if (c->zoom > 45.0){
        c->zoom = 45.0;
    }
}

void
camMove(Camera *c, CamDirection dir, u32 t)
{
    if (dir == DIR_FORWARD || dir == DIR_BACKWARD){
        vec3s dist = vec3_scale(c->dir, c->speed);
        if (dir == DIR_FORWARD)
            c->pos = vec3_add(c->pos, dist);
        else
            c->pos = vec3_sub(c->pos, dist);
    }
    if (dir == DIR_LEFT || dir == DIR_RIGHT){
        vec3s dist;
        if (dir == DIR_LEFT)
            dist = vec3_scale(c->left, c->speed);
        else
            dist = vec3_scale(c->right, c->speed);
        c->pos = vec3_add(c->pos, dist);
    }
    // UPDATE "AT"
    c->at = vec3_add(c->pos, c->dir);
}

void
camToggleSprint(Camera *c)
{
    c->is_sprinting = !c->is_sprinting;
    c->speed = (c->is_sprinting) ? c->speed_sprint : c->speed_default;
}

void
camInit(Camera *c)
{
    c->dir = (vec3s){0, 0, 1};
    c->up = (vec3s){0, 1, 0};
    c->up_world = (vec3s){0, 1, 0};
    c->pos = (vec3s){0, 0, -50};

    glm_vec3_cross(c->dir.raw, c->up_world.raw, c->right.raw);
    glm_vec3_cross(c->right.raw, c->dir.raw, c->up.raw);
    glm_vec3_scale(c->right.raw, -1.0, c->left.raw);

    glm_vec3_normalize(c->up.raw);
    glm_vec3_normalize(c->right.raw);
    glm_vec3_normalize(c->left.raw);

    c->speed = 1.00;
    c->speed_default = 1.00;
    c->speed_sprint = 4.00;
    c->fov = 45.0;
    c->draw_dist = 5000.0;
    camZoom(c, 0, 0);
}



























