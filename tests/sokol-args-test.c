//------------------------------------------------------------------------------
//  sokol-args-test.c
//------------------------------------------------------------------------------
#define SOKOL_IMPL
#include "sokol_args.h"
#include "test.h"

#define NUM_ARGS(x) (sizeof(x)/sizeof(void*))

static const char* argv_0[] = { "exe_name " };
static const char* argv_1[] = { "exe_name", "kvp0=val0", "kvp1=val1", "kvp2=val2" };
static const char* argv_2[] = { "exe_name", "kvp0  = val0 ", "  \tkvp1=  val1", "kvp2  = val2   "};
static const char* argv_3[] = { "exe_name", "kvp0:val0 ", "  kvp1:  val1", "kvp2 \t: val2   "};
static const char* argv_4[] = { "exe_name", "kvp0 ", "=val0 ", "  kvp1", "=", "val1", "kvp2 \t", "= val2   "};
static const char* argv_5[] = { "exe_name", "kvp0='bla bla'", "kvp1=' blub blub'", "kvp2='blob blob '"};
static const char* argv_6[] = { "exe_name", "kvp0=\"bla bla\"", "kvp1=\" blub blub\"", "kvp2=\"blob blob \""};

static void test_init_shutdown(void) {
    test("sokol-args init shutdown");
    sargs_setup(&(sargs_desc){0});
    T(sargs_isvalid());
    T(_sargs.max_args == _SARGS_MAX_ARGS_DEF);
    T(_sargs.args);
    T(_sargs.buf_size == _SARGS_BUF_SIZE_DEF);
    T(_sargs.buf_pos == 1);
    T(_sargs.buf);
    T(sargs_num_args() == 0);
    TSTR(sargs_key_at(0), "");
    TSTR(sargs_value_at(0), "");
    sargs_shutdown();
    T(!sargs_isvalid());
    T(0 == _sargs.args);
    T(0 == _sargs.buf);
}

static void test_no_args(void) {
    test("sokol-args simple args");
    sargs_setup(&(sargs_desc){
        .argc = NUM_ARGS(argv_0),
        .argv = argv_0
    });
    T(sargs_isvalid());
    T(sargs_num_args() == 0);
    TSTR(sargs_key_at(0), "");
    TSTR(sargs_value_at(0), "");
    T(-1 == sargs_find("bla"));
    T(!sargs_exists("bla"));
    TSTR(sargs_value("bla"), "");
    TSTR(sargs_value_def("bla", "blub"), "blub");
    sargs_shutdown();
}

static void test_simple_args(void) {
    test("sokol-args simple args");
    sargs_setup(&(sargs_desc){
        .argc = NUM_ARGS(argv_1),
        .argv = argv_1,
    });
    T(sargs_isvalid());
    T(sargs_num_args() == 3);
    T(0 == sargs_find("kvp0"));
    TSTR(sargs_value("kvp0"), "val0");
    TSTR(sargs_key_at(0), "kvp0");
    TSTR(sargs_value_at(0), "val0");
    T(1 == sargs_find("kvp1"));
    TSTR(sargs_value("kvp1"), "val1");
    TSTR(sargs_key_at(1), "kvp1");
    TSTR(sargs_value_at(1), "val1");
    T(2 == sargs_find("kvp2"));
    TSTR(sargs_value("kvp2"), "val2");
    TSTR(sargs_key_at(2), "kvp2");
    TSTR(sargs_value_at(2), "val2");
    T(_sargs.buf_pos == 31);
    sargs_shutdown();
}

static void test_simple_whitespace(void) {
    test("sokol-args simple whitespace");
    sargs_setup(&(sargs_desc){
        .argc = NUM_ARGS(argv_2),
        .argv = argv_2
    });
    T(sargs_isvalid());
    T(sargs_num_args() == 3);
    T(0 == sargs_find("kvp0"));
    TSTR(sargs_value("kvp0"), "val0");
    TSTR(sargs_key_at(0), "kvp0");
    TSTR(sargs_value_at(0), "val0");
    T(1 == sargs_find("kvp1"));
    TSTR(sargs_value("kvp1"), "val1");
    TSTR(sargs_key_at(1), "kvp1");
    TSTR(sargs_value_at(1), "val1");
    T(2 == sargs_find("kvp2"));
    TSTR(sargs_value("kvp2"), "val2");
    TSTR(sargs_key_at(2), "kvp2");
    TSTR(sargs_value_at(2), "val2");
    T(_sargs.buf_pos == 31);
    sargs_shutdown();
}

static void test_colon_separator(void) {
    test("sokol-args colon separator");
    sargs_setup(&(sargs_desc){
        .argc = NUM_ARGS(argv_3),
        .argv = argv_3
    });
    T(sargs_isvalid());
    T(sargs_num_args() == 3);
    T(0 == sargs_find("kvp0"));
    TSTR(sargs_value("kvp0"), "val0");
    TSTR(sargs_key_at(0), "kvp0");
    TSTR(sargs_value_at(0), "val0");
    T(1 == sargs_find("kvp1"));
    TSTR(sargs_value("kvp1"), "val1");
    TSTR(sargs_key_at(1), "kvp1");
    TSTR(sargs_value_at(1), "val1");
    T(2 == sargs_find("kvp2"));
    TSTR(sargs_value("kvp2"), "val2");
    TSTR(sargs_key_at(2), "kvp2");
    TSTR(sargs_value_at(2), "val2");
    T(_sargs.buf_pos == 31);
    sargs_shutdown();
}

static void test_standalone_separator(void) {
    test("sokol-args standalone separator");
    sargs_setup(&(sargs_desc){
        .argc = NUM_ARGS(argv_4),
        .argv = argv_4
    });
    T(sargs_isvalid());
    T(sargs_num_args() == 3);
    T(0 == sargs_find("kvp0"));
    TSTR(sargs_value("kvp0"), "val0");
    TSTR(sargs_key_at(0), "kvp0");
    TSTR(sargs_value_at(0), "val0");
    T(1 == sargs_find("kvp1"));
    TSTR(sargs_value("kvp1"), "val1");
    TSTR(sargs_key_at(1), "kvp1");
    TSTR(sargs_value_at(1), "val1");
    T(2 == sargs_find("kvp2"));
    TSTR(sargs_value("kvp2"), "val2");
    TSTR(sargs_key_at(2), "kvp2");
    TSTR(sargs_value_at(2), "val2");
    T(_sargs.buf_pos == 31);
    sargs_shutdown();
}

static void test_single_quotes(void) {
    test("sokol-args single quotes");
    sargs_setup(&(sargs_desc){
        .argc = NUM_ARGS(argv_5),
        .argv = argv_5
    });
    T(sargs_isvalid());
    T(sargs_num_args() == 3);
    T(0 == sargs_find("kvp0"));
    TSTR(sargs_value("kvp0"), "bla bla");
    TSTR(sargs_key_at(0), "kvp0");
    TSTR(sargs_value_at(0), "bla bla");
    T(1 == sargs_find("kvp1"));
    TSTR(sargs_value("kvp1"), " blub blub");
    TSTR(sargs_key_at(1), "kvp1");
    TSTR(sargs_value_at(1), " blub blub");
    T(2 == sargs_find("kvp2"));
    TSTR(sargs_value("kvp2"), "blob blob ");
    TSTR(sargs_key_at(2), "kvp2");
    TSTR(sargs_value_at(2), "blob blob ");
    sargs_shutdown();
}

static void test_double_quotes(void) {
    test("sokol-args double quotes");
    sargs_setup(&(sargs_desc){
        .argc = NUM_ARGS(argv_6),
        .argv = argv_6
    });
    T(sargs_isvalid());
    T(sargs_num_args() == 3);
    T(0 == sargs_find("kvp0"));
    TSTR(sargs_value("kvp0"), "bla bla");
    TSTR(sargs_key_at(0), "kvp0");
    TSTR(sargs_value_at(0), "bla bla");
    T(1 == sargs_find("kvp1"));
    TSTR(sargs_value("kvp1"), " blub blub");
    TSTR(sargs_key_at(1), "kvp1");
    TSTR(sargs_value_at(1), " blub blub");
    T(2 == sargs_find("kvp2"));
    TSTR(sargs_value("kvp2"), "blob blob ");
    TSTR(sargs_key_at(2), "kvp2");
    TSTR(sargs_value_at(2), "blob blob ");
    sargs_shutdown();
}

int main() {
    test_begin("sokol-args-test");
    test_init_shutdown();
    test_no_args();
    test_simple_args();
    test_simple_whitespace();
    test_colon_separator();
    test_standalone_separator();
    test_single_quotes();
    test_double_quotes();
    return test_end();
}

