/* EOS 7D.206 module callbacks and settings.
 * The film OS as a module: the end of filmroll.c when it is built as
 * ML/modules/filmos.mo (build_fork.sh --module), in place of its INIT_FUNC
 * and TASK_CREATE. Included there so it sees filmroll.c's statics. */


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

/* A camera that ran the built-in film OS kept these settings in Magic
 * Lantern's own magic.cfg (config.c). The first time the module starts (no
 * FILMOS.CFG yet) it takes them from there, so a roll in the camera carries
 * on where it was. */
static void config_from_core()
{
#ifdef CONFIG_7D_206
    /* This port never shipped a built-in film OS. Keep its settings private. */
    return;
#else
    char path[64];
    snprintf(path, sizeof(path), "%sfilmos.cfg", get_config_dir());
    if ((int) FIO_GetFileSize_direct(path) >= 0)
        return;
    snprintf(path, sizeof(path), "%smagic.cfg", get_config_dir());
    int size = 0;
    char *buf = (char *) read_entire_file(path, &size);
    if (!buf)
        return;
    int taken = 0;
    for (char *line = buf; line < buf + size; )
    {
        char *end = line;
        while (end < buf + size && *end != '\n' && *end != '\r') end++;
        char save = (end < buf + size) ? *end : 0;
        if (end < buf + size) *end = 0;
        if (strncmp(line, "filmroll.", 9) == 0)
        {
            for (module_config_t *c = __module_config_filmos; c->name; c++)
            {
                int n = strlen(c->ref->name);
                if (strncmp(line, c->ref->name, n) == 0 && (line[n] == ' ' || line[n] == '='))
                {
                    char *v = line + n;
                    while (*v == ' ' || *v == '=') v++;
                    *(c->ref->value) = atoi(v);
                    taken++;
                    break;
                }
            }
        }
        if (end < buf + size) *end = save;
        line = end + 1;
    }
    fio_free(buf);
    dbg("init: %d settings taken from magic.cfg (first start as a module)", taken);
#endif
}

/* errno for the maths library linked into the module (fork/module/Makefile) */
int *fos_errno(void)
{
    static int e;
    return &e;
}

static int filmos_running;
static unsigned int filmos_init()
{
    if (!is_camera("7D", "2.0.6")) return 1;
    config_from_core();
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

/* What the core's shim (filmhook.c) calls. Not static: the module loader
 * hands these to the core by name once the module is loaded. */
int filmos_key_used(struct event *event)
{
    return handle_filmroll_keys(event) == 0;
}

int filmos_menu_open()
{
    return filmroll_menu_requested();
}

int filmos_overlays()
{
    return filmos_running && fos_fonts_ready() && !gui_menu_shown() && !is_movie_mode() && !ml_menu_pending;
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
