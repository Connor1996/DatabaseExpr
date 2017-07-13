#include <QCoreApplication>
#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>
#include <cmath>
#include <sstream>

using namespace std;

template <typename T>
ostream& operator << (ostream& out, vector<T> t) {
    out << "[";
    for (int i(0); i < t.size(); i++) {
        out << t[i];
        if (i < t.size() - 1)
            out << ",";
    }
    out << "]";
    return out;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);    
        vector<vector<string>> vec = { { "a","b","1.2","1.1" },
        { "c","d","1.3","1.4" },
        { "e","f","1.5","1.5" },
        { "g","h","1.2","1.6" },
        { "h","j","1.2","1.4" } };
        vector<vector<string>> rslts;//6¨¢D .. + Prb9 Prb6
        for (int i(0); i < vec.size(); i++) {
            vector<string> rslt;
            rslt.push_back(vec[i][0]);
            rslt.push_back(vec[i][1]);
            rslt.push_back(vec[i][2]);
            rslt.push_back(vec[i][3]);
            stringstream ss;
            string meanStr = vec[i][2], stdStr = vec[i][3];
            float mean, std;
            ss << meanStr;
            ss >> mean;
            ss.clear();
            ss << stdStr;
            ss >> std;
            float radical2 = pow(2, 1 / 2);
            float inErf = (9 - mean) / (std * radical2);
            float prb9 = (1 + erf(inErf)) / 2;
            ss.clear();
            ss << prb9;
            string prb9Str;
            ss >> prb9Str;
            rslt.push_back(prb9Str);
            inErf = (6 - mean) / (std * radical2);
            float prb6 = (1 + erf(inErf)) / 2;
            ss.clear();
            ss << prb6;
            string prb6Str;
            ss >> prb6Str;
            rslt.push_back(prb6Str);
            rslts.push_back(rslt);
            for (int i(0); i < rslt.size(); i++)
                cout << rslt[i] << endl;
        }
        cout << rslts << endl;

    return a.exec();
}

