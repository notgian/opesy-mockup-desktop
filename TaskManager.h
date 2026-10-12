#pragma once

// Renders a Windows-style Task Manager window (Processes + Performance tabs)
// using dummy/placeholder data. Does nothing when *p_open is false.
// The window's close (X) button sets *p_open back to false.
void RenderTaskManager(bool* p_open);
