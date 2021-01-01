#include "MyCjson.h"
#include<assert.h>
#include<string>
#include<iostream>
#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')
void json_init(json_value* v)
{

}
void json_free(json_value* v)
{

}
static void json_parse_whitespace(json_context* c)
{
    int i=0;
    while((*c).json[i]==' '||(*c).json[i]=='\t'||(*c).json[i]=='\n'||(*c).json[i]=='\r')
        i++;
    c->json=c->json.substr(i);
}
static int json_parse_literal(json_context* c, json_value* v, const char* literal, json_type type) {
    size_t i;
    assert(c->json[0]==literal[0]);
    for (i = 0; literal[i + 1]; i++)
        if (c->json[i] != literal[i])
            return JSON_PARSE_INVALID_VALUE;
    c->json=c->json.substr(i);
    v->type = type;
    return JSON_PARSE_OK;
}
static int json_parse_number(json_context* c, json_value* v) {
    //const char* p = c->json;
    size_t i=0;
    std::string::iterator p=c->json.begin();
    if (*p == '-') {p++;}
    if (*p == '0') {p++;}
    else {
        if (!ISDIGIT1TO9(*p)) return JSON_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    if (*p == '.') {
        p++;
        if (!ISDIGIT(*p)) return JSON_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    if (*p == 'e' || *p == 'E') {
        p++;
        if (*p == '+' || *p == '-') p++;
        if (!ISDIGIT(*p)) return JSON_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    errno = 0;
    v->u.n=atof(c->json.c_str());
    if (errno == ERANGE && (v->u.n == HUGE_VAL || v->u.n == -HUGE_VAL))
        return JSON_PARSE_NUMBER_TOO_BIG;
    v->type = JSON_NUMBER;
    i=p-c->json.begin();
    c->json = c->json.substr(i);
    return JSON_PARSE_OK;
}
static int json_parse_string(json_context* c, json_value* v) {
    int ret;
    char* s;
    std::string::iterator p=c->json.begin();
    for(;;)
    {
        char ch=*p++;
        switch (ch)
        {
        case '\"':

            return JSON_PARSE_OK;
        case '\\':
            switch (*p++)
            {
                
            }
        default:
            break;
        }
    }
    return ret;
}
static int json_parse_value(json_context* c, json_value* v) {
    switch ((*c).json[0]) {
        case 't':  return json_parse_literal(c, v, "true", JSON_TRUE);
        case 'f':  return json_parse_literal(c, v, "false", JSON_FALSE);
        case 'n':  return json_parse_literal(c, v, "null", JSON_NULL);
        case '"':  return json_parse_string(c, v);
        default:   return json_parse_number(c, v);
        case '\0': return JSON_PARSE_EXPECT_VALUE;
    }
}
void json_set_boolean(json_value* v,int b)
{
    json_free(v);
    v->type = b ? JSON_TRUE : JSON_FALSE;
}
int json_parse(json_value* v, const std::string json)
{
    json_context c;
    int ret;
    assert(v!=NULL);
    c.json=json;
    json_parse_whitespace(&c);
    if((ret = json_parse_value(&c,v))==JSON_PARSE_OK){
        json_parse_whitespace(&c);
        if(*c.json.end()!='\0'){
            v->type = JSON_NULL;
            ret = JSON_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}
json_type json_get_type(const json_value* v)
{
    assert(v!=NULL);
    return v->type;
}
double json_get_number(const json_value* v)
{
    assert(v!=NULL);
    return v->u.n;
}
std::string json_get_string(const json_value* v)
{
    assert(v!=NULL);
    return v->s;
}