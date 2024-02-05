#include "minunit.h"
#include <simdstr/search.h>
#include <stdio.h>

static const char *data = "Liane reindorsing two-time zippering chromolithography rainbowweed "
                          "Cacatua bunking cooptions zinckenite Polygala "
                          "smooth-bellied chirognostic inkos BVM antigraphy pagne bicorne "
                          "complementizer commorant ever-endingly sheikhly\n"
                          "glam predamaged objectionability evil-looking quaquaversal\n"
                          "composite halter-wise mosasaur Whelan coleopterist grass-grown Helladic "
                          "DNB nondeliriousness arpents uncasing "
                          "predepletion delator unnaive sucken solid-gold brassards tutorials "
                          "refrangible terebras autobiographal mid-breast "
                          "rim-fire yallow soapfish trammelled Fangio thermocauteries river-drift "
                          "micromeasurement BSMetE tiddling Delaunay "
                          "prebendaryship depigment Cranwell Fruma sickly-seeming Abnaki Woodlake "
                          "unknocked Vanorin sudds long-shadowed\n"
                          "reciters Platypoda expdt imperious ensheathes diallagic indigo-yielding "
                          "enforcive Ibilao jubilus meisje sitively "
                          "town-bound leukocytoblast serpuline Camb widespreadly archsnob "
                          "convocational nonelliptically turfless nonagent "
                          "intellectually arm-great unperceptional robotries campestral hyaenid "
                          "mispublicized Othoniel ultraistic nemophily "
                          "babel's overhate one-storied unkodaked Ogden Gore Gekkonidae Hymenolepis "
                          "nonheritable TMR poppy-crowned inclipped "
                          "well-centered by-job crop-tailed vagrantism condescensively\1";

void
test_setup (void)
{
        /* Nothing */
}

void
test_teardown (void)
{
        /* Nothing */
}

MU_TEST (test_simdstr_search_strchr)
{
        {
                const char *match = simd_strchr (data, strlen (data), 'L');
                mu_assert_string_eq (data, match);
        }
        {
                const char *match = simd_strchr (data + 1, strlen (data), 'L');
                mu_check (match == NULL);
        }
        {
                const char *match = simd_strchr (data, strlen (data), 'l');
                mu_assert_string_eq (data + 43, match);
        }
        {
                const char *match = simd_strchr (data + 44, strlen (data), 'l');
                mu_assert_string_eq (data + 106, match);
        }
        {
                const char *match = simd_strchr (data, strlen (data), '\n');
                mu_assert_string_eq (data + 223, match);
        }
        {
                const char *match = simd_strchr (data + 100, strlen (data), '\n');
                mu_assert_string_eq (data + 223, match);
        }
        {
                const char *match = simd_strchr (data, strlen (data), 'Z');
                mu_check (match == NULL);
        }
        {
                const char *match = simd_strchr (data, strlen (data), '\1');
                mu_assert_string_eq ("\1", match);
        }
}

MU_TEST (test_simdstr_search_strichr)
{
        {
                const char *match = simd_strichr (data, strlen (data), 'L');
                mu_assert_string_eq (data, match);
        }
        {
                const char *match = simd_strichr (data + 1, strlen (data), 'L');
                mu_assert_string_eq(data + 43, match);
        }
        {
                const char *match = simd_strichr (data, strlen (data), 'l');
                mu_assert_string_eq (data, match);
        }
        {
                const char *match = simd_strichr (data + 44, strlen (data), 'l');
                mu_assert_string_eq (data + 106, match);
        }
        {
                const char *match = simd_strichr (data + 44, strlen (data), 'l');
                mu_assert_string_eq (data + 106, match);
        }
        {
                const char *match = simd_strichr (data, strlen (data), '\n');
                mu_assert_string_eq (data + 223, match);
        }
        {
                const char *match = simd_strichr (data + 100, strlen (data), '\n');
                mu_assert_string_eq (data + 223, match);
        }
        {
                const char *match = simd_strichr (data, strlen (data), 'Z');
                mu_assert_string_eq (data + 27, match);
        }
        {
                const char *match = simd_strichr (data, strlen (data), 'Z');
                mu_assert_string_eq (data + 27, match);
        }
        {
                const char *match = simd_strichr (data, strlen (data), '\1');
                mu_assert_string_eq ("\1", match);
        }
        {
                const char *match = simd_strchr (data, strlen (data), '_');
                mu_check (match == NULL);
        }
}

MU_TEST (test_simdstr_search_strstr)
{
        {
                //const char *match = simd_strstr (data, strlen (data), "zipperigg", 9);
                //mu_check(match == NULL);
        }
        {
                const char *match = simd_strstr (data, strlen (data), "dorsing", 7);
                //printf("\n..\n%lu\n..\n", strlen (match));
                mu_assert_string_eq(strstr(data, "dorsing"), match);
        }
        {
                const char *match = simd_strstr (data, strlen (data), "zippering", 9);
                //printf("\n..\n%lu\n..\n", strlen (match));
                mu_assert_string_eq(strstr(data, "zippering"), match);
        }
        {
                const char *match = simd_strichr (data, strlen (data), 'l');
                mu_assert_string_eq (data, match);
        }
        {
                const char *match = simd_strichr (data + 44, strlen (data), 'l');
                mu_assert_string_eq (data + 106, match);
        }
        {
                const char *match = simd_strichr (data + 44, strlen (data), 'l');
                mu_assert_string_eq (data + 106, match);
        }
        {
                const char *match = simd_strichr (data, strlen (data), '\n');
                mu_assert_string_eq (data + 223, match);
        }
        {
                const char *match = simd_strichr (data + 100, strlen (data), '\n');
                mu_assert_string_eq (data + 223, match);
        }
        {
                const char *match = simd_strichr (data, strlen (data), 'Z');
                mu_assert_string_eq (data + 27, match);
        }
        {
                const char *match = simd_strichr (data, strlen (data), 'Z');
                mu_assert_string_eq (data + 27, match);
        }
        {
                const char *match = simd_strichr (data, strlen (data), '\1');
                mu_assert_string_eq ("\1", match);
        }
        {
                const char *match = simd_strchr (data, strlen (data), '_');
                mu_check (match == NULL);
        }
}

MU_TEST_SUITE (test_suite)
{
        MU_SUITE_CONFIGURE (&test_setup, &test_teardown);

        MU_RUN_TEST (test_simdstr_search_strchr);
        MU_RUN_TEST (test_simdstr_search_strichr);

        MU_RUN_TEST (test_simdstr_search_strstr);
}

int
main (int argc, char **argv)
{
        MU_RUN_SUITE (test_suite);
        MU_REPORT ();
        return MU_EXIT_CODE;
}