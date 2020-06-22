#pragma once
#include <glm/detail/type_vec3.hpp>

void board_init();
void display();
void timer(int);
void create_menu();
void action_menu(int);
void passive_motion(int, int);
void put_checker();
void undo();
void init();
void create_shader_program(char*, char*, GLuint&);
void update_uniform_fragment_shader(GLuint& );
void compile_shader(GLuint& shader);
void init_data();
void draw_possible_moves();
void draw_circle(float, float, float, const int, const glm::vec3, GLuint& ,int,  bool);
void draw_board_with_texture(GLuint&);
void draw_board_square(int, int, const glm::vec3, GLuint&);
void draw_checkers_piece(float, float, const int, const glm::vec3, GLuint&, int, bool);
void draw_crown(GLuint, int, int, int);
