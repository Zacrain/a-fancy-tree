#ifndef FPV_PLAYER_HPP
#define FPV_PLAYER_HPP

//#include <string>

#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>

namespace godot {

class FPVPlayer : public CharacterBody3D {

    GDCLASS(FPVPlayer, CharacterBody3D);

    // **********************
    // ***** Attributes *****
    // ********************** 
    private:

    protected:

    // ***** Orientation related variables *****
    /// Accumulated vertical rotation. Keeps track of overall vertical rotation and helps to lock possible rotations.
    /// (Can't break your neck just to look at your own tushy.)
    double accumulated_vertical_rot {0.0};
    /// Angle between global y-axis and local y-axis (of the camera). Modified by rotation processing.
    /// performance.
    double angle_y_glob_to_y_loc;
    /// Direction of horizontal rotations.
    double horizontal_rot_direction {-1.0};
    /// Horizontal rotation speed. "Mouse sensitivity X".
    double horizontal_rot_speed {0.001};
    /// Overstretch in vertical down direction. (Allows to look through your legs.)
    double vertical_overstretch_down {0.0};
    /// Overstretch in vertical up direction. (Allows to lean your head a bit more into the neck.)
    double vertical_overstretch_up {0.0};
    /// Vertical rotation angle. Modified by rotation processing.
    double vertical_rot_ang;
    /// Direction of vertical rotations.
    double vertical_rot_direction {1.0};
    /// Vertical rotation speed. "Mouse sensitivity Y".
    double vertical_rot_speed {0.001};

    // ***** Translation related variables *****
    /// Fall speed multiplier.
    double move_fall_speed {10.0};
    /// Jump speed multiplier.
    double move_jump_speed {10.0};
    /// Move speed multiplier.
    double move_speed {10.0};
    

    /// Reference to global Input singleton. This will allow us to catch and process user inputs.
    Input& input;

    /// Pointer to camera node. May be nullptr. Set during _ready().
    Camera3D* camera_node_ptr; // TODO: mhm... are there smart pointers in Godot?

    // Path to camera child node. TODO: externalize.
    const NodePath camera_node_path {"Camera3D"};

    /// Action names to distinguish user input actions.
    const StringName action_name_move_back    {"move_back"};
    const StringName action_name_move_forward {"move_forward"};
    const StringName action_name_move_jump    {"move_jump"};
    const StringName action_name_move_left    {"move_left"};
    const StringName action_name_move_right   {"move_right"};

    /// Camera and Player transform objects. Modified by rotation processing.
    Transform3D camera_transform;
    Transform3D characterBody_transform;

    /// Move direction. Zero initialized by default.
    Vector3 move_direction;
    /// Target velocity. Changed during _physics_process().
    Vector3 target_vel;

    public:

    // *******************
    // ***** Methods *****
    // *******************

    private:

    protected:

    /** @brief Binds selected methods of this class to Godot and makes them visible. (Similar to @export .)
     * 
     *  Currently binds getters and setters.
     */
    static void _bind_methods();
        
    public:

    /** @brief Constructor, sets the singleton reference upon construction. */
    FPVPlayer() : input(*Input::get_singleton()) {};

    /** @brief Overrides _ready method called right after an object of this class was instantiated.
     * 
     *  Here, only disables processing the node when in editor. And enabling it when playing.
     */
    virtual void _ready() override;

    /** @brief Overrides the inherited _physics_process method for making computations relevant for the physics engine.
     *  Runs on each frame.
     * 
     *  Currently handles moving the player, by processing player inputs.
     * 
     * @param[in] delta Time delta between calls. (Frametime.)
     */
    virtual void _physics_process(double delta) override;

    /** @brief Handles unhandled input and overrides the base class virtual method. 
     * 
     * Here only used to process mouse movements.
     * 
     * @param[in] event Reference to the unhandled input event.
     */
    virtual void _unhandled_input(const Ref<InputEvent> &event) override;

    // ***** Getter & Setter *****
    inline bool get_horizontal_rot_direction() const;
    inline bool get_vertical_rot_direction() const;

    inline double get_horizontal_rot_speed() const;
    inline double get_move_fall_speed() const;
    inline double get_move_jump_speed() const;
    inline double get_move_speed() const;
    inline double get_vertical_overstretch_down() const;
    inline double get_vertical_overstretch_up() const;
    inline double get_vertical_rot_speed() const;
    
    inline void set_horizontal_rot_direction(const bool invert);
    inline void set_vertical_rot_direction(const bool invert);

    inline void set_horizontal_rot_speed(const double horizontal_rotation_speed);
    inline void set_move_fall_speed(const double fall_speed);
    inline void set_move_jump_speed(const double jump_speed);
    inline void set_move_speed(const double ground_move_speed);
    inline void set_vertical_overstretch_down(const double vertical_overtretch_downwards_degree);
    inline void set_vertical_overstretch_up(const double vertical_overstretch_upwards_degree);
    inline void set_vertical_rot_speed(const double vertical_rotation_speed);

};

} // namespace godot

#endif /* FPV_PLAYER_HPP */
