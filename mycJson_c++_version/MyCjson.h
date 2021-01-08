#include<stddef.h>
#include<string>
#include<math.h>
#include<iostream>
typedef enum {JSON_NULL,JSON_FALSE,JSON_TRUE,JSON_NUMBER,JSON_STRING,JSON_ARRAY,JSON_OBJECT}json_type;

struct json_value
{
    union 
    {
        
        double n;
    }u;
    std::string s;
    json_type type;    
};

struct json_context{
    std::string json;
};
enum{
    JSON_PARSE_OK = 0,
    JSON_PARSE_EXPECT_VALUE,
    JSON_PARSE_INVALID_VALUE,
    JSON_PARSE_ROOT_NOT_SINGULAR,
    JSON_PARSE_NUMBER_TOO_BIG,
    JSON_PARSE_MISS_QUOTATION_MARK,
    JSON_PARSE_INVALID_STRING_ESCAPE,
    JSON_PARSE_INVALID_STRING_CHAR,
    JSON_PARSE_INVALID_UNICODE_HEX,
    JSON_PARSE_INVALID_UNICODE_SURROGATE,
    JSON_PARSE_MISS_KEY,
    JSON_PARSE_MISS_COLON,
    JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET
};

void json_init(json_value* v);
void json_set_boolean(json_value* v,int b);
void json_free(json_value* v);
int json_parse(json_value* v, const std::string json);
double json_get_number(const json_value* v);
std::string json_get_string(const json_value* v);
json_type json_get_type(const json_value* v);