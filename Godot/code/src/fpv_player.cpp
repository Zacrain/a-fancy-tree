#include "../include/fpv_player.hpp"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "../include/vector_constants.hpp"

using namespace godot;

// Bind methods
void FPVPlayer::_bind_methods() {

    // Move Fall Speed
    ClassDB::bind_method(D_METHOD("get_move_fall_speed"), &FPVPlayer::get_move_fall_speed);
    ClassDB::bind_method(D_METHOD("set_move_fall_speed", "fall_speed"), &FPVPlayer::set_move_fall_speed);
    ClassDB::add_property("FPVPlayer", PropertyInfo(Variant::FLOAT, "fall speed"), "set_move_fall_speed", 
        "get_move_fall_speed");
    
    // Move Jump Speed
    ClassDB::bind_method(D_METHOD("get_move_jump_speed"), &FPVPlayer::get_move_jump_speed);
    ClassDB::bind_method(D_METHOD("set_move_jump_speed", "jump_speed"), &FPVPlayer::set_move_jump_speed);
    ClassDB::add_property("FPVPlayer", PropertyInfo(Variant::FLOAT, "jump speed"), "set_move_jump_speed", 
        "get_move_jump_speed");

    // Move Speed
    ClassDB::bind_method(D_METHOD("get_move_speed"), &FPVPlayer::get_move_speed);
    ClassDB::bind_method(D_METHOD("set_move_speed", "ground_move_speed"), &FPVPlayer::set_move_speed);
    ClassDB::add_property("FPVPlayer", PropertyInfo(Variant::FLOAT, "ground move speed"), "set_move_speed", 
        "get_move_speed");

}

// Node processing
void FPVPlayer::_ready() {
    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
    else
        set_process_mode(Node::ProcessMode::PROCESS_MODE_INHERIT);
}

void FPVPlayer::_physics_process(double delta) {

    // Reset movement. If no button is pressed, movement should stop and therefore be zero.
    move_direction.zero();

    // ***** Ground Movement & Jump *****
    // Reasoning for using += and -= below instead of a single = :
    // If the player presses two opposite keys (almost) simultaneously, they should cancel each other out instead of
    // having the last pressed key overwrite the previous one.
    if (input.is_action_pressed(action_name_move_forward))
        move_direction.z += 1;
    if (input.is_action_pressed(action_name_move_back))
        move_direction.z -= 1;
    if (input.is_action_pressed(action_name_move_left))
        move_direction.x += 1;
    if (input.is_action_pressed(action_name_move_right))
        move_direction.x -= 1;
    if (input.is_action_pressed(action_name_move_jump)) // bunny hop?
        move_direction.y += 1;

    // Normalize vector if inequal to zero. Otherwise we would destroy the universe. (Division by zero.)
    if (move_direction != Vec3_ZERO)
        move_direction.normalize();

    // Fall back to floor when in air.
    if (!is_on_floor())
        target_vel.y -= (move_fall_speed * delta);
    else // Jump. Bunny hop if is_action_pressed() is used instead of is_action_just_pressed()
        target_vel.y = move_direction.y * move_jump_speed;

    // Set Ground movement velocity.
    target_vel.x = move_direction.x * move_speed;
    target_vel.z = move_direction.z * move_speed;

    set_velocity(target_vel);
    move_and_slide();
}

// Getter & Setter
inline double FPVPlayer::get_move_fall_speed() const {
    return move_fall_speed;    
}

inline double FPVPlayer::get_move_jump_speed() const {
    return move_jump_speed;
}

inline double FPVPlayer::get_move_speed() const {
    return move_speed;
}

inline void FPVPlayer::set_move_fall_speed(const double fall_speed) {
    move_fall_speed = fall_speed;
}

inline void FPVPlayer::set_move_jump_speed(const double jump_speed) {
    move_jump_speed = jump_speed;
}

inline void FPVPlayer::set_move_speed(const double ground_move_speed) {
    move_speed = ground_move_speed;
}