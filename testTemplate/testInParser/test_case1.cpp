#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "TemplateLetParser.hpp"
#include "TemplateLetItem.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"
#include "FileInputStream.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(BasicData) {
public:
      int basic_data;
      DECLARE_REFLECT_FIELD(BasicData,basic_data);
};

DECLARE_CLASS(BasicData1) {
public:
    BasicData data1;
    BasicData data2;
    DECLARE_REFLECT_FIELD(BasicData1,data1,data2);
};

void test_case1() {    
    auto input = FileInputStream::New("./test_case/case1.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    BasicData1 testdata = BasicData1::New();
    testdata->data1 = BasicData::New();
    testdata->data2 = BasicData::New();
    auto result = t->execute(scopedValues,testdata);
    
    printf("data1 is %d,data2 is %d \n",
            testdata->data1->basic_data,
            testdata->data2->basic_data);
}
