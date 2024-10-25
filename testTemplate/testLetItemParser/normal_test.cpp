#include "Template.hpp"

using namespace obotcha;
using namespace gagira;

int normal_test() {
    String case1_str = "{{let int v = 100}}";
    auto htmlTemplate = Template::New(case1_str);
    
    String case2_str = "{{let int v   =   99    }}";
    auto htmlTemplate2 = Template::New(case2_str);
    
    String case3_str = "{{let   int     v   =   98    }}";
    auto htmlTemplate3 = Template::New(case3_str);
    
    String case4_str = "{{let   double     v   =   98.1123    }}";
    auto htmlTemplate4 = Template::New(case4_str);
    
    String case5_str = "{{let   string     v   =   \"hello world\"    }}";
    auto htmlTemplate5 = Template::New(case5_str);
    return 0;
}
