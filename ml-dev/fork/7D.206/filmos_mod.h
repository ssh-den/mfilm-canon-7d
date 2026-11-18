/* EOS 7D.206 module callbacks and settings. Included by filmroll.c. */


/* The settings, saved in ML/SETTINGS/filmos.cfg by the module loader (same
 * names as before: filmroll.roll, filmroll.shot, ...). */
MODULE_CONFIGS_START()
    MODULE_CONFIG(film_sel)
    MODULE_CONFIG(exp_sel)
    MODULE_CONFIG(hud_enabled)
    MODULE_CONFIG(focus_screen)
    MODULE_CONFIG(leds_on_half)
    MODULE_CONFIG(wind_mode)
    MODULE_CONFIG(play_lock)
    MODULE_CONFIG(startup_picker)
    MODULE_CONFIG(boot_anim)
    MODULE_CONFIG(debug_log)
    MODULE_CONFIG(film_mode)
    MODULE_CONFIG(lens_widest)
    MODULE_CONFIG(manual_fstop)
    MODULE_CONFIG(aperture_keys)
    MODULE_CONFIG(scr_std)
    MODULE_CONFIG(scr_sat)
    MODULE_CONFIG(scr_warm)
    MODULE_CONFIG(scr_bright)
    MODULE_CONFIG(scr_flip)
    MODULE_CONFIG(expsim_set)
    MODULE_CONFIG(hud_style)
    MODULE_CONFIG(hud_batt)
    MODULE_CONFIG(print_size)
    MODULE_CONFIG(date_stamp)
    MODULE_CONFIG(recent_films)
    MODULE_CONFIG(page_fade)
    MODULE_CONFIG(canon_hidden)
    MODULE_CONFIG(roll_loaded)
    MODULE_CONFIG(roll_number)
    MODULE_CONFIG(roll_film)
    MODULE_CONFIG(roll_exposures)
    MODULE_CONFIG(frames_shot)
    MODULE_CONFIG(last_file_number)
    MODULE_CONFIG(roll_folder)
    MODULE_CONFIG(need_wind)
MODULE_CONFIGS_END()

/* This core avoids config writes at shutdown. Persist roll/settings from the
 * normal film task once Canon is idle, also when no native ML menu was opened. */
static void filmos_save_if_dirty(void)
{
    static unsigned previous;
    static int saved;
    static int retry_at;
    if (ml_shutdown_requested || ui_running || lens_info.job_state ||
        !module_port.done || module_port.loading || module_port.result) return;
    if ((int)(get_ms_clock()-retry_at)<0) return;
    unsigned hash=2166136261u;
    for (module_config_t *c=__module_config_filmos;c->name;c++)
        hash=(hash ^ (unsigned)*c->ref->value)*16777619u;
    if (!saved || hash!=previous) {
        module_entry_t entry={.name="filmos",.filename="FILMOS.MO",.config=__module_config_filmos};
        if (module_config_save_now(&entry)==0) {
            previous=hash;
            saved=1;
        } else retry_at=get_ms_clock()+1000;
    }
}

/* errno storage for the maths library linked into this module. */
int *fos_errno(void)
{
    static int e;
    return &e;
}

static int filmos_running;
static unsigned int filmos_init()
{
    if (!is_camera("7D", "2.0.6")) return 1;
    filmroll_init();
    int task=task_create("film_main", 0x1e, 0x2000, filmroll_task, 0);
    if (task<=0) return 1;
    filmos_running=1;
    return 0;
}

static unsigned int filmos_deinit()
{
    filmos_running=0;
    ui_abort=1;
    return 0;
}

/* The 7D core binds this symbol when the module is loaded. */
int filmos_overlays()
{
    return filmos_running && fos_fonts_ready() && !gui_menu_shown() && !is_movie_mode() &&
           !ml_menu_pending && (ui_running ||
               (!canon_quick_opening() && hud_enabled && roll_loaded && (hud_showing || canon_handover)));
}

static unsigned int filmos_raw_key(unsigned int arg)
{
    struct event *event=(struct event *)arg;
    if (!event || !filmos_running || ml_shutdown_requested || gui_menu_shown() || !fos_fonts_ready()) return 1;
    if (!ui_running && is_movie_mode()) return 1;
    /* Consume the chord now; opening while held makes ML close immediately. */
    if (!ui_running && event->param==BGMT_TRASH && get_halfshutter_pressed()) {
        ml_menu_pending=1;
        return 0;
    }
    if (ml_menu_pending) return 1;
    return handle_filmroll_keys(event);
}
MODULE_CBRS_START()
    MODULE_CBR(CBR_KEYPRESS_RAW,filmos_raw_key,0)
MODULE_CBRS_END()

MODULE_INFO_START()
    MODULE_INIT(filmos_init)
    MODULE_DEINIT(filmos_deinit)
MODULE_INFO_END()
