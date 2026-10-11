#pragma once

// Renders a simple Notepad-style window with placeholder text.
// Does nothing when *p_open is false. The window's close (X) button
// sets *p_open back to false.
void RenderNotepad(bool* p_open);
