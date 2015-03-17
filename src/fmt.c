/**
 * @file fmt.c Formatting Testcode
 *
 * Copyright (C) 2010 Creytiv.com
 */
#include <string.h>
#include <stdlib.h>
#include <re.h>
#include "test.h"


#define DEBUG_MODULE "testfmt"
#define DEBUG_LEVEL 5
#include <re_dbg.h>


int test_fmt_pl(void)
{
	const struct pl pl   = PL("rattarei");
	const struct pl pl0  = PL("rattarei");
	const struct pl pl0_ = PL("rAtTaReI");
	const struct pl pl1  = PL("foobar");
	const struct pl pl2  = PL("rarei");
	const struct pl pl3  = PL("8zmoijdalij32li34jkljsldsjalkfj9jshruhga"
				  "laksjdliasjf98uasehr98wehrdisflsdifholis"
				  "djlweijrdisfjslkdfjslfjowiejrodflaijsdla"
				  "ksjdlaskdfjslkdfjsldfkjsdlfkjsdlkrtjqwli"
				  "ejsldfjsldfkjsdlfkjdiajsduhiafhurjiejidi");
	const struct pl pl4  = PL("rattarei4");
	const char str0[] = "rattarei";
	const char str1[] = "rattaray";
	struct pl pl5, pl6;
	const struct pl pl7   = PL("hei");
	const struct pl pl7_  = PL("Hei");
	const struct pl pl7__ = PL("Duz");

	/* pl_cmp() */
	if (EINVAL != pl_cmp(NULL, NULL))
		goto out;
	if (0 != pl_cmp(&pl, &pl0))
		goto out;
	if (0 == pl_cmp(&pl, &pl1))
		goto out;
	if (0 != pl_cmp(&pl, &pl0))
		goto out;
	if (0 == pl_cmp(&pl, &pl2))
		goto out;
	if (0 != pl_cmp(&pl3, &pl3))
		goto out;
	if (0 != pl_cmp(&pl_null, &pl_null))
		goto out;

	/* pl_casecmp() */
	if (EINVAL != pl_casecmp(NULL, NULL))
		goto out;
	if (0 != pl_casecmp(&pl, &pl0))
		goto out;
	if (0 != pl_casecmp(&pl, &pl0_))
		goto out;
	if (0 == pl_casecmp(&pl, &pl1))
		goto out;
	if (0 == pl_casecmp(&pl, &pl4))
		goto out;
	pl5.p = str0;
	pl5.l = 6;
	pl6.p = str1;
	pl6.l = 6;
	if (0 != pl_casecmp(&pl5, &pl6))
		goto out;
	if (0 != pl_casecmp(&pl, &pl0))
		goto out;
	if (0 != pl_casecmp(&pl, &pl0_))
		goto out;
	if (0 == pl_casecmp(&pl, &pl2))
		goto out;
	if (0 != pl_casecmp(&pl7, &pl7_))
		goto out;
	if (0 == pl_casecmp(&pl7, &pl7__))
		goto out;
	if (0 != pl_casecmp(&pl_null, &pl_null))
		goto out;

	/* pl_strcmp() */
	if (EINVAL != pl_strcmp(NULL, NULL))
		goto out;
	if (0 != pl_strcmp(&pl0, str0))
		goto out;
	if (0 == pl_strcmp(&pl0, str1))
		goto out;
	if (0 == pl_strcmp(&pl3, str0))
		goto out;

	/* pl_strcasecmp() */
	if (EINVAL != pl_strcasecmp(NULL, NULL))
		goto out;
	if (0 != pl_strcasecmp(&pl0_, str0))
		goto out;
	if (0 == pl_strcasecmp(&pl0_, str1))
		goto out;
	if (0 == pl_strcasecmp(&pl3, str0))
		goto out;

	/* pl_strchr() */
	if (pl0.p != pl_strchr(&pl0, 'r'))
		goto out;
	if (NULL != pl_strchr(&pl0, 'B'))
		goto out;

	return 0;
 out:
	return EINVAL;
}


int test_fmt_pl_u32(void)
{
	const struct {
		const struct pl pl;
		uint32_t v;
	} testv[] = {
		/* Error cases */
		{PL("hei"),  0},
		{PL("abc"),  0},
		{PL(""),     0},
		{{NULL, 2},  0},
		{{"fo", 0},  0},
		{PL("4294967296"), 0},
		{PL("18446744073709551616"), 0},

		/* Working cases */
		{PL("0"),    0},
		{PL("1"),    1},
		{PL("123"),  123},
		{PL("1234"), 1234},
		{PL("4294967295"), 4294967295U}
	};
	uint32_t i;
	int err = 0;

	for (i=0; i<ARRAY_SIZE(testv); i++) {
		const uint32_t v = pl_u32(&testv[i].pl);

		if (testv[i].v != v) {
			DEBUG_WARNING("pl_u32 test %u failed %llu != %llu\n",
				      i, testv[i].v, v);
			err = EINVAL;
			break;
		}
	}

	return err;
}


int test_fmt_pl_u64(void)
{
	const struct {
		const struct pl pl;
		uint64_t v;
	} testv[] = {
		/* Error cases */
		{PL("hei"),  0},
		{PL("abc"),  0},
		{PL(""),     0},
		{{NULL, 2},  0},
		{{"fo", 0},  0},
		{PL("18446744073709551616"), 0},

		/* Working cases */
		{PL("0"),    0},
		{PL("1"),    1},
		{PL("123"),  123},
		{PL("1234"), 1234},
		{PL("4294967295"), 4294967295UL},
		{PL("4294967296"), 4294967296ULL},
		{PL("18446744073709551615"), 18446744073709551615ULL}
	};
	uint32_t i;
	int err = 0;

	for (i=0; i<ARRAY_SIZE(testv); i++) {
		const uint64_t v = pl_u64(&testv[i].pl);

		if (testv[i].v != v) {
			DEBUG_WARNING("pl_u64 test %u failed %llu != %llu\n",
				      i, testv[i].v, v);
			err = EINVAL;
			break;
		}
	}

	return err;
}


int test_fmt_pl_x3264(void)
{
	const struct {
		const char *str;
		uint32_t x32;
		uint64_t x64;
	} testv[] = {
		/* Working cases */
		{"0",            0x0,        0x0              },
		{"1",            0x1,        0x1              },
		{"123",          0x123,      0x123            },
		{"1234",         0x1234,     0x1234           },
		{"abc",          0xabc,      0xabc            },
		{"94967295",     0x94967295, 0x94967295       },
		{"4bca9ef2",     0x4bca9ef2, 0x4bca9ef2       },
		{"18ab44cd8954", 0x44cd8954, 0x18ab44cd8954ULL},

		/* Error cases */
		{"hei",          0,          0                },
		{"",             0,          0                },
		{"0x123",        0,          0                },
		{",.!$%",        0,          0                },
	};
	uint32_t i;
	int err = 0;

	for (i=0; i<ARRAY_SIZE(testv); i++) {
		struct pl pl;
		uint32_t x32;
		uint64_t x64;

		pl_set_str(&pl, testv[i].str);

		x32 = pl_x32(&pl);
		x64 = pl_x64(&pl);

		if (testv[i].x32 != x32) {
			DEBUG_WARNING("pl_x32 test %u failed %x != %x\n",
				      i, testv[i].x32, x32);
			err = EINVAL;
			break;
		}

		if (testv[i].x64 != x64) {
			DEBUG_WARNING("pl_x64 test %u failed %lx != %lx\n",
				      i, testv[i].x64, x64);
			err = EINVAL;
			break;
		}
	}

	return err;
}


int test_fmt_regex(void)
{
	const struct pl pl  = PL("hei42sann!");
	const struct pl pl1 = PL("42");
	const struct pl pl2 = PL("sann");
	const struct pl pl3 = PL(";foo=\"bla;bla\"");
	const struct pl pl4 = PL("bla;bla");
	const struct pl pl5 = PL("a \"b \"1123\"\" c");
	const struct pl pl6 = PL("-42");
	const struct pl pla = PL("a");
	const struct pl plb = PL("b \"1123\"");
	const struct pl plc = PL("c");
	struct pl pln, pls, foo, a, b, c, d, e;
	int err = 0;

	/* Successful case */
	err = re_regex(pl.p, pl.l, "Hei[0-9]+[^!]+", &pln, &pls);
	if (err)
		goto out;
	err = pl_cmp(&pln, &pl1);
	if (err)
		goto out;
	err = pl_cmp(&pls, &pl2);
	if (err)
		goto out;

	/* Quoted strings */
	err = re_regex(pl3.p, pl3.l, ";foo=\"[^\"]+\"", &foo);
	if (err)
		goto out;
	err = pl_cmp(&foo, &pl4);
	if (err) {
		DEBUG_WARNING("regex quoted string failed (%r)\n", &foo);
		goto out;
	}

	err = re_regex(pl3.p, pl3.l, ";foo=[~]+", &foo);
	if (err)
		goto out;
	err = pl_cmp(&foo, &pl4);
	if (err)
		goto out;

	/* re_regex('a "b \"1123\"" c', "[^ ]+ [~ ]+ [^ ]+", &a, &b, &c);
	   result:  a='a', b='b \"1123\"', c='c' */
	err = re_regex(pl5.p, pl5.l, "[^ ]+ [~ ]+ [^ ]+", &a, &b, &c);
	if (err)
		goto out;
	err = pl_cmp(&a, &pla);
	if (err)
		goto out;
	err = pl_cmp(&b, &plb);
	if (err)
		goto out;
	err = pl_cmp(&c, &plc);
	if (err)
		goto out;

	/* Failing case */
	if (0 == re_regex(pl.p, pl.l, "tull")) {
		err = EINVAL;
		goto out;
	}

	if (0 == re_regex(pl.p, pl.l, "[^\r\n]+\r\n", &pln)) {
		err = EINVAL;
		goto out;
	}

	/* Test escaping */
	if (0 == re_regex(pl.p, pl.l, "[\\^0-9]*\\]", NULL)) {
		err = EINVAL;
		goto out;
	}
	err = re_regex(pl6.p, pl6.l, "[\\-0-9\\^]+", &pln);
	if (err)
		goto out;
	err = pl_strcmp(&pln, "-42");
	if (err)
		goto out;

	/* verify that optional matching sets the PL to zero
	   if there is no match */
	e.p = "x";
	e.l = 42;
	err = re_regex(pl4.p, pl4.l, "[a-z]+;[0-9]*", &d, &e);
	if (err)
		goto out;
	TEST_ASSERT(!pl_isset(&e));

	return 0;

 out:
	DEBUG_WARNING("regex failed (%d)\n", err);

	return err;
}


static int fooprint(struct re_printf *pf, void *arg)
{
	int a = *(int *)arg;

	return re_hprintf(pf, "[a=%d]", a);
}


static int va_printf(struct mbuf *mb, const char *fmt, ...)
{
	va_list ap;
	int err;

	va_start(ap, fmt);
	err = mbuf_printf(mb, "[%v]", fmt, &ap);
	va_end(ap);

	return err;
}


int test_fmt_print(void)
{
	const struct pl ref1 = PL("-12345 -1234567890 -1234567890123456789");
	const struct pl ref2 = PL("12345 1234567890 1234567890123456789");
	const struct pl ref3 = PL("65535 4294967295 18446744073709551615");
	struct pl pl;
	struct mbuf mb;
	const int a = 42;
	char *s = NULL;
	int err;

	mbuf_init(&mb);
	err = mbuf_printf(&mb, "%d %ld %lld", -12345, -1234567890L,
			  -1234567890123456789LL);
	if (err)
		goto out;
	pl.p = (char *)mb.buf;
	pl.l = mb.end;
	err = pl_cmp(&pl, &ref1);
	if (err) {
		DEBUG_WARNING("print 1: ref=(%r) buf=(%r)\n", &ref1, &pl);
		goto out;
	}

	mbuf_reset(&mb);
	err = mbuf_printf(&mb, "%u %lu %llu", 12345, 1234567890UL,
			  1234567890123456789ULL);
	if (err)
		goto out;
	pl.p = (char *)mb.buf;
	pl.l = mb.end;
	err = pl_cmp(&pl, &ref2);
	if (err) {
		DEBUG_WARNING("print 2: buf: %r\n", &pl);
		goto out;
	}

	mbuf_reset(&mb);
	err = mbuf_printf(&mb, "%u %lu %llu", 65535, 4294967295UL,
			  18446744073709551615ULL);
	if (err)
		goto out;
	pl.p = (char *)mb.buf;
	pl.l = mb.end;
	err = pl_cmp(&pl, &ref3);
	if (err) {
		DEBUG_WARNING("print 3: buf: %r\n", &pl);
		goto out;
	}

	mbuf_reset(&mb);
	err = mbuf_printf(&mb, "fookokaoskdokoskdookokokq%Hbar", fooprint, &a);
	if (err)
		goto out;
	pl.p = (char *)mb.buf;
	pl.l = mb.end;
	err = pl_strcmp(&pl, "fookokaoskdokoskdookokokq[a=42]bar");
	if (err) {
		DEBUG_WARNING("print 4: buf: %r\n", &pl);
		goto out;
	}

	mbuf_reset(&mb);
	err = va_printf(&mb, "foo%d%s", 42, "barrompabarplaplsdpalspdlplplp");
	if (err)
		goto out;
	pl.p = (char *)mb.buf;
	pl.l = mb.end;
	err = pl_strcmp(&pl, "[foo42barrompabarplaplsdpalspdlplplp]");
	if (err) {
		DEBUG_WARNING("print 5: buf: %r\n", &pl);
		goto out;
	}

	/* dynamic print */
	err = re_sdprintf(&s, "okaspdokaspodkjalsj%fkmzl12kpdokasdlkj", 3.14);
	if (err)
		goto out;

	pl_set_str(&pl, s);
	err = pl_strcmp(&pl, "okaspdokaspodkjalsj3.140000kmzl12kpdokasdlkj");
	if (err) {
		DEBUG_WARNING("sdprintf: %r\n", &pl);
		goto out;
	}

 out:
	mbuf_reset(&mb);
	mem_deref(s);

	return err;
}


int test_fmt_snprintf(void)
{
	const struct pl ref3 = PL("65535 4294967295 18446744073709551615");
	const uint8_t v[] = {0xfa, 0xce, 0xb0, 0x0c};
	struct sa sa4;
	const char addr4[] = "1.2.3.4";
#ifdef HAVE_INET6
	struct sa sa6;
	const char addr6[] = "2001:5c0:8fff:ffff::d";
#endif
	char buf[128], sbuf[8];
	int n, err;

	/* Test binary vector printing */
	n = re_snprintf(buf, sizeof(buf), "%w", v, sizeof(v));
	if (2*sizeof(v) != n) {
		err = EINVAL;
		goto out;
	}
	if (0 != strcmp(buf, "faceb00c")) {
		err = EINVAL;
		goto out;
	}

	/* Test sockaddr printing */
	err = sa_set_str(&sa4, addr4, 0);
	if (err) {
		DEBUG_WARNING("sa_set_str4: %m\n", err);
		goto out;
	}
#ifdef HAVE_INET6
	err = sa_set_str(&sa6, addr6, 0);
	if (err) {
		DEBUG_WARNING("sa_set_str6: %m\n", err);
		goto out;
	}
#endif

	(void)re_snprintf(buf, sizeof(buf), "%j", &sa4);
	if (0 != strcmp(buf, addr4)) {
		err = EINVAL;
		goto out;
	}
#ifdef HAVE_INET6
	(void)re_snprintf(buf, sizeof(buf), "%j", &sa6);
	if (0 != strcmp(buf, addr6)) {
		err = EINVAL;
		goto out;
	}
#endif

	/* Overflow */
	n = re_snprintf(buf, 3, "12");
	if (2 != n) {
		err = EINVAL;
		goto out;
	}
	n = re_snprintf(buf, 3, "123");
	if (-1 != n) {
		err = EINVAL;
		goto out;
	}
	n = re_snprintf(buf, 4, "%u", 12345);
	if (-1 != n) {
		err = EINVAL;
		goto out;
	}
	n = re_snprintf(buf, 4, "%s", "asdasd");
	if (-1 != n) {
		err = EINVAL;
		goto out;
	}
	n = re_snprintf(buf, 37, "%r", &ref3);
	if (-1 != n) {
		err = EINVAL;
		goto out;
	}

	/* Double */
	(void)re_snprintf(buf, sizeof(buf), "%f", 123.456);
	if (0 != strcmp(buf, "123.456000"))
		goto perr;
	(void)re_snprintf(buf, sizeof(buf), "%f", -123.456);
	if (0 != strcmp(buf, "-123.456000"))
		goto perr;
	(void)re_snprintf(buf, sizeof(buf), "%.3f", 123.456);
	if (0 != strcmp(buf, "123.456"))
		goto perr;
	(void)re_snprintf(buf, sizeof(buf), "%6.3f", 3.14);
	if (0 != strcmp(buf, " 3.140"))
		goto perr;
	(void)re_snprintf(buf, sizeof(buf), "%06.3f", 3.14);
	if (0 != strcmp(buf, "03.140"))
		goto perr;
	(void)re_snprintf(buf, sizeof(buf), "%6.3f", -3.14);
	if (0 != strcmp(buf, "-3.140"))
		goto perr;

#if 0
	(void)re_snprintf(buf, sizeof(buf), "%05f", strtod("inf", NULL));
	if (0 != strcmp(buf, "  inf"))
		goto perr;
#endif

	(void)re_snprintf(buf, sizeof(buf), "%.2f", 123123123123.00);
	if (0 != strcmp(buf, "123123123123.00"))
		goto perr;

	memset(sbuf, 0xff, sizeof(sbuf));
	n = re_snprintf(sbuf, sizeof(sbuf), "ab %d cd", 42);
	if (n != -1 || strcmp(sbuf, "ab 42")) {
		DEBUG_WARNING("n=%d sbuf='%s'\n", n, sbuf);
		goto perr;
	}

 out:
	return err;
 perr:
	DEBUG_WARNING("bad msg: '%s'\n", buf);
	return EBADMSG;
}


int test_fmt_str(void)
{
	const struct {
		const char *dst;
		const char *src;
		uint32_t n;
	} testv[] = {
		{"foo", "foo", 64},
		{"foo", "foo", 4},
		{"fo",  "foo", 3},
		{"123456789",  "1234567890", 10}
	};
	char buf[64];
	size_t i;
	int err = 0;

	for (i=0; i<ARRAY_SIZE(testv); i++) {
		const size_t sz = strlen(testv[i].dst) + 1;
		str_ncpy(buf, testv[i].src, testv[i].n);

		if (0 != memcmp(testv[i].dst, buf, sz)) {
			DEBUG_WARNING("test %u failed %s != %s\n",
				      i, buf, testv[i].dst);
			err = EINVAL;
			break;
		}
	}

	return err;
}


static const struct {
	const char *pname;
	const char *pval;
	bool present;
} testv[] = {
	/* existing parameters: */
	{"bitrate",     "42",  true},
	{"foo",         0,     true},
	{"bar",         "BAR", true},

	/* non-existing parameters: */
	{"xbitrate",    0,     false},
	{"bitratex",    0,     false},
	{"itrat",       0,     false},
	{"fo",          0,     false},
	{"oo",          0,     false},
};
static const struct pl params = PL("bitrate=42; foo ; bar = \"BAR\"");


static void fmt_param_handler(const struct pl *name, const struct pl *val,
			      void *arg)
{
	void **argv = arg;
	size_t *i = argv[0];
	int *err = argv[1];

	if (*i >= ARRAY_SIZE(testv)) {
		DEBUG_WARNING("param: too many parameters (%u > %u)\n",
			      *i, ARRAY_SIZE(testv));
		*err = EOVERFLOW;
		return;
	}

	if (!testv[*i].present) {
		DEBUG_WARNING("param: %u: unexpected param '%r'\n",
			      *i, name);
		*err = EBADMSG;
	}

	if (0 != pl_strcmp(name, testv[*i].pname)) {
		DEBUG_WARNING("param: %u: name mismatch: '%r' != '%s'\n",
			      *i, name, testv[*i].pname);
		*err = EBADMSG;
	}

	if (testv[*i].pval && 0 != pl_strcmp(val, testv[*i].pval)) {
		DEBUG_WARNING("param: %u: value mismatch: '%r' != '%s'\n",
			      *i, val, testv[*i].pval);
		*err = EBADMSG;
	}

	++(*i);
}


int test_fmt_param(void)
{
	size_t i;
	int err = 0;
	void *argv[2];

	argv[0] = &i;
	argv[1] = &err;

	for (i=0; i<ARRAY_SIZE(testv); i++) {
		struct pl val;
		bool res;

		res = fmt_param_exists(&params, testv[i].pname);
		if (!res != !testv[i].present) {
			DEBUG_WARNING("%u: unexpected %d != %d\n",
				      i, testv[i].present, res);
			return EBADMSG;
		}

		if (!testv[i].pval)
			continue;

		res = fmt_param_get(&params, testv[i].pname, &val);
		if (!res != !testv[i].present) {
			DEBUG_WARNING("%u: unexpected %d != %d\n",
				      i, testv[i].present, res);
			return EBADMSG;
		}

		if (res && 0 != pl_strcmp(&val, testv[i].pval)) {
			DEBUG_WARNING("%u: value mismatch %r != %s\n",
				      i, &val, testv[i].pval);
			return EINVAL;
		}
	}

	i = 0;
	fmt_param_apply(&params, fmt_param_handler, argv);

	return err;
}


int test_fmt_human_time(void)
{
	const char ref1[] = "1 day 2 hours 3 mins 4 secs";
	const char ref2[] = "1 min 2 secs";
	char buf[256];
	uint32_t sec;
	int err = 0;

	sec = 1*24*60*60 + 2*60*60 + 3*60 + 4;
	(void)re_snprintf(buf, sizeof(buf), "%H", fmt_human_time, &sec);
	TEST_STRCMP(ref1, strlen(ref1), buf, strlen(buf));

	sec = 0*24*60*60 + 0*60*60 + 1*60 + 2;
	(void)re_snprintf(buf, sizeof(buf), "%H", fmt_human_time, &sec);
	TEST_STRCMP(ref2, strlen(ref2), buf, strlen(buf));

 out:
	return err;
}


int test_fmt_str_error(void)
{
	char buf[256];
	int err = 0;

	TEST_ASSERT(str_isset(str_error(EINVAL, buf, sizeof(buf))));
 out:
	return err;
}