#ifndef FPV_PLAYER_HPP
#define FPV_PLAYER_HPP

//#include <string>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input.hpp>

namespace godot {

class FPVPlayer : public CharacterBody3D {

    GDCLASS(FPVPlayer, CharacterBody3D);

    // **********************
    // ***** Attributes *****
    // ********************** 
    private:

    protected:

    /// Reference to global Input singleton. This will allow us to catch and process user inputs.
    Input& input;
    
    /// Action names to distinguish user input actions.
    const StringName action_name_move_back    {"move_back"};
    const StringName action_name_move_forward {"move_forward"};
    const StringName action_name_move_jump    {"move_jump"};
    const StringName action_name_move_left    {"move_left"};
    const StringName action_name_move_right   {"move_right"};

    /// Fall speed multiplier.
    double move_fall_speed {10};
    /// Jump speed multiplier.
    double move_jump_speed {10};
    /// Move speed multiplier.
    double move_speed {10};

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

    // ***** Getter & Setter *****
    inline double get_move_fall_speed() const;
    inline double get_move_jump_speed() const;
    inline double get_move_speed() const;

    inline void set_move_fall_speed(const double fall_speed);
    inline void set_move_jump_speed(const double jump_speed);
    inline void set_move_speed(const double ground_move_speed);


};

} // namespace godot

#endif /* FPV_PLAYER_HPP */
