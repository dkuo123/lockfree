#include <iostream>
#include <iterator>
#include <string>
#include <regex>
using namespace std;

void proc_signal(const string& signal, string rhs) {
  cout << "check to make sure these signals are there before" << endl;
  regex reg2("(\\w+)\\[([\\s,\\d]*)\\]");
  sregex_iterator it(rhs.begin(), rhs.end(), reg2), it_end;
  while (it != it_end) {
    cout << "processing elements " << it->str() << endl;
    smatch match = *it;
    //cout << "size = " << match.size() << ", length = " << match.length() << endl;
    cout << "signal = " << match.str(1) << ", tenors = " << match.str(2)<< endl;
    string tenors = match.str(2);
    regex reg3("(\\d)+");
    sregex_iterator it2(tenors.begin(), tenors.end(), reg3), it2_end;
    if (it2 == it2_end) {
      cout << match.str(1) << " signal has no tenor" << endl;
    }
    else
      while (it2 != it2_end) {
        cout << match.str(1) << " signal on tenor = " << it2->str() << endl;
        ++it2;
      }
    ++it;
  }

}

void proc_event(const string& signal, string rhs) {
  regex reg1("AlphaSignal\\$(.+)\\$");
  smatch result;

  if (regex_search(rhs, result, reg1)) {
    cout << signal << " uses other signal: " << result[1].str() << endl;
    proc_signal(signal, result[1].str());
    rhs = regex_replace(rhs, reg1,"");
  }
  else
    cout <<"no other AlphaSignal" << endl;

  cout << "processing pure callbacks without signal: " << rhs << endl;
  regex reg2("(\\w+)\\[([\\s,\\d]*)\\]");
  sregex_iterator it(rhs.begin(), rhs.end(), reg2), it_end;
  while (it != it_end) {
    cout << it->str() << endl;
    smatch match = *it;
    cout << "size = " << match.size() << ", length = " << match.length() << endl;
    cout << "event = " << match.str(1) << ", tenor = " << match.str(2)<< endl;
    string tenors = match.str(2);
    regex reg3("(\\d)+");
    sregex_iterator it2(tenors.begin(), tenors.end(), reg3), it2_end;
    if (it2 == it2_end)
      cout << "no tenor" << endl;
    else
      while (it2 != it2_end) {
        cout << "tenor = " << it2->str() << endl;
        ++it2;
      }
    ++it;
  }
}

void loadAlphas(const string& cfg_file) {
  ifstream fs(cfg_file);
  if (fs.is_open()) {
    string line;
    for (auto& event: eventAlphas)
      event.resize(t2cMap.size()+1);
    for (getline(fs, line); fs.good(); getline(fs, line)) {
      try {
        smatch result;
        regex reg1("(\\w+):(.+)");
        if (regex_search(line, result, reg1)) {
          string callbk = result[1].str();
          string rhs = result[2].str();
          cout << "line = " << line <<", signal " << callbk << " has value " << rhs << endl;

          if (!rhs.empty()) { // has value
            processEvent(callbk, rhs);
            regex reg2("(\\w+)\\[([,\\d]*)\\]");
            sregex_iterator it(rhs.begin(), rhs.end(), reg2), it_end;
            while (it != it_end) {
              cout << it->str() << endl;
              smatch match = *it;
              cout << "size = " << match.size() << ", length = " << match.length() << endl;
              cout << "event = " << match.str(1) << ", tenor = " << match.str(2)<< endl; //OnBook, OnTick
              auto e = eventName2Enum.find(match.str(1));
              if (e == eventName2Enum.end())
                throw match.str(1)+ string(" isn't callable");
              AlphaEvent event = e->second;
              string tenors = match.str(2);
              regex reg3("(\\d)+")
              sregex_iterator it2(tenors.begin(), tenors.end(), reg3), it2_end;
              if (it2 == it2_end) {
                cout << "no tenor" << endl;
                AlphaElement* alg = AlphaElementFactory::create(callbk); // PriceImpact
                alphas.emplace_back(alg);
                eventAlphas[event][0].push_back(alg);
              }
              else
                while (it2 != it2_end) {
                  cout << "tenor = " << it2->str() << endl;
                  AlphaElement* alg = AlphaElementFactory::create(callbk); // PriceImpact
                  alphas.emplace_back(alg);
                  eventAlphas[event][t2cMap[atoi(it2->str())]+1].push_back(alg);
                  ++it2;
                }
              ++it;
            }
          }
          else
            throw callbk + string(" must has value");
        }
        else
          throw string("Ignore. Wrong format for line: ") +  line;
      }
      catch(regex_error& e) {
        cout << e << endl;
      }
      catch(string& e) {
        cout << e << endl;
        continue;
      }
    }

  }
  else
    throw string("alphaConfig file doesn't exist") + cfg_file;
}

int main()
{
  //std::string line = "ShortTenorSignal: onBook[2,3,5, 7,  10,30] onTick[2,3,5,7,10,30]  AlphaSignal$ BookPressure[2,3]   PriceImpact[2,3] $ onReady[] ";
  loadAlphas("alpha.cfg");
}

