/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Fluent Bit
 *  ==========
 *  Copyright (C) 2025 The Fluent Bit Authors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <fluent-bit/unicode/flb_wchar.h>
#include <fluent-bit/unicode/flb_conv.h>
#include "maps/uhc_to_utf8.map"
#include "maps/utf8_to_uhc.map"

/* ----------
 * Returns the number of bytes successfully converted.
 * ----------
 */
int
flb_uhc_to_utf8(const unsigned char *src, unsigned char **dest,
                size_t len, bool no_error, int encoding)
{
    int converted = -1;

    converted = flb_convert_to_utf_internal(src, len, *dest,
                                            &uhc_to_unicode_tree,
                                            NULL, 0,
                                            NULL,
                                            FLB_UHC,
                                            no_error);

    return converted;
}

int
flb_utf8_to_uhc(const unsigned char *src, unsigned char **dest,
                size_t len, bool no_error, int encoding)
{
    int converted = -1;

    converted = flb_convert_to_local_internal(src, len, *dest,
                                              &uhc_from_unicode_tree,
                                              NULL, 0,
                                              NULL,
                                              FLB_UHC,
                                              no_error);

    return converted;
}

struct flb_unicode_converter uhc_converter = {
    .name = "UHC",
    .aliases = {"CP949", "Windows-949", NULL},
    .desc = "UHC encoding converter",
    .encoding = FLB_UHC,
    .max_width = 3,
    .cb_to_utf8 = flb_uhc_to_utf8,
    .cb_from_utf8 = flb_utf8_to_uhc,
};
