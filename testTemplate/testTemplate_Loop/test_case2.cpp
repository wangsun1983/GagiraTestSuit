#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"
#include "ForEveryOne.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(TestCase2_Data) {
public:
    int value;
    DECLARE_REFLECT_FIELD(TestCase2_Data,value)
};

DECLARE_CLASS(TestCase2_Object) {
public:
    HashMap<TestCase2_Data,TestCase2_Data> map;
    DECLARE_REFLECT_FIELD(TestCase2_Object,map)
};

void test_case2() {    
    auto input = FileInputStream::New("./test_case/case2.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    auto obj = TestCase2_Object::New();
    obj->map = HashMap<TestCase2_Data,TestCase2_Data>::New();
    auto key1 = TestCase2_Data::New();
    auto value1 = TestCase2_Data::New();
    key1->value = 1;
    value1->value = 2;
    obj->map->put(key1,value1);
    
    auto key2 = TestCase2_Data::New();
    auto value2 = TestCase2_Data::New();
    key2->value = 3;
    value2->value = 4;
    obj->map->put(key2,value2);
    auto result = t->execute(scopedValues,obj);
    
    int i = 1;
    ForEveryOne(pair,obj->map) {
        if(pair->getKey()->value != i+100 ||
           pair->getValue()->value != i+100 ) {
              TEST_FAIL("testTemplate_Loop test_case2 case1");
           }
        i++;
    }
    
    TEST_OK("testTemplate_Loop test_case2 case100");
}
