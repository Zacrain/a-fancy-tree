#ifndef MAIN_NODE_HPP
#define MAIN_NODE_HPP

#include <godot_cpp/classes/node.hpp>

#include "../include/fpv_player.hpp"

namespace godot {

class MainNode : public Node {

    GDCLASS(MainNode, Node);

    // **********************
    // ***** Attributes *****
    // ********************** 
    private:

    protected:
    /// Pointer to player node. May be nullptr. Set during _ready().
    FPVPlayer* player_node_ptr; // TODO: mhm... are there smart pointers in Godot?
    /// Path of player node.
    const NodePath player_node_path {"player"};
    /// Teleport position to move the player to, if they fall below the map.
    const Vector3 teleport_pos {-0.336, 0.5, -6.767};
    

    public:

    // *******************
    // ***** Methods *****
    // *******************
    private:
    protected:
    /// Required to compile. Not used here yet.
    static void _bind_methods() {}

    public:

    /** @brief Class constructor. Does nothing currently. */
    MainNode() {}

    /** @brief Overrides _ready method called right after an object of this class was instantiated.
     * 
     *  Here, disables processing the node when in editor. And enabling it when playing.
     *  Also retrieves a player node, if possible.
     */
    virtual void _ready() override;

    /** @brief Overrides the inherited _physics_process method for making computations relevant for the physics engine.
     *  Runs on each frame.
     * 
     *  Currently does the following:
     *  - teleports the player - if existing - back to ground when they fall below.
     *  - TODO: set mouse mode and stuff
     * 
     * @param[in] delta Time delta between calls. (Frametime.)
     */
    virtual void _physics_process(double delta) override;


};

} // namespace godot

#endif /* MAIN_NODE_HPP */
