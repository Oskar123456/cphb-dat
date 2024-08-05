static float cell_vertex_data[] = {
    -0.5f, -0.5f, 0.0f, // left  
    -0.5f,  0.5f, 0.0f, // right 
     0.5f,  0.5f, 0.0f, // top   

    -0.5f, -0.5f, 0.0f, // left  
     0.5f, -0.5f, 0.0f, // right 
     0.5f,  0.5f, 0.0f  // top   
};

typedef struct {
    union {
    // for hashmap / hashset
        ivec2s key;
        ivec2s value;
        ivec2s pos;
        struct {int x, y;};
    };
} Cell;

Cell*
golUpdate(Cell **cells)
{
    Cell *next_gen = NULL;
    int x, y, neighbor_x, neighbor_y;
    for (int i = 0; i < hmlen(*cells); ++i) {
        int center_cell_neighbors = 0;
        ivec2s center_cell_pos = (*cells)[i].pos;
        for (x = center_cell_pos.x - 1; x <= center_cell_pos.x + 1; ++x) {
            for (y = center_cell_pos.y - 1; y <= center_cell_pos.y + 1; ++y) {
                if (x == center_cell_pos.x && y == center_cell_pos.y)
                    continue;
                int neighbor_cell_neighbors = 0;
                ivec2s neighbor_cell_pos = {x, y};
                for (neighbor_x = neighbor_cell_pos.x - 1; neighbor_x <= neighbor_cell_pos.x + 1; ++neighbor_x) {
                    for (neighbor_y = neighbor_cell_pos.y - 1; neighbor_y <= neighbor_cell_pos.y + 1; ++neighbor_y) {
                        if (neighbor_x == neighbor_cell_pos.x && neighbor_y == neighbor_cell_pos.y)
                            continue;
                        ivec2s neighbor_n_cell_pos = {neighbor_x, neighbor_y};
                        if (hmgeti((*cells), neighbor_n_cell_pos) >= 0)
                            ++neighbor_cell_neighbors;
                    }
                }
                if (neighbor_cell_neighbors == 3)
                    hmput(next_gen, neighbor_cell_pos, neighbor_cell_pos);
                if (hmgeti((*cells), neighbor_cell_pos) >= 0)
                    ++center_cell_neighbors;
            }
        }
        if (center_cell_neighbors >= 2 && center_cell_neighbors <= 3)
            hmput(next_gen, center_cell_pos, center_cell_pos);
    }
    hmfree((*cells));
    return next_gen;
}

void
golRender(Cell *cells, vec4s bg_col, vec4s cell_col, u32 vao, u32 shader)
{
    glUseProgram(shader);
    glBindVertexArray(vao);

    u32 shader_uniform_color = glGetUniformLocation(shader, "uni_col");
    u32 shader_uniform_view = glGetUniformLocation(shader, "view");
    u32 shader_uniform_proj = glGetUniformLocation(shader, "proj");
    u32 shader_uniform_modl = glGetUniformLocation(shader, "modl");

    mat4s view = camGetViews(&player_camera);
    //mat4s proj = glms_ortho(
    //        -(_win_scr_width / 18.0), (_win_scr_width / 18.0), 
    //        -(_win_scr_height / 18.0), (_win_scr_height / 18.0), 
    //        0.01, player_camera.draw_dist);
    mat4s proj = glms_perspective(player_camera.fov, winGetScrAspect(), 0.01, player_camera.draw_dist);

    glUniform3fv(shader_uniform_color, 1, (float*)cell_col.raw);
    glUniformMatrix4fv(shader_uniform_view, 1, GL_FALSE, (float*)view.raw);
    glUniformMatrix4fv(shader_uniform_proj, 1, GL_FALSE, (float*)proj.raw);

    for (int i = 0; i < hmlen(cells); ++i) {
        vec3s cell_pos = {cells[i].x, cells[i].y, 0.0};
        vec4s cell_pos_4 = {cells[i].x, cells[i].y, 0.0, 1.0};
        mat4s modl = glms_translate(mat4_identity(), cell_pos);
        glUniformMatrix4fv(shader_uniform_modl, 1, GL_FALSE, (float*)modl.raw);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}

