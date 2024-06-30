#include "../include/fpv_player.hpp"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "../include/math_constants.hpp"
#include "../include/vector_constants.hpp"

using namespace godot;

// Bind methods
void FPVPlayer::_bind_methods() {

    // Horizontal Rotation Direction
    ClassDB::bind_method(D_METHOD("get_horizontal_rot_direction"), &FPVPlayer::get_horizontal_rot_direction);
    ClassDB::bind_method(D_METHOD("set_horizontal_rot_direction", "invert"), &FPVPlayer::set_horizontal_rot_direction);
    ClassDB::add_property("FPVPlayer", PropertyInfo(Variant::BOOL, "invert horizontal axis"),
        "set_horizontal_rot_direction", "get_horizontal_rot_direction");
    
    // Vertical Rotation Direction
    ClassDB::bind_method(D_METHOD("get_vertical_rot_direction"), &FPVPlayer::get_vertical_rot_direction);
    ClassDB::bind_method(D_METHOD("set_vertical_rot_direction", "invert"), &FPVPlayer::set_vertical_rot_direction);
    ClassDB::add_property("FPVPlayer", PropertyInfo(Variant::BOOL, "invert vertical axis"),
        "set_vertical_rot_direction", "get_vertical_rot_direction");

    // Horizontal Rotation Speed
    ClassDB::bind_method(D_METHOD("get_horizontal_rot_speed"), &FPVPlayer::get_horizontal_rot_speed);
    ClassDB::bind_method(D_METHOD("set_horizontal_rot_speed", "horizontal_rotation_speed"), 
        &FPVPlayer::set_horizontal_rot_speed);
    ClassDB::add_property("FPVPlayer", PropertyInfo(Variant::FLOAT, "horizontal rotation speed"),
        "set_horizontal_rot_speed", "get_horizontal_rot_speed");

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

    // Vertical Overstretch Down
    ClassDB::bind_method(D_METHOD("get_vertical_overstretch_down"), &FPVPlayer::get_vertical_overstretch_down);
    ClassDB::bind_method(D_METHOD("set_vertical_overstretch_down", "vertical_overstretch_downwards"), 
        &FPVPlayer::set_vertical_overstretch_down);
    ClassDB::add_property("FPVPlayer", PropertyInfo(Variant::FLOAT, "vertical overstretch downwards"),
        "set_vertical_overstretch_down", "get_vertical_overstretch_down");
    
    // Vertical Overstretch Up
    ClassDB::bind_method(D_METHOD("get_vertical_overstretch_up"), &FPVPlayer::get_vertical_overstretch_up);
    ClassDB::bind_method(D_METHOD("set_vertical_overstretch_up", "vertical_overstretch_upwards"), 
        &FPVPlayer::set_vertical_overstretch_up);
    ClassDB::add_property("FPVPlayer", PropertyInfo(Variant::FLOAT, "vertical overstretch upwards"),
        "set_vertical_overstretch_up", "get_vertical_overstretch_up");

    // Vertical Rotation Speed
    ClassDB::bind_method(D_METHOD("get_vertical_rot_speed"), &FPVPlayer::get_vertical_rot_speed);
    ClassDB::bind_method(D_METHOD("set_vertical_rot_speed", "vertical_rotation_speed"), 
        &FPVPlayer::set_vertical_rot_speed);
    ClassDB::add_property("FPVPlayer", PropertyInfo(Variant::FLOAT, "vertical rotation speed"),
        "set_vertical_rot_speed", "get_vertical_rot_speed");

}

// Node processing
void FPVPlayer::_ready() {
    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
    else
        set_process_mode(Node::ProcessMode::PROCESS_MODE_INHERIT);

    camera_node_ptr = get_node<Camera3D>(camera_node_path);
    // TODO: make the child node a requirement. Similar to missing child nodes in default Godot classes (you get a warning in the editor)
}

void FPVPlayer::_physics_process(double delta) {

    // Reset movement. If no button is pressed, movement should stop and therefore be zero.
    move_direction.zero();

    // ***** Ground Movement & Jump *****
    // Reasoning for using += and -= below instead of a single = :
    // If the player presses two opposite keys (almost) simultaneously, they should cancel each other out instead of
    // having the last pressed key overwrite the previous one.
    if (input.is_action_pressed(action_name_move_forward))
        move_direction.z -= 1;
    if (input.is_action_pressed(action_name_move_back))
        move_direction.z += 1;
    if (input.is_action_pressed(action_name_move_left))
        move_direction.x -= 1;
    if (input.is_action_pressed(action_name_move_right))
        move_direction.x += 1;
    if (input.is_action_pressed(action_name_move_jump)) // bunny hop?
        move_direction.y += 1;

    // Normalize vector if inequal to zero. Otherwise we would destroy the universe. (Division by zero.)
    if (move_direction != Vec3_ZERO)
        move_direction.normalize();

    // Get the move direction related to the global coordinate system. This will make the succeeding computations
    // perform in the global coordinate system, which seems to be used by set_velocity() and move_and_slide();
    move_direction = to_global(move_direction) - get_transform().get_origin();

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

void FPVPlayer::_unhandled_input(const Ref<InputEvent> &event) {
    const Ref<InputEventMouseMotion> mouse_motion_event = event;

    // Ensure that the underlying pointer of Ref is not nullptr.
    if (mouse_motion_event.is_valid()) {
        
        // Rotate player horizontally.
        // x-axis is the "side"-axis in view coords, thereby determines the rotation around y, the up axis in local coords.
        rotate_y(mouse_motion_event->get_relative().x * horizontal_rot_direction * horizontal_rot_speed);

        // Orthonormalize transform. It may get non-orthonormal with time due to floating point represenation precision.
        characterBody_transform = get_transform();
        characterBody_transform.orthonormalize();
        set_transform(characterBody_transform);

        // Rotate the camera of the player vertically, if the camera node was found.
        if (camera_node_ptr) { // TODO: make it a requirement

            // y-axis is the "up"-axis in view coords, thereby determines the rotation around x, the side-axis in local coords.
            vertical_rot_ang = mouse_motion_event->get_relative().y * vertical_rot_direction * vertical_rot_speed;
            accumulated_vertical_rot += vertical_rot_ang;
            angle_y_glob_to_y_loc = 
                Math::acos(get_global_basis().get_column(1).dot(camera_node_ptr->get_basis().get_column(1)));

            if (accumulated_vertical_rot > Math_PI_HALFED + vertical_overstretch_up) {
                vertical_rot_ang = Math_PI_HALFED + vertical_overstretch_up - angle_y_glob_to_y_loc;
                accumulated_vertical_rot = Math_PI_HALFED + vertical_overstretch_up;
            }
            else if (accumulated_vertical_rot < -(Math_PI_HALFED + vertical_overstretch_down)) {
                vertical_rot_ang = -(Math_PI_HALFED + vertical_overstretch_down - angle_y_glob_to_y_loc);
                accumulated_vertical_rot = -(Math_PI_HALFED + vertical_overstretch_down);
            }
            
            camera_node_ptr->rotate_x(vertical_rot_ang);

            // Orthonormalize transform. It may get non-orthonormal with time due to floating point represenation precision.
            camera_transform = camera_node_ptr->get_transform();
            camera_transform.orthonormalize();
            camera_node_ptr->set_transform(camera_transform);
        }
    }
}

// Getter & Setter
inline bool FPVPlayer::get_horizontal_rot_direction() const {
    return horizontal_rot_direction > 0 ? true : false;
}

inline bool FPVPlayer::get_vertical_rot_direction() const {
    return vertical_rot_direction > 0 ? true : false;
}

inline double FPVPlayer::get_horizontal_rot_speed() const {
    return horizontal_rot_speed;
}

inline double FPVPlayer::get_move_fall_speed() const {
    return move_fall_speed;
}

inline double FPVPlayer::get_move_jump_speed() const {
    return move_jump_speed;
}

inline double FPVPlayer::get_move_speed() const {
    return move_speed;
}

inline double FPVPlayer::get_vertical_overstretch_up() const {
    return Math::rad_to_deg(vertical_overstretch_up);
}

inline double FPVPlayer::get_vertical_overstretch_down() const {
    return Math::rad_to_deg(vertical_overstretch_down);
}

inline double FPVPlayer::get_vertical_rot_speed() const {
    return vertical_rot_speed;
}

inline void FPVPlayer::set_horizontal_rot_direction(const bool invert) {
    horizontal_rot_direction = -horizontal_rot_direction;
}

inline void FPVPlayer::set_vertical_rot_direction(const bool invert) {
    vertical_rot_direction = -vertical_rot_direction;
}

inline void FPVPlayer::set_horizontal_rot_speed(const double horizontal_rotation_speed) {
    horizontal_rot_speed = horizontal_rotation_speed;
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

inline void FPVPlayer::set_vertical_overstretch_down(const double vertical_overtretch_downwards_degree) {
    vertical_overstretch_down = Math::deg_to_rad(vertical_overtretch_downwards_degree);
}

inline void FPVPlayer::set_vertical_overstretch_up(const double vertical_overstretch_upwards_degree) {
    vertical_overstretch_up = Math::deg_to_rad(vertical_overstretch_upwards_degree);
}

inline void FPVPlayer::set_vertical_rot_speed(const double vertical_rotation_speed) {
    vertical_rot_speed = vertical_rotation_speed;
}

