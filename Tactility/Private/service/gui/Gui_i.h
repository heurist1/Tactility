#pragma once

#include "MessageQueue.h"
#include "Mutex.h"
#include "Pubsub.h"
#include "service/gui/Gui.h"
#include "service/gui/ViewPort.h"
#include "service/gui/ViewPort_i.h"
#include <cstdio>

namespace tt::service::gui {

#define GUI_THREAD_FLAG_DRAW (1 << 0)
#define GUI_THREAD_FLAG_INPUT (1 << 1)
#define GUI_THREAD_FLAG_EXIT (1 << 2)
#define GUI_THREAD_FLAG_ALL (GUI_THREAD_FLAG_DRAW | GUI_THREAD_FLAG_INPUT | GUI_THREAD_FLAG_EXIT)

/** Gui structure */
struct Gui {
    // Thread and lock
    Thread* thread;
    Mutex* mutex;
    PubSubSubscription* loader_pubsub_subscription;

    // Layers and Canvas
    lv_obj_t* lvgl_parent;

    // App-specific
    ViewPort* app_view_port;

    lv_obj_t* _Nullable keyboard;
    lv_group_t* keyboard_group;
};

/** Update GUI, request redraw */
void requestDraw();

/** Lock GUI */
void lock();

/** Unlock GUI */
void unlock();

} // namespace
