#include <iostream>
#include <string>

#include "TutorialConfig.h"

#ifdef USE_MY_MATH

#include <MathFunctions.h>

#endif

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
             << Tutorial_VERSION_MINOR << endl;
        cout << "Usage: " << argv[0] << " number" << endl;
        return 1;
    }

    const double inputValue = std::stod(argv[1]);
#ifdef USE_MY_MATH
    cout << "use my math" << endl;
    const string &outputValue = format_double(mysqrt(inputValue), 16);
#else
    cout << "use c math" << endl;
    const double outputValue = sqrt(inputValue);
#endif
    cout << "The square root of " << inputValue << " is " << outputValue
         << endl;
    return 0;
}
