#include "TaskManager.h"
#include "imgui.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

namespace {

// ---------------------------------------------------------------------
// Dummy data
// ---------------------------------------------------------------------
struct Process {
    std::string name;
    int         pid;
    const char* status;
    float       cpu_base;   // baseline CPU % (dummy)
    float       mem_mb;     // memory usage in MB (dummy)
    float       cpu_live;   // CPU % shown on screen (wobbles around cpu_base)
};

enum ColumnId { COL_NAME, COL_PID, COL_STATUS, COL_CPU, COL_MEM };

const float TOTAL_MEMORY_MB = 16384.0f; // pretend the machine has 16 GB

std::vector<Process> MakeDummyProcesses() {
    return {
        { "CSOPESY Desktop Shell",        1024, "Running",   3.2f,  148.0f, 0 },
        { "Compositor (GLFW + OpenGL)",   1088, "Running",   5.8f,  212.4f, 0 },
        { "Dear ImGui Renderer",          1132, "Running",   2.1f,   96.7f, 0 },
        { "Taskbar Service",              1260, "Running",   0.2f,   12.9f, 0 },
        { "TimeClock Service",            1204, "Running",   0.1f,    8.3f, 0 },
        { "Wallpaper Loader (stb_image)", 1316, "Suspended", 0.0f,   64.0f, 0 },
        { "Input Event Poller",           1512, "Running",   0.6f,    6.1f, 0 },
        { "Scheduler Daemon",             1480, "Running",   1.0f,   22.5f, 0 },
        { "Virtual Memory Manager",       1544, "Running",   3.0f, 1840.0f, 0 },
        { "File System Indexer",          1600, "Waiting",   0.4f,   57.3f, 0 },
        { "Network Stack",                1668, "Running",   0.9f,   31.8f, 0 },
        { "Audio Mixer",                  1724, "Waiting",   0.3f,   18.6f, 0 },
        { "Notepad",                      2048, "Running",   0.0f,   14.2f, 0 },
        { "Task Manager",                 2112, "Running",   1.4f,   38.6f, 0 },
        { "System Interrupts",               4, "Running",   0.5f,    0.0f, 0 },
    };
}

// ---------------------------------------------------------------------
// State that lives between frames
// ---------------------------------------------------------------------
const int HISTORY_LEN = 90;

std::vector<Process> g_processes;
bool   g_initialized   = false;
int    g_selected_pid  = -1;
double g_last_tick     = 0.0;
float  g_cpu_hist[HISTORY_LEN] = {0};
float  g_mem_hist[HISTORY_LEN] = {0};
float  g_cpu_total     = 0.0f;
float  g_mem_total_mb  = 0.0f;

void RecomputeTotals() {
    float cpu = 0.0f, mem = 0.0f;
    for (const Process& p : g_processes) { cpu += p.cpu_live; mem += p.mem_mb; }
    g_cpu_total    = std::min(cpu, 100.0f);
    g_mem_total_mb = mem;
}

void PushHistory() {
    std::copy(g_cpu_hist + 1, g_cpu_hist + HISTORY_LEN, g_cpu_hist);
    std::copy(g_mem_hist + 1, g_mem_hist + HISTORY_LEN, g_mem_hist);
    g_cpu_hist[HISTORY_LEN - 1] = g_cpu_total;
    g_mem_hist[HISTORY_LEN - 1] = 100.0f * g_mem_total_mb / TOTAL_MEMORY_MB;
}

// Makes the dummy numbers wobble a little so the window feels "live".
void Tick(double t) {
    for (Process& p : g_processes) {
        float wobble = (p.cpu_base > 0.0f)
            ? p.cpu_base * 0.35f * (float)std::sin(t * 1.7 + p.pid)
            : 0.0f;
        p.cpu_live = std::max(0.0f, p.cpu_base + wobble);
    }
    RecomputeTotals();
    PushHistory();
}

void Init() {
    g_processes = MakeDummyProcesses();
    g_selected_pid = -1;
    double t = ImGui::GetTime();
    for (int i = 0; i < HISTORY_LEN; i++) Tick(t - (HISTORY_LEN - i) * 0.5);
    g_last_tick = t;
    g_initialized = true;
}

// Windows-like "heat" shading: the busier the cell, the more orange it gets.
void HeatCell(float ratio) {
    ratio = std::min(std::max(ratio, 0.0f), 1.0f);
    ImU32 col = ImGui::GetColorU32(ImVec4(1.0f, 0.72f, 0.15f, 0.10f + 0.45f * ratio));
    ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, col);
}

// ---------------------------------------------------------------------
// Tabs
// ---------------------------------------------------------------------
void DrawProcessesTab() {
    ImGui::Text("CPU: %.0f%%    Memory: %.0f%%    Processes: %d",
                g_cpu_total, 100.0f * g_mem_total_mb / TOTAL_MEMORY_MB, (int)g_processes.size());
    ImGui::Spacing();

    float footer_h = ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y;

    ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable |
                            ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerV |
                            ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp;

    if (ImGui::BeginTable("ProcessTable", 5, flags, ImVec2(0.0f, -footer_h))) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Name",   ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_DefaultSort, 3.0f, COL_NAME);
        ImGui::TableSetupColumn("PID",    ImGuiTableColumnFlags_WidthStretch, 0.8f, COL_PID);
        ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthStretch, 1.2f, COL_STATUS);
        ImGui::TableSetupColumn("CPU",    ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_PreferSortDescending, 1.0f, COL_CPU);
        ImGui::TableSetupColumn("Memory", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_PreferSortDescending, 1.2f, COL_MEM);
        ImGui::TableHeadersRow();

        // Sorting (click a column header)
        if (ImGuiTableSortSpecs* specs = ImGui::TableGetSortSpecs()) {
            if (specs->SpecsCount > 0) {
                const ImGuiTableColumnSortSpecs& s = specs->Specs[0];
                bool asc = (s.SortDirection == ImGuiSortDirection_Ascending);
                std::stable_sort(g_processes.begin(), g_processes.end(),
                    [&](const Process& a, const Process& b) {
                        int cmp = 0;
                        switch (s.ColumnUserID) {
                            case COL_NAME:   cmp = a.name.compare(b.name); break;
                            case COL_PID:    cmp = (a.pid > b.pid) - (a.pid < b.pid); break;
                            case COL_STATUS: cmp = std::string(a.status).compare(b.status); break;
                            case COL_CPU:    cmp = (a.cpu_live > b.cpu_live) - (a.cpu_live < b.cpu_live); break;
                            case COL_MEM:    cmp = (a.mem_mb > b.mem_mb) - (a.mem_mb < b.mem_mb); break;
                        }
                        return asc ? (cmp < 0) : (cmp > 0);
                    });
            }
        }

        for (const Process& p : g_processes) {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(COL_NAME);
            ImGui::PushID(p.pid);
            bool selected = (g_selected_pid == p.pid);
            if (ImGui::Selectable(p.name.c_str(), selected, ImGuiSelectableFlags_SpanAllColumns))
                g_selected_pid = p.pid;
            ImGui::PopID();

            ImGui::TableSetColumnIndex(COL_PID);
            ImGui::Text("%d", p.pid);

            ImGui::TableSetColumnIndex(COL_STATUS);
            ImGui::TextUnformatted(p.status);

            ImGui::TableSetColumnIndex(COL_CPU);
            HeatCell(p.cpu_live / 25.0f);
            ImGui::Text("%.1f%%", p.cpu_live);

            ImGui::TableSetColumnIndex(COL_MEM);
            HeatCell(p.mem_mb / 1024.0f);
            ImGui::Text("%.1f MB", p.mem_mb);
        }
        ImGui::EndTable();
    }

    // Footer: End task
    bool has_selection = false;
    for (const Process& p : g_processes) if (p.pid == g_selected_pid) has_selection = true;

    ImGui::BeginDisabled(!has_selection);
    if (ImGui::Button("End task")) {
        g_processes.erase(
            std::remove_if(g_processes.begin(), g_processes.end(),
                           [](const Process& p) { return p.pid == g_selected_pid; }),
            g_processes.end());
        g_selected_pid = -1;
        RecomputeTotals();
    }
    ImGui::EndDisabled();
}

void DrawPerformanceTab() {
    float avail_w = ImGui::GetContentRegionAvail().x;
    float graph_h = 90.0f;
    char overlay[64];

    float mem_pct = 100.0f * g_mem_total_mb / TOTAL_MEMORY_MB;

    ImGui::Text("CPU");
    std::snprintf(overlay, sizeof(overlay), "%.0f%%", g_cpu_total);
    ImGui::PlotLines("##cpu", g_cpu_hist, HISTORY_LEN, 0, overlay, 0.0f, 100.0f, ImVec2(avail_w, graph_h));

    ImGui::Spacing();
    ImGui::Text("Memory");
    std::snprintf(overlay, sizeof(overlay), "%.0f MB / %.0f GB (%.0f%%)",
                  g_mem_total_mb, TOTAL_MEMORY_MB / 1024.0f, mem_pct);
    ImGui::PlotLines("##mem", g_mem_hist, HISTORY_LEN, 0, overlay, 0.0f, 100.0f, ImVec2(avail_w, graph_h));

    ImGui::Separator();
    int up = (int)ImGui::GetTime();
    ImGui::Text("Up time:              %d:%02d:%02d", up / 3600, (up / 60) % 60, up % 60);
    ImGui::Text("Processes:            %d", (int)g_processes.size());
    ImGui::Text("Logical processors:   8");
    ImGui::Text("Total memory:         %.0f GB", TOTAL_MEMORY_MB / 1024.0f);
}

} // namespace

// ---------------------------------------------------------------------
// Public entry point
// ---------------------------------------------------------------------
void RenderTaskManager(bool* p_open) {
    if (p_open == nullptr || !*p_open) return;

    if (!g_initialized) Init();

    double now = ImGui::GetTime();
    if (now - g_last_tick >= 0.5) {
        Tick(now);
        g_last_tick = now;
    }

    ImGui::SetNextWindowPos(ImVec2(280.0f, 90.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(640.0f, 440.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(480.0f, 320.0f), ImVec2(FLT_MAX, FLT_MAX));

    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoSavedSettings;
    if (!ImGui::Begin("Task Manager", p_open, flags)) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Reset dummy data")) Init();
            if (ImGui::MenuItem("Exit Task Manager")) *p_open = false;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (ImGui::BeginTabBar("TaskManagerTabs")) {
        if (ImGui::BeginTabItem("Processes"))   { DrawProcessesTab();   ImGui::EndTabItem(); }
        if (ImGui::BeginTabItem("Performance")) { DrawPerformanceTab(); ImGui::EndTabItem(); }
        ImGui::EndTabBar();
    }

    ImGui::End();
}
