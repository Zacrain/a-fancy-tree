#include "../include/main_node.hpp"

#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void MainNode::_ready() {
    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
    else {
        set_process_mode(Node::ProcessMode::PROCESS_MODE_INHERIT);
        // Hide mouse cursor and capture mouse in middle of screen.
        input.set_mouse_mode(Input::MouseMode::MOUSE_MODE_CAPTURED);
    }

    player_node_ptr = get_node<FPVPlayer>(player_node_path); // May become a nullptr!
}

void MainNode::_physics_process(double delta) {

    // TODO: preliminary polling. Use events / signals instead via colliders.
    // If a player node was found (!nullptr) check whether their position is below a certain threshold.
    // if so: reset position to somewhere on the map.
    if (player_node_ptr && player_node_ptr->get_position().y < -10) {
        player_node_ptr->set_position(teleport_pos);
    }

}