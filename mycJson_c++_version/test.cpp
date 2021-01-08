#include "MyCjson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;
// #define EXPECT_EQ_BASE(equality, expect, actual, format) \
//     do {\
//         test_count++;\
//         if (equality)\
//             test_pass++;\
//         else {\
//             fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
//             main_ret = 1;\
//         }\
//     }while (0) 
template <class T,class U,class V,class N>
inline void EXPECT_EQ_BASE(T equality, U expect, V actual, N format)
{
    do{
        test_count++;
        if(equality)
            test_pass++;
        else
        {
            std::cout<<__FILE__<<":"<<__LINE__<<" :expect:"<<expect<<":actual: "<<actual<<std::endl;
            //fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);
            main_ret = 1;
        }        
    }while(0) ;  
}
// #define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")
template <class T,class U>
inline void EXPECT_EQ_INT(T expect,U actual)
{
    EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d");
}
// #define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g")
template <class T,class U>
inline void EXPECT_EQ_DOUBLE(T expect, U actual)
{
    EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g");
} 
// #define EXPECT_EQ_STRING(expect, actual)\
//     EXPECT_EQ_BASE(expect  == actual, expect, actual, "%s")
template <class T,class U>
inline void EXPECT_EQ_STRING(T expect,U actual)
{
     EXPECT_EQ_BASE(expect  == actual, expect, actual, "%s");
}
// #define EXPECT_TRUE(actual) EXPECT_EQ_BASE((actual) != 0, "true", "false", "%s")
template <class T>
inline void EXPECT_TRUE(T actual)
{
    EXPECT_EQ_BASE((actual) != 0, "true", "false", "%s");
}
// #define EXPECT_FALSE(actual) EXPECT_EQ_BASE((actual) == 0, "false", "true", "%s")
template <class T>
inline void EXPECT_FALSE(T actual) 
{
    EXPECT_EQ_BASE((actual) == 0, "false", "true", "%s");
}
static void test_parse_null() {
    json_value v;
    json_init(&v);
    json_set_boolean(&v, 0);
    EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "null"));
    EXPECT_EQ_INT(JSON_NULL, json_get_type(&v));
    json_free(&v);
}

static void test_parse_true() {
    json_value v;
    json_init(&v);
    json_set_boolean(&v, 0);
    EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "true"));
    EXPECT_EQ_INT(JSON_TRUE, json_get_type(&v));
    json_free(&v);
}

static void test_parse_false() {
    json_value v;
    json_init(&v);
    json_set_boolean(&v, 1);
    EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "false"));
    EXPECT_EQ_INT(JSON_FALSE, json_get_type(&v));
    json_free(&v);
}
#define TEST_STRING(expect,json)\
    do{\
        json_value v;\
        json_init(&v);\
        EXPECT_EQ_INT(JSON_PARSE_OK,json_parse(&v,json));\
        EXPECT_EQ_INT(JSON_STRING,json_get_type(&v));\
        EXPECT_EQ_STRING(expect,json_get_string(&v));\
        json_free(&v);\        
    }while(0)
#define TEST_NUMBER(expect, json)\
    do {\
        json_value v;\
        json_init(&v);\
        EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, json));\
        EXPECT_EQ_INT(JSON_NUMBER, json_get_type(&v));\
        EXPECT_EQ_DOUBLE(expect, json_get_number(&v));\
        json_free(&v);\
    } while(0)
static void test_parse_number() {
    TEST_NUMBER(0.0, "0");
    TEST_NUMBER(0.0, "-0");
    TEST_NUMBER(0.0, "-0.0");
    TEST_NUMBER(1.0, "1");
    TEST_NUMBER(-1.0, "-1");
    TEST_NUMBER(1.5, "1.5");
    TEST_NUMBER(-1.5, "-1.5");
    TEST_NUMBER(3.1416, "3.1416");
    TEST_NUMBER(1E10, "1E10");
    TEST_NUMBER(1e10, "1e10");
    TEST_NUMBER(1E+10, "1E+10");
    TEST_NUMBER(1E-10, "1E-10");
    TEST_NUMBER(-1E10, "-1E10");
    TEST_NUMBER(-1e10, "-1e10");
    TEST_NUMBER(-1E+10, "-1E+10");
    TEST_NUMBER(-1E-10, "-1E-10");
    TEST_NUMBER(1.234E+10, "1.234E+10");
    TEST_NUMBER(1.234E-10, "1.234E-10");
    TEST_NUMBER(0.0, "1e-10000"); /* must underflow */

    TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */
    TEST_NUMBER( 4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
    TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
    TEST_NUMBER( 2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
    TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
    TEST_NUMBER( 2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
    TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
    TEST_NUMBER( 1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
    TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}
static void test_parse_string() {
    TEST_STRING("", "\"\"");
    TEST_STRING("Hello", "\"Hello\"");
    TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
    TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
    // TEST_STRING("Hello\0World", "\"Hello\\u0000World\"");
    // TEST_STRING("\x24", "\"\\u0024\"");         /* Dollar sign U+0024 */
    // TEST_STRING("\xC2\xA2", "\"\\u00A2\"");     /* Cents sign U+00A2 */
    // TEST_STRING("\xE2\x82\xAC", "\"\\u20AC\""); /* Euro sign U+20AC */
    // TEST_STRING("\xF0\x9D\x84\x9E", "\"\\uD834\\uDD1E\"");  /* G clef sign U+1D11E */
    // TEST_STRING("\xF0\x9D\x84\x9E", "\"\\ud834\\udd1e\"");  /* G clef sign U+1D11E */
}
static void test_parse()
{
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_number();
    test_parse_string();
}
int main()
{
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
}