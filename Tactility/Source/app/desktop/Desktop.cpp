#include "app/ManifestRegistry.h"
#include "Assets.h"
#include "Check.h"
#include "lvgl.h"
#include <algorithm>
#include "service/loader/Loader.h"

namespace tt::app::desktop {

static void on_app_pressed(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        const auto* manifest = static_cast<const Manifest*>(lv_event_get_user_data(e));
        service::loader::startApp(manifest->id, false, Bundle());
    }
}

static void create_app_widget(const Manifest* manifest, void* parent) {
    tt_check(parent);
    auto* list = static_cast<lv_obj_t*>(parent);
    const void* icon = !manifest->icon.empty() ? manifest->icon.c_str() : TT_ASSETS_APP_ICON_FALLBACK;
    lv_obj_t* btn = lv_list_add_button(list, icon, manifest->name.c_str());
    lv_obj_add_event_cb(btn, &on_app_pressed, LV_EVENT_CLICKED, (void*)manifest);
}

static void desktop_show(TT_UNUSED App& app, lv_obj_t* parent) {
    lv_obj_t* list = lv_list_create(parent);
    lv_obj_set_size(list, LV_PCT(100), LV_PCT(100));
    lv_obj_center(list);

    auto manifests = getApps();
    std::sort(manifests.begin(), manifests.end(), SortAppManifestByName);

    lv_list_add_text(list, "User");
    for (const auto& manifest: manifests) {
        if (manifest->type == TypeUser) {
            create_app_widget(manifest, list);
        }
    }

    lv_list_add_text(list, "System");
    for (const auto& manifest: manifests) {
        if (manifest->type == TypeSystem) {
            create_app_widget(manifest, list);
        }
    }
}

extern const Manifest manifest = {
    .id = "Desktop",
    .name = "Desktop",
    .type = TypeDesktop,
    .onShow = &desktop_show,
};

} // namespace
