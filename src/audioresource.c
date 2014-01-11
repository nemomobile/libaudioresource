
/**
 *
 * Copyright (C) 2013 Jolla Ltd.
 * Contact: Thomas Perl <thomas.perl@jolla.com>
 * All rights reserved.
 *
 * This file is part of libaudioresource
 *
 * You may use this file under the terms of the GNU Lesser General
 * Public License version 2.1 as published by the Free Software Foundation
 * and appearing in the file COPYING.LGPL included in the packaging
 * of this file.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * and appearing in the file license.lgpl included in the packaging
 * of this file.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 **/


#include "audioresource.h"

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <resource.h>


struct audioresource_t {
    enum audioresource_type_t type;
    audioresource_acquired_callback_t acquired_callback;
    void *user_data;
    bool acquired;

    resource_set_t *resource_set;
};


static void grant_callback(resource_set_t *resource_set, uint32_t resources, void *userdata) {
    audioresource_t *audio_resource = (audioresource_t *)userdata;
    bool acquired = ((resources & RESOURCE_AUDIO_PLAYBACK) != 0);

    if (audio_resource->acquired != acquired) {
        audio_resource->acquired_callback(audio_resource, acquired, audio_resource->user_data);
        audio_resource->acquired = acquired;
    }
}

static void advice_callback(resource_set_t *resource_set, uint32_t resources, void *userdata) {
    // TODO
}


audioresource_t *audioresource_init(enum audioresource_type_t type,
        audioresource_acquired_callback_t acquired_cb,
        void *user_data)
{
    const char *type_str;
    switch (type) {
        case AUDIO_RESOURCE_GAME:
            type_str = "game";
            break;
        case AUDIO_RESOURCE_MEDIA:
            type_str = "player";
            break;
        default:
            /* Invalid audio resource type */
            return NULL;
    }

    audioresource_t *audio_resource =
        (audioresource_t *)malloc(sizeof(audioresource_t));

    audio_resource->type = type;
    audio_resource->acquired_callback = acquired_cb;
    audio_resource->user_data = user_data;
    audio_resource->acquired = false;

    audio_resource->resource_set = resource_set_create(type_str,
            RESOURCE_AUDIO_PLAYBACK, 0, 0, grant_callback,
            audio_resource);

    resource_set_configure_advice_callback(audio_resource->resource_set,
            advice_callback, audio_resource);
    resource_set_configure_audio(audio_resource->resource_set, type_str,
            getpid(), "*");

    return audio_resource;
}

void audioresource_acquire(audioresource_t *audio_resource)
{
    resource_set_acquire(audio_resource->resource_set);
}

void audioresource_release(audioresource_t *audio_resource)
{
    resource_set_release(audio_resource->resource_set);
}

void audioresource_free(audioresource_t *audio_resource)
{
    resource_set_destroy(audio_resource->resource_set);
    free(audio_resource);
}
