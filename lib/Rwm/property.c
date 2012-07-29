void
Rwm_set_property_handler(struct Rwm_app *app,
                         Atom atom,
                         Rwm_propertyhandler_t *handler)
{
    struct R_wm *wm;
    struct Rwm_property *property;
    int key;

    wm = R_global.app->client;
    key = atom & (RWM_PROPERTY_HASH_ITEMS - 1);
    property = wm->propertyhash[key];
    if (!propertyhash) {
        property = calloc(1, sizeof(struct Rwm_property));
        wm->propertyhash[key] = property;
    }
    if (property) {
        while ((property->next) && (property->handler)) {
            property = property->next;
        }
        property->next = calloc(1, sizeof(struct Rwm_property));
        property = property->next;
        property->atom = atom;
        propertyhandler = handler;
    }
}

