
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


#ifndef LIBAUDIORESOURCE_AUDIORESOURCE_H
#define LIBAUDIORESOURCE_AUDIORESOURCE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Opaque pointer to our audio resource
typedef struct audioresource_t audioresource_t;

/**
 * Callback that needs to be passed to audioresource_init.
 *
 * This callback will be called every time the audio resource
 * is acquired or released (both can happen due to API requests
 * or because of system-initiated policy changes).
 *
 * The application can start playing audio as soon as this callback
 * is called with acquired=true, and must stop playing audio as soon
 * as this callback is called with acquired=false.
 *
 * The user_data parameter is the one passed to audioresource_init().
 **/
typedef void (*audioresource_acquired_callback_t)(audioresource_t *audio_resource,
        bool acquired, void *user_data);

/**
 * Type of audio resource - for games, use AUDIO_RESOURCE_GAME, for music
 * and audio players, use AUDIO_RESOURCE_MEDIA.
 **/
enum audioresource_type_t {
    AUDIO_RESOURCE_INVALID = 0,
    AUDIO_RESOURCE_GAME = 1,
    AUDIO_RESOURCE_MEDIA = 2,
};

/**
 * Initialize the audio resource library, registering a callback that
 * will be called every time the acquired state changes. The user_data
 * parameter can be NULL. It will be passed to the callback on ever call.
 **/
audioresource_t *audioresource_init(enum audioresource_type_t type,
        audioresource_acquired_callback_t acquired_cb,
        void *user_data);

/**
 * Request to acquire the audio resource. If the resource was successfully
 * acquired, the callback passed to audioresource_init() will be called.
 *
 * If the request was not successful, or if the resource was already
 * acquired, the call will do nothing.
 **/
void audioresource_acquire(audioresource_t *audio_resource);

/**
 * Request to release the audio resource. This function should be called
 * whenever the application stops playing back audio (e.g. when it is put
 * into the background). 
 *
 * If the request was not successful, or if the resource was not acquired,
 * the call will do nothing.
 **/
void audioresource_release(audioresource_t *audio_resource);

/**
 * Close the audio resource library and release resources / free memory.
 **/
void audioresource_free(audioresource_t *audio_resource);

#ifdef __cplusplus
};
#endif

#endif /* LIBAUDIORESOURCE_AUDIORESOURCE_H */
