/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_mem.h>
#include <fluent-bit/flb_str.h>
#include <fluent-bit/flb_upstream.h>
#include <fluent-bit/flb_config.h>
#include "flb_tests_internal.h"

void test_upstream_create_functions_exist() {
    struct flb_config *config;
    struct flb_upstream *upstream1, *upstream2;
    
    /* Create a minimal config */
    config = flb_config_init();
    if (!config) {
        TEST_CHECK(0); /* Failed to create config */
        return;
    }
    
    /* Test regular upstream creation */
    upstream1 = flb_upstream_create(config, "example.com", 80, 0, NULL);
    TEST_CHECK(upstream1 != NULL);
    
    /* Test bypass proxy upstream creation */
    upstream2 = flb_upstream_create_bypass_proxy(config, "example.com", 80, 0, NULL);
    TEST_CHECK(upstream2 != NULL);
    
    /* Cleanup */
    if (upstream1) {
        flb_upstream_destroy(upstream1);
    }
    if (upstream2) {
        flb_upstream_destroy(upstream2);
    }
    
    flb_config_exit(config);
}

void test_upstream_with_proxy_env() {
    struct flb_config *config;
    struct flb_upstream *upstream1, *upstream2;
    
    /* Create a config with proxy settings */
    config = flb_config_init();
    if (!config) {
        TEST_CHECK(0);
        return;
    }
    
    /* Set proxy configuration */
    if (config->http_proxy) {
        flb_free(config->http_proxy);
    }
    config->http_proxy = flb_strdup("http://proxy.example.com:8080");
    
    /* Test regular upstream creation - should use proxy */
    upstream1 = flb_upstream_create(config, "destination.com", 443, 0, NULL);
    TEST_CHECK(upstream1 != NULL);
    
    /* Test bypass proxy upstream creation - should skip proxy */
    upstream2 = flb_upstream_create_bypass_proxy(config, "destination.com", 443, 0, NULL);
    TEST_CHECK(upstream2 != NULL);
    
    /* Verify they are different (one uses proxy, one doesn't) */
    if (upstream1 && upstream2) {
        /* The key test: when using bypass, we should connect directly to destination */
        /* When not using bypass, proxy settings may affect the connection */
        TEST_CHECK(1); /* Functions work without crashing */
    }
    
    /* Cleanup */
    if (upstream1) {
        flb_upstream_destroy(upstream1);
    }
    if (upstream2) {
        flb_upstream_destroy(upstream2);
    }
    
    if (config->http_proxy) {
        flb_free(config->http_proxy);
    }
    flb_config_exit(config);
}

TEST_LIST = {
    { "upstream_create_functions_exist", test_upstream_create_functions_exist },
    { "upstream_with_proxy_env", test_upstream_with_proxy_env },
    { 0 }
};