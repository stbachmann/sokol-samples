//------------------------------------------------------------------------------
//  multi-window-sapp.c
//------------------------------------------------------------------------------
#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_glue.h"
#include "dbgui/dbgui.h"

static struct {
    sg_pass_action pass_action;
    sg_context main_context;
    sg_context second_context;
    sapp_window second_window;
} state;

void init(void) {
    // Main window setup
    state.main_context = sg_setup(&(sg_desc){
        .context = sapp_sgcontext()
    });
    state.pass_action = (sg_pass_action) {
        .colors[0] = { .action=SG_ACTION_CLEAR, .val={1.0f, 0.0f, 0.0f, 1.0f} }
    };
    __dbgui_setup(sapp_sample_count());

    state.second_window = sapp_create_window(&(sapp_window_desc){
        .window_title = "Second Window!"
    });
    sg_context_desc desc = sapp_window_sgcontext(state.second_window);
    state.second_context = sg_setup_context(&desc);
}

void frame(void) {
    float g = state.pass_action.colors[0].val[1] + 0.01f;
    state.pass_action.colors[0].val[1] = (g > 1.0f) ? 0.0f : g;
    sg_activate_context(state.main_context);
    sg_begin_default_pass(&state.pass_action, sapp_width(), sapp_height());
    __dbgui_draw();
    sg_end_pass();
    sg_commit();

    if(sapp_window_isvalid(state.second_window)){
        sg_activate_context(state.second_context);
        sg_begin_default_pass(&state.pass_action, sapp_window_width(state.second_window), sapp_window_height(state.second_window));
        sg_end_pass();
        sg_commit();
    }
}

void cleanup(void) {
    __dbgui_shutdown();
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = __dbgui_event,
        .width = 400,
        .height = 300,
        .gl_force_gles2 = true,
        .window_title = "Multi-Window (sokol app)",
        .window_pool_size = 2
    };
}

