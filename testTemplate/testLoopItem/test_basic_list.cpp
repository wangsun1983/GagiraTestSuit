#include <stdio.h>
#include <unistd.h>

#include "Template.hpp"
#include "TemplateFunctionParser.hpp"
#include "Log.hpp"
#include "TestLog.hpp"
#include "ArrayList.hpp"
#include "Reflect.hpp"
#include "FileInputStream.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(BasicListData) {
public:
    int value;
    DECLARE_REFLECT_FIELD(BasicListData,value)
};

DECLARE_CLASS(TestList) {
public:
    ArrayList<BasicListData> list;
    DECLARE_REFLECT_FIELD(TestList,list)
};

void test_basic_list() {
    TestList ll = TestList::New();
    ll->list = ArrayList<BasicListData>::New();
    
    auto item1 = BasicListData::New();
    item1->value = 11;
    
    auto item2 = BasicListData::New();
    item2->value = 22;
    
    auto list = ArrayList<BasicListData>::New();
    ll->list->add(item1);
    ll->list->add(item2);
    
    auto input = FileInputStream::New("./test_case/case1.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto result = t->execute(scopedValues,ll);
    
    TEST_OK("testLoopItem basic list case100");
}
