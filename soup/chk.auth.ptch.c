--- ref/auth.reference.json.c	2024-03-02 05:59:50
+++ chk_sem_auth.c	2024-03-02 06:00:54
@@ -40,24 +40,24 @@
 struct json_sem sem_auth[SEM_AUTH_LEN+1] = {
 /* depth    type        min     max   count   index  name_len validate  name */
   { 5,	JTYPE_NUMBER,	1,	5,	5,	0,	0,	NULL,	NULL },
-  { 5,	JTYPE_STRING,	1,	104,	104,	1,	0,	NULL,	NULL },
-  { 5,	JTYPE_BOOL,	1,	10,	10,	2,	0,	NULL,	NULL },
-  { 5,	JTYPE_NULL,	1,	1,	1,	3,	0,	NULL,	NULL },
+  { 5,	JTYPE_STRING,	15,	104,	104,	1,	0,	NULL,	NULL },
+  { 5,	JTYPE_BOOL,	2,	10,	10,	2,	0,	NULL,	NULL },
+  { 5,	JTYPE_NULL,	0,	30,	30,	3,	0,	NULL,	NULL },
   { 4,	JTYPE_MEMBER,	1,	5,	5,	4,	11,	chk_affiliation,	"affiliation" },
-  { 4,	JTYPE_MEMBER,	1,	5,	5,	5,	7,	chk_alt_url,	"alt_url" },
-  { 4,	JTYPE_MEMBER,	1,	5,	5,	6,	13,	chk_author_handle,	"author_handle" },
-  { 4,	JTYPE_MEMBER,	1,	5,	5,	7,	13,	chk_author_number,	"author_number" },
-  { 4,	JTYPE_MEMBER,	1,	5,	5,	8,	14,	chk_default_handle,	"default_handle" },
-  { 4,	JTYPE_MEMBER,	1,	5,	5,	9,	5,	chk_email,	"email" },
-  { 4,	JTYPE_MEMBER,	1,	5,	5,	10,	6,	chk_github,	"github" },
-  { 4,	JTYPE_MEMBER,	1,	5,	5,	11,	13,	chk_location_code,	"location_code" },
-  { 4,	JTYPE_MEMBER,	1,	5,	5,	12,	8,	chk_mastodon,	"mastodon" },
-  { 4,	JTYPE_MEMBER,	1,	5,	5,	13,	4,	chk_name,	"name" },
-  { 4,	JTYPE_MEMBER,	1,	5,	5,	14,	19,	chk_past_winning_author,	"past_winning_author" },
-  { 4,	JTYPE_MEMBER,	1,	5,	5,	15,	3,	chk_url,	"url" },
+  { 4,	JTYPE_MEMBER,	1,	5,	5,	5,	13,	chk_author_handle,	"author_handle" },
+  { 4,	JTYPE_MEMBER,	1,	5,	5,	6,	13,	chk_author_number,	"author_number" },
+  { 4,	JTYPE_MEMBER,	1,	5,	5,	7,	14,	chk_default_handle,	"default_handle" },
+  { 4,	JTYPE_MEMBER,	1,	5,	5,	8,	5,	chk_email,	"email" },
+  { 4,	JTYPE_MEMBER,	1,	5,	5,	9,	6,	chk_github,	"github" },
+  { 4,	JTYPE_MEMBER,	1,	5,	5,	10,	13,	chk_location_code,	"location_code" },
+  { 4,	JTYPE_MEMBER,	1,	5,	5,	11,	8,	chk_mastodon,	"mastodon" },
+  { 4,	JTYPE_MEMBER,	1,	5,	5,	12,	4,	chk_name,	"name" },
+  { 4,	JTYPE_MEMBER,	1,	5,	5,	13,	19,	chk_past_winning_author,	"past_winning_author" },
+  { 4,	JTYPE_MEMBER,	1,	5,	5,	14,	3,	chk_url,	"url" },
+  { 4,	JTYPE_MEMBER,	1,	5,	5,	15,	7,	chk_alt_url,	"alt_url" },
   { 3,	JTYPE_OBJECT,	1,	5,	5,	16,	0,	NULL,	NULL },
-  { 2,	JTYPE_NUMBER,	1,	6,	6,	17,	0,	NULL,	NULL },
-  { 2,	JTYPE_STRING,	1,	28,	28,	18,	0,	NULL,	NULL },
+  { 2,	JTYPE_NUMBER,	6,	6,	6,	17,	0,	NULL,	NULL },
+  { 2,	JTYPE_STRING,	28,	28,	28,	18,	0,	NULL,	NULL },
   { 2,	JTYPE_BOOL,	1,	1,	1,	19,	0,	NULL,	NULL },
   { 2,	JTYPE_ARRAY,	1,	1,	1,	20,	0,	NULL,	NULL },
   { 1,	JTYPE_MEMBER,	1,	1,	1,	21,	18,	chk_IOCCC_auth_version,	"IOCCC_auth_version" },
