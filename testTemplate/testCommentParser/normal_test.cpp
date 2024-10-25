#include "Template.hpp"

using namespace obotcha;
using namespace gagira;

int normal_test() {
    String case1_str = "{{/*fff \r\n ggg\r\n\r\n*/}}";
    auto htmlTemplate = Template::New(case1_str);
    
    String case2_str = "{{/*f2*/}}";
    auto htmlTemplate2 = Template::New(case2_str);
    
    String case3_str = "{{/*f3\n*/}}";
    auto htmlTemplate3 = Template::New(case3_str);
    
    String case4_str = "{{/*f4\r\n*/}}";
    auto htmlTemplate4 = Template::New(case4_str);
    return 0;
}
