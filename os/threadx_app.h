/**
 * @file        threadx_app.h
 * 
 * @brief       ThreadX application initialization interface
 * 
 * @author      toberg
 * 
 * @date        2025/12/30
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Thread function for the main application
 * @param thread_input Thread input parameter (unused)
 */
void main_app_thread_entry(ULONG thread_input);

#ifdef __cplusplus
}
#endif
