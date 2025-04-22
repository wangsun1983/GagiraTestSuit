#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "TemplateScopedValueSeeker.hpp"

using namespace obotcha;
using namespace gagira;

void basic_test() {    
    String tag1 = String::New("a.b");
    st(TemplateScopedValueSeeker)::Get(tag1,nullptr,nullptr);
    
    String tag2 = String::New(".b");
    st(TemplateScopedValueSeeker)::Get(tag2,nullptr,nullptr);
    
    String tag3 = String::New("b");
    st(TemplateScopedValueSeeker)::Get(tag3,nullptr,nullptr);
}
