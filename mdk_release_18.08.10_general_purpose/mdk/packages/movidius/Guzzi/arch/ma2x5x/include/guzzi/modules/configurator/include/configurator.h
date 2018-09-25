/* =============================================================================
* Copyright (c) 2013-2014 MM Solutions AD
* All rights reserved. Property of MM Solutions AD.
*
* This source code may not be used against the terms and conditions stipulated
* in the licensing agreement under which it has been supplied, or without the
* written permission of MM Solutions. Rights to use, copy, modify, and
* distribute or disclose this source code and its documentation are granted only
* through signed licensing agreement, provided that this copyright notice
* appears in all copies, modifications, and distributions and subject to the
* following conditions:
* THIS SOURCE CODE AND ACCOMPANYING DOCUMENTATION, IS PROVIDED AS IS, WITHOUT
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED. MM SOLUTIONS SPECIFICALLY DISCLAIMS
* ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN
* NO EVENT SHALL MM SOLUTIONS BE LIABLE TO ANY PARTY FOR ANY CLAIM, DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
* PROFITS, OR OTHER LIABILITY, ARISING OUT OF THE USE OF OR IN CONNECTION WITH
* THIS SOURCE CODE AND ITS DOCUMENTATION.
* =========================================================================== */
/**
* @file configurator.h
*
* @author ( MM Solutions AD )
*
*/
/* -----------------------------------------------------------------------------
*!
*! Revision History
*! ===================================
*! 05-Nov-2013 : Author ( MM Solutions AD )
*! Created
* =========================================================================== */

#ifndef _CONFIGURATOR_H
#define _CONFIGURATOR_H

typedef struct configurator configurator_t;
typedef struct configurator_client configurator_client_t;
typedef struct configurator_client_desc configurator_client_desc_t;
typedef struct configurator_plugin_desc configurator_plugin_desc_t;
typedef struct configurator_create_param configurator_create_param_t;
typedef struct configurator_config_struct_node configurator_config_struct_node_t;
typedef struct configurator_config_struct configurator_config_struct_t;

/*********************************************
* Common Interface ***************************
**********************************************/
#define CONFIGURATOR_CONFIG_STRUCT_ROOT_NODE 0xFFFFFFFF

struct configurator;

struct configurator_create_param {
    configurator_plugin_desc_t *plugin;
    void *plugin_prv;
    int num_of_configs;
};

struct configurator_config_struct_node {
    unsigned int seq_num;
    int parrent_offset;
};

struct configurator_config_struct {
    configurator_config_struct_node_t node;
};

configurator_t *configurator_create(
        configurator_create_param_t *create_param
    );
void configurator_destroy(configurator_t *c);

/*********************************************
* Source Interface ***************************
**********************************************/
int configurator_get(configurator_t *c, unsigned int index, void *data);
int configurator_set_begin(configurator_t *c);
int configurator_set_end(configurator_t *c);
void configurator_set_cancel(configurator_t *c);
int configurator_set(configurator_t *c, unsigned int index, void *data);

/*********************************************
* Client Interface ***************************
**********************************************/
struct configurator_client;

struct configurator_client_desc {
    char *name;
    int (*registered)(
            configurator_t *c,
            configurator_client_t *client,
            void *client_prv,
            void *config
        );
    int (*unregistered)(
            configurator_t *c,
            configurator_client_t *client,
            void *client_prv,
            void *config
        );
    int (*alternate)(
            configurator_t *c,
            configurator_client_t *client,
            void *client_prv,
            void *config
        );
    void (*notify)(
            configurator_t *c,
            configurator_client_t *client,
            void *client_prv,
            void *config
        );
};

configurator_client_t *configurator_client_register(
        configurator_t *c,
        configurator_client_desc_t *client_desc,
        void *client_prv
    );
int configurator_client_unregister(
        configurator_t *c,
        configurator_client_t *client
    );
void configurator_config_lock(void *config);
void configurator_config_unlock(void *config);
void configurator_config_touch(configurator_config_struct_node_t *node);

#define CONFIGURATOR_CONFIG_TOUCH(FIELD) \
    configurator_config_touch( \
            &(((configurator_config_struct_t*)&(FIELD))->node) \
        )

/*********************************************
* Plugin Interface ***************************
**********************************************/
struct configurator_plugin_desc {
    unsigned int config_struct_size;
    int (*get)(
            configurator_t *c,
            void *plugin_prv,
            void *config,
            unsigned int index,
            void *param
        );
    int (*set)(
            configurator_t *c,
            void *plugin_prv,
            void *config,
            unsigned int index,
            void *param
        );
    int (*set_to_default)(
            configurator_t *c,
            void *plugin_prv,
            void *config
        );
};

#endif /* _CONFIGURATOR_H */

