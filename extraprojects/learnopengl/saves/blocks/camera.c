/*
 *
 * */

typedef union CAM_UVEC3 {
  vec3 vec;
  struct {
    float x;
    float y;
    float z;
  };
} CAM_UVEC3;

struct Camera {
    CAM_UVEC3 pos;
    CAM_UVEC3 dir;
    CAM_UVEC3 at;
    CAM_UVEC3 up;
    CAM_UVEC3 right;
    CAM_UVEC3 left;
    CAM_UVEC3 up_world;

    float yaw;   // around y-axis
    float pitch; // around x-axis
    bool  restrict_pitch;

    float speedCurrent;
    float speedRegular;
    float speedSprint;
    float sens;
    float zoom;

    float draw_dist;

    u32 camtype;
};

enum cam_mvmt {
    DIR_FORWARD,
    DIR_BACKWARD,
    DIR_LEFT,
    DIR_RIGHT
};

enum cam_types {
    FLYING,
    FPS
};

void
cam_get_view(struct Camera *c, mat4 *dest)
{
    glm_vec3_add(c->pos.vec, c->dir.vec, c->at.vec);
    glm_lookat(c->pos.vec, c->at.vec, c->up.vec, *dest);
}

void
cam_update_dir(struct Camera *c, double x_offset, double y_offset)
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
    
    glm_vec3_add(c->pos.vec, dir, c->at.vec);
    glm_vec3_copy(dir, c->dir.vec);

    glm_vec3_cross(c->dir.vec, c->up_world.vec, c->left.vec);
    glm_vec3_cross(c->left.vec, c->dir.vec, c->up.vec);
    glm_vec3_scale(c->left.vec, -1.0, c->right.vec);

    glm_vec3_normalize(c->up.vec);
    glm_vec3_normalize(c->right.vec);
    glm_vec3_normalize(c->left.vec);
}

void
cam_update_zoom(struct Camera *c, double x_offset, double y_offset)
{
    c->zoom -= y_offset;
    if (c->zoom < 1.0)
        c->zoom = 1.0;
    if (c->zoom > 45.0)
        c->zoom = 45.0;
}

void
cam_update_move(struct Camera *c, enum cam_mvmt dir, u32 t)
{
    if (dir == DIR_FORWARD || dir == DIR_BACKWARD){
        vec3 dist;
        glm_vec3_scale(c->dir.vec, c->speedCurrent, dist);
        if (c->camtype == FPS)
            dist[1] = 0.0;
        if (dir == DIR_FORWARD)
            glm_vec3_add(c->pos.vec, dist, c->pos.vec);
        else
            glm_vec3_sub(c->pos.vec, dist, c->pos.vec);
    }
    if (dir == DIR_LEFT || dir == DIR_RIGHT){
        vec3 dist;
        if (dir == DIR_LEFT)
            glm_vec3_scale(c->right.vec, c->speedCurrent, dist);
        else
            glm_vec3_scale(c->left.vec, c->speedCurrent, dist);
        if (c->camtype == FPS)
            dist[1] = 0.0;
        glm_vec3_add(c->pos.vec, dist, c->pos.vec);
    }

}





























