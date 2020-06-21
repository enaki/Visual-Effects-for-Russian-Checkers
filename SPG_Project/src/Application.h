#pragma once

//prototipurile functiilor
void board_init();
void display();
void timer(int);
void create_menu();
void action_menu(int);
void passive_motion(int, int);
void put_checker();
void undo();
void init();
void create_shader_program(char* vertex_shader_file, char* fragment_shader_path, GLuint& shader_programme);
void update_uniform_fragment_shader(GLuint& shader_programme);
void compile_shader(GLuint& shader);
void init_data();
void draw_possible_moves();

