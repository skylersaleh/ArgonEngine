/**
 * @brief This file contains the declaration for the initialization functions of the engine.
 * @file ArgonInit.h
 * @author Skyler Saleh, Ian Wiggins
 **/
#include <string>
#ifndef ARGON_INIT_H
#define ARGON_INIT_H
namespace Argon{

    static bool Run_Engine;

    /// Initialize the engine and HAL.
    void initialize_engine(std::string organization_name, std::string app_name);
    /// Termintate the engine, closing the program.
    void terminate_engine();
    /// Poll for events, this updates all hardware and HAL values, and should be called frequently.
    /// Returns true if the window is still open.
    bool poll_events();
    void set_manual_redraw(void (*draw)());
    /// Swaps the rendered buffers to make them visible to the user.
    void swap_buffers();
}
#endif
