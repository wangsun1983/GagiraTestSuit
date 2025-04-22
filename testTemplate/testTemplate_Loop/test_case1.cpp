#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(TestCase1_Data) {
public:
    int value;
    DECLARE_REFLECT_FIELD(TestCase1_Data,value)
};

DECLARE_CLASS(TestCase1_Object) {
public:
    ArrayList<TestCase1_Data> list;
    DECLARE_REFLECT_FIELD(TestCase1_Object,list)
};

void test_case1() {    
    auto input = FileInputStream::New("./test_case/case1.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    auto obj = TestCase1_Object::New();
    obj->list = ArrayList<TestCase1_Data>::New();
    TestCase1_Data data1 = TestCase1_Data::New();
    data1->value = 100;
    TestCase1_Data data2 = TestCase1_Data::New();
    data2->value = 200;
    obj->list->add(data1);
    obj->list->add(data2);
    
    auto result = t->execute(scopedValues,obj);
    auto v1 = obj->list->get(0);
    if(v1->value != 1) {
        TEST_FAIL("testTemplate_Loop test_case1 case1");
    }
    auto v2 = obj->list->get(1);
    if(v2->value != 2) {
        TEST_FAIL("testTemplate_Loop test_case1 case2");
    }
    
    TEST_OK("testTemplate_Loop test_case1 case100");
}
